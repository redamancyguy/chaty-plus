//
// Created by 孙文礼 on 2021/12/8.
//
#include "Utils/Tree.h"
#include <stdbool.h>
#include <pthread.h>

#ifndef CLIENT_H
#define CLIENT_H
typedef struct {
    pthread_rwlock_t rwlock;
    bool Online;
    time_t time;
    struct sockaddr_in address;
    socklen_t length;
} Client;





#endif //CLIENT_H
