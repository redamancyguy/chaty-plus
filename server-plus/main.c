#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "BufferQueue.h"
#include "Utils/Array.h"

#include "Client.h"

const unsigned short serverPort = 19999;
size_t threadNumber = 10;
int serverFileDescriptor;


size_t MaxClientNumber = 4;
//size_t MaxClientNumber = 1024*65536;
Client *clients;
size_t clientsIndex = 0;


size_t MaxGroupNumber = 4;
//size_t MaxGroupNumber = 1024*65536;
Tree *groups;
bool *groupsUsing;
size_t groupsIndex = 0;

_Noreturn void *Handle(BufferQueue *queue) {
    unsigned int HandleBufSize = 64;
    unsigned sleepPace = 1024;
    while (true) {
        if (!BufferQueueIsEmpty(queue)) {
            Message messages[HandleBufSize];
            size_t length = 0;
            for (size_t i = 0; i < HandleBufSize && !BufferQueueIsEmpty(queue); i++) {
                messages[length++] = *Front(queue);
                BufferQueuePop(queue);
            }
            for (size_t i = 0; i < length; i++) {
                Message message = messages[i];
                printf("%hhu.", *(char *) (&message.address.sin_addr.s_addr));
                printf("%hhu.", *((char *) (&message.address.sin_addr.s_addr) + 1));
                printf("%hhu.", *((char *) (&message.address.sin_addr.s_addr) + 2));
                printf("%hhu:", *((char *) (&message.address.sin_addr.s_addr) + 3));
                printf("%d\t", message.address.sin_port);
                switch (message.data.code) {
                    case TOUCH: {
                        size_t id = message.data.id;
                        if(id < MaxClientNumber){
                            pthread_rwlock_wrlock(&clients[id].rwlock);
                            if(clients[id].Online){
                                clients[id].time = time(NULL);
                                printf("%lu Touch Online !\n",id);
                            }else{
                                clients[id].address = message.address;
                                clients[id].length = message.length;
                                clients[id].time = time(NULL);
                                clients[id].Online = true;
                                printf("%lu Touch Offline !\n",id);
                            }
                            pthread_rwlock_unlock(&clients[id].rwlock);
                        }else{ // distribute an id
                            size_t newId = clientsIndex;
                            bool flag = false;
                            for(size_t ii = clientsIndex,size = MaxClientNumber;ii<size;ii++){
                                pthread_rwlock_rdlock(&clients[ii].rwlock);
                                bool result = clients[ii].Online;
                                pthread_rwlock_unlock(&clients[ii].rwlock);
                                if(!result){
                                    newId = ii;
                                    flag = true;
                                    break;
                                }
                            }
                            if(!flag){
                                for(size_t ii=0;ii<clientsIndex;ii++){
                                    pthread_rwlock_rdlock(&clients[ii].rwlock);
                                    bool result = clients[ii].Online;
                                    pthread_rwlock_unlock(&clients[ii].rwlock);
                                    if(!result){
                                        newId = ii;
                                        flag = true;
                                        break;
                                    }
                                }
                            }
                            if(!flag){
                                message.data.code = ERROR;
                                puts("Too many clients");
                            }else{
                                clientsIndex = message.data.id = newId;
                                message.data.code = ID;
                                printf("New Id : %lu\n",message.data.id);
                            }
                        }
                        break;
                    }
                    case NEW_GROUP:{
                        size_t newId = groupsIndex;
                        bool flag = false;
                        for(size_t ii = groupsIndex,size = MaxGroupNumber;ii<size;ii++){
                            TreeRLock(groups[ii]);
                            bool result = groupsUsing[ii];
                            TreeUnLock(groups[ii]);
                            if(!result){
                                newId = ii;
                                flag = true;
                                break;
                            }
                        }
                        if(!flag){
                            for(size_t ii=0;ii<clientsIndex;ii++){
                                TreeRLock(groups[ii]);
                                bool result = groupsUsing[ii];
                                TreeUnLock(groups[ii]);
                                if(!result){
                                    newId = ii;
                                    flag = true;
                                    break;
                                }
                            }
                        }
                        if(!flag){
                            message.data.code = ERROR;
                            puts("Too many group ");
                        }else{
                            groupsIndex = ((GroupPackage*)message.data.data)->groupId = newId;
                            message.data.code = ID;
                            TreeWLock(groups[newId]);
                            groupsUsing[newId] = true;
                            TreeUnLock(groups[newId]);
                            printf("New group Id : %lu\n",newId);
                        }
                        break;
                    }
                    case DELETE_GROUP:
                        break;
                    case JOIN_GROUP:
                        break;
                    case DETACH_GROUP:
                        break;
                    case ERROR:
                        break;
                    case CHAT:
                        break;
                    case UNKNOWN:
                        break;
                    default:{
                        break;
                    }
                }
                sendto(serverFileDescriptor, &message.data, sizeof(Package), 0,
                       (struct sockaddr *) &message.address, message.length);
            }
        } else {
            usleep(sleepPace);
        }
    }
}

_Noreturn void *GetMessage(BufferQueue *queue) {
    while (true) {
        Message *buffer = End(queue);
        long long count = recvfrom(serverFileDescriptor, &buffer->data, sizeof(Package) + 1024,
                                   0, (struct sockaddr *) &buffer->address, &buffer->length);
        switch (count) {
            case -1: {
                perror("package size < 0");
                break;
            }
            case sizeof(Package): {
                while (BufferQueueIsFull(queue)) {
                    usleep(256);
                }
                BufferQueuePush(queue);
                break;
            }
            default: {
                perror("package size error");
            }
        }
    }
}

#include "fileServer.h"

int main() {
    pthread_t fileThread;
    if (pthread_create(&fileThread, NULL, (void *(*)(void *)) fileServer, NULL) != 0) {
        exit(55);
    }
    serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverFileDescriptor < 0) {
        exit(1);
    }
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(serverPort);
    if (bind(serverFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        close(serverFileDescriptor);
        exit(2);
    }
    clients = malloc(sizeof(Client)*MaxClientNumber);
    for(size_t i=0;i<MaxClientNumber;i++){
        pthread_rwlock_init(&clients[i].rwlock,NULL);
        clients[i].Online = false;
    }
    groups = malloc(sizeof(Tree)*MaxGroupNumber);
    for(size_t i=0;i<MaxGroupNumber;i++){
        groups[i] = TreeNew();
    }
    groupsUsing = malloc(sizeof(bool)*MaxGroupNumber);
    memset(groupsUsing,0,sizeof(bool)*MaxGroupNumber);
    BufferQueue *queues[threadNumber];
    for (size_t i = 0; i < threadNumber; i++) {
        queues[i] = BufferQueueNew();
    }
    pthread_t GetThreads[threadNumber];
    for (size_t i = 0; i < threadNumber; i++) {
        pthread_create(&GetThreads[i], NULL, (void *_Nullable (*_Nonnull)(void *_Nullable)) GetMessage, queues[i]);
    }
    pthread_t HandleThreads[threadNumber];
    for (size_t i = 0; i < threadNumber; i++) {
        pthread_create(&HandleThreads[i], NULL, (void *_Nullable (*_Nonnull)(void *_Nullable)) Handle, queues[i]);
    }
    getchar();
    getchar();
    for (size_t i = 0; i < threadNumber; i++) {
        pthread_detach(GetThreads[i]);
    }
    for (size_t i = 0; i < threadNumber; i++) {
        pthread_detach(HandleThreads[i]);
    }
    for (size_t i = 0; i < threadNumber; i++) {
        BufferQueueDestroy(queues[i]);
    }
    free(clients);
    for(size_t i=0;i<MaxGroupNumber;i++){
        TreeDestroy(groups[i]);
    }
    free(groups);
    free(groupsUsing);
    return 0;
}

