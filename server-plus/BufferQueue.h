//
// Created by 孙文礼 on 2021/11/25.
//
#include "Communication.h"
#include <pthread.h>
#include <netinet/in.h>
#include <stdbool.h>

#ifndef BUFFERQUEUE_H
#define BUFFERQUEUE_H

typedef struct {
    struct sockaddr_in address;
    socklen_t length;
    Package data;
}Message;


typedef struct {
        pthread_rwlock_t lock;
        unsigned short head, tail;
        Message data[65536];
        bool status;
}BufferQueue;


BufferQueue *BufferQueueNew();
void BufferQueueDestroy(BufferQueue *queue);
void BufferQueuePush(BufferQueue *queue);
void BufferQueuePop(BufferQueue *queue);
bool BufferQueueIsFull(BufferQueue *queue);
bool BufferQueueIsEmpty(BufferQueue *queue);
Message *Front(BufferQueue *queue);
Message *End(BufferQueue *queue);

#endif //BUFFERQUEUE_H
