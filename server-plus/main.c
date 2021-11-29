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
#include "Utils/Hash.h"
#include "Utils/Array.h"
#include "Clients.h"
#include "Groups.h"

const unsigned short serverPort = 19999;
const int threadNumber = 10;
int serverFileDescriptor;
Clients *clients;
Groups *groups;

_Noreturn void *Handle(BufferQueue *queue) {
    unsigned int HandleBufSize = 64;
    unsigned sleepPace = 1024;
    while (true) {
        if (!BufferQueueIsEmpty(queue)) {
            Message messages[HandleBufSize];
            int length = 0;
            for (int i = 0; i < HandleBufSize && !BufferQueueIsEmpty(queue); i++) {
                messages[length++] = *Front(queue);
                BufferQueuePop(queue);
            }
            for (int i = 0; i < length; i++) {
                Message message = messages[i];
                printf("%hhu.", *(char *) (&message.address.sin_addr.s_addr));
                printf("%hhu.", *((char *) (&message.address.sin_addr.s_addr) + 1));
                printf("%hhu.", *((char *) (&message.address.sin_addr.s_addr) + 2));
                printf("%hhu:", *((char *) (&message.address.sin_addr.s_addr) + 3));
                printf("%d\t", message.address.sin_port);
                switch (message.data.code) {
                    case TOUCH: {
                        Client *client;
                        if (ClientsGet(clients, message.address, &client)) {
                            client->time = time(NULL);
                            puts("Touch ok !");
                        } else {
                            client = ClientNew();
                            ClientsInsert(clients, message.address, client);
                            message.data.code = ERROR;
                            client->address = message.address;
                            client->length = message.length;
                            puts("Touch error !");
                        }
                        break;
                    }
                    case NEW_GROUP: {
                        Client *client;
                        if (ClientsGet(clients, message.address, &client) && HashSize(client->group) < 16) {
                            Group *group = GroupNew();
                            ((GroupPackage *) (message.data.data))->groupId = (unsigned long long) group;
                            GroupsInsert(groups, group, group);
                            ArrayPushBack(group->array, client);
                            HashInsert(client->group, group, group);
                            puts("New group ok !");
                        } else {
                            message.data.code = ERROR;
                            puts("New group error !");
                        }
                        break;
                    }
                    case DELETE_GROUP: {
                        Client *client;
                        if (ClientsGet(clients, message.address, &client)) {
                            Group *group;
                            if (GroupsGet(groups, (void *) ((GroupPackage *) (message.data.data))->groupId, &group)) {
                                GroupsErase(groups, group);
                                for (size_t ii = 0, size = group->array->size; ii < size; ii++) {
                                    HashErase(((Client *) ArrayGet(group->array, ii))->group, group);
                                }
                                GroupDestroy(group);
                                puts("Delete group ok !");
                            } else {
                                puts("Delete group error !");
                                message.data.code = ERROR;
                            }
                        } else {
                            puts("Delete group error !");
                            message.data.code = ERROR;
                        }
                        break;
                    }
                    case JOIN_GROUP: {
                        Client *client;
                        if (ClientsGet(clients, message.address, &client)) {
                            Group *group;
                            if (!HashGet(client->group, (void *) ((GroupPackage *) (message.data.data))->groupId,
                                         (void **) &group)
                                &&
                                GroupsGet(groups, (void *) ((GroupPackage *) (message.data.data))->groupId, &group)) {
                                ArrayPushBack(group->array, client);
                                HashInsert(client->group, group, group);
                                puts("Join group ok !");
                            } else {
                                puts("Join group error !");
                                message.data.code = ERROR;
                            }
                        } else {
                            puts("Join group error !");
                            message.data.code = ERROR;
                        }
                        break;
                    }
                    case DETACH_GROUP: {
                        Client *client;
                        if (ClientsGet(clients, message.address, &client)) {
                            Group *group;
                            if (HashGet(client->group, (void *) ((GroupPackage *) (message.data.data))->groupId,
                                        (void **) &group)
                                &&
                                GroupsGet(groups, (void *) ((GroupPackage *) (message.data.data))->groupId, &group)) {
                                ArrayDelete(group->array, client);
                                HashErase(client->group, group);
                                puts("Detach group ok !");
                            } else {
                                puts("Detach group error !");
                                message.data.code = ERROR;
                            }
                        } else {
                            puts("Detach group error !");
                            message.data.code = ERROR;
                        }
                        break;
                    }
                    case CHAT: {
                        Client *client;
                        if (ClientsGet(clients, message.address, &client)) {
                            Group *group;
                            if (HashGet(client->group, (void *) ((ChatPackage *) message.data.data)->groupId,
                                        (void **) &group)) {
                                for (size_t ii = 0, size = ArraySize(group->array); ii < size; ii++) {
                                    sendto(serverFileDescriptor, &message.data, sizeof(Package), 0,
                                           (struct sockaddr *) &((Client *) ArrayGet(group->array, ii))->address,
                                           ((Client *) ArrayGet(group->array, ii))->length);
                                }
                                puts("Chat ok !");
                                continue;
                            } else {
                                puts("Chat error !");
                                message.data.code = ERROR;
                            }
                        } else {
                            puts("Chat error !");
                            message.data.code = ERROR;
                        }
                        break;
                    }
                    default: {
                        message.data.code = UNKNOWN;
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
    clients = ClientsNew();
    groups = GroupsNew();
    BufferQueue *queues[threadNumber];
    for (int i = 0; i < threadNumber; i++) {
        queues[i] = BufferQueueNew();
    }
    pthread_t GetThreads[threadNumber];
    for (int i = 0; i < threadNumber; i++) {
        pthread_create(&GetThreads[i], NULL, (void *_Nullable (*_Nonnull)(void *_Nullable)) GetMessage, queues[i]);
    }
    pthread_t HandleThreads[threadNumber];
    for (int i = 0; i < threadNumber; i++) {
        pthread_create(&HandleThreads[i], NULL, (void *_Nullable (*_Nonnull)(void *_Nullable)) Handle, queues[i]);
    }
    getchar();
    getchar();
    for (int i = 0; i < threadNumber; i++) {
        pthread_detach(GetThreads[i]);
    }
    for (int i = 0; i < threadNumber; i++) {
        pthread_detach(HandleThreads[i]);
    }

    for (int i = 0; i < threadNumber; i++) {
        BufferQueueDestroy(queues[i]);
    }
    return 0;
}

