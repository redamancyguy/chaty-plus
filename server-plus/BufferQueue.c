//
// Created by 孙文礼 on 2021/11/25.
//

#include <stdlib.h>
#include "BufferQueue.h"


BufferQueue *BufferQueueNew() {
    BufferQueue *queue = (BufferQueue *) malloc(sizeof(BufferQueue));
    if (pthread_rwlock_init(&queue->lock, NULL) != 0) {
        exit(99);
    }
    for (int i = 0; i < 65536; i++) {
        queue->data[i].length = sizeof(struct sockaddr_in);
    }
    queue->head = queue->tail = 0;
    queue->status = false;
    return queue;
}

void BufferQueueDestroy(BufferQueue *queue) {
    if (pthread_rwlock_destroy(&queue->lock) != 0) {
        exit(99);
    }
    free(queue);
}

void BufferQueuePush(BufferQueue *queue) {
    pthread_rwlock_wrlock(&queue->lock);
    queue->status = true;
    ++queue->tail;
    pthread_rwlock_unlock(&queue->lock);
}

void BufferQueuePop(BufferQueue *queue) {
    pthread_rwlock_wrlock(&queue->lock);
    queue->status = false;
    ++queue->head;
    pthread_rwlock_unlock(&queue->lock);
}

bool BufferQueueIsFull(BufferQueue *queue) {
    pthread_rwlock_rdlock(&queue->lock);
    bool result = queue->head == queue->tail && queue->status;
    pthread_rwlock_unlock(&queue->lock);
    return result;
}

bool BufferQueueIsEmpty(BufferQueue *queue) {
    pthread_rwlock_rdlock(&queue->lock);
    bool result = queue->head == queue->tail && !queue->status;
    pthread_rwlock_unlock(&queue->lock);
    return result;
}

Message *Front(BufferQueue *queue) {
    pthread_rwlock_rdlock(&queue->lock);
    Message *result = &queue->data[queue->head];
    pthread_rwlock_unlock(&queue->lock);
    return result;
}

Message *End(BufferQueue *queue) {
    pthread_rwlock_rdlock(&queue->lock);
    Message *result = &queue->data[queue->tail];
    pthread_rwlock_unlock(&queue->lock);
    return result;
}
