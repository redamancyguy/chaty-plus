//
// Created by 孙文礼 on 2021/11/28.
//

#ifndef FILESERVER_H
#define FILESERVER_H
#include <pthread.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
struct fileInfo {
    bool flag;
    long long size;
    char fileName[128];
};
extern const unsigned short serverPort;
#define bufSize 16384
void dealFile(int client) {
    struct fileInfo info;
    if (read(client, (void *) &info, sizeof(struct fileInfo)) < 0) {
        info.flag = false;
        if (write(client, (void *) &info, sizeof(struct fileInfo)) < 0) {
            perror("return false error");
        }
    }
    long long i = 0;
    FILE *fp;
    if (info.flag) {
        printf("Post : %s %lld\n",info.fileName,info.size);
        char fileName[164];
        sprintf(fileName,"file/%s",info.fileName);
        if ((fp = fopen(fileName, "w")) == NULL) {
            info.flag = false;
            if (write(client, (void *) &info, sizeof(struct fileInfo)) < 0) {
                perror("return false error");
            }
            perror("Get info error");
            close(client);
            return;
        }
        while (i < info.size) {
            char buf[bufSize] = {0};
            long long temp;
            if ((temp = read(client, (void *) buf, sizeof(char) * bufSize)) < 0) {
                break;
            }
            if(temp == 0){
                usleep(1024);
                if ((temp = read(client, (void *) buf, sizeof(char) * bufSize)) <= 0) {
                    break;
                }
            }
            i += temp;
            fwrite(buf, 1, temp, fp);
        }
    } else {
        char fileName[164];
        sprintf(fileName,"file/%s",info.fileName);
        if ((fp = fopen(fileName, "r")) == NULL) {
            info.flag = false;
            if (write(client, (void *) &info, sizeof(struct fileInfo)) < 0) {
                perror("return false error");
            }
            perror("Get info error");
            close(client);
            return;
        }
        fseek(fp, 0, SEEK_END);
        info.size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        printf("Get : %s %lld\n",info.fileName,info.size);
        if (write(client, (void *) &info, sizeof(struct fileInfo)) < 0) {
            perror("return file info error");
        }
        while (i < info.size) {
            char buf[bufSize] = {0};
            long long temp;
            if ((temp = fread(buf, 1, sizeof(char) * bufSize, fp)) < 0) {
                break;
            }
            i += temp;
            write(client, buf, temp);
        }
    }
    fclose(fp);
    if (i == info.size) {
        info.flag = true;
        puts("OK");
    } else {
        info.flag = false;
        puts("Not ok");
    }
    if (write(client, (void *) &info, sizeof(struct fileInfo)) < 0) {
        perror("return true error");
    }
    close(client);
}

void fileServer() {
    int serverFD;
    struct sockaddr_in sockAddress;
    socklen_t addressLen = sizeof(struct sockaddr_in);
    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    memset(&sockAddress, 0, sizeof(struct sockaddr_in));
    sockAddress.sin_family = AF_INET;
    sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddress.sin_port = htons(serverPort);
    if (bind(serverFD, (struct sockaddr *) (&sockAddress), sizeof(struct sockaddr)) < 0) {
        perror("Bind");
        exit(1);
    }
    if (listen(serverFD, 4) < 0) {
        perror("Listen");
        exit(1);
    }
    while (true) {
        int clientFD;
        struct sockaddr_in clientAddress;
        socklen_t clientLen = sizeof(struct sockaddr_in);
        if ((clientFD = accept(serverFD, (struct sockaddr *) (&clientAddress), &clientLen)) < 0) {
            perror("Accept");
            continue;
        }
        pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *)) dealFile, (void*)(clientFD));
        pthread_detach(thread);
    }
}
#endif //FILESERVER_H
