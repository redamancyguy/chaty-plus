//
// Created by 孙文礼 on 2021/11/27.
//
#include <pthread.h>
#include <stdbool.h>
#ifndef HASH_H
#define HASH_H

typedef struct Node{
    void *key,*value;
    bool status;
    struct Node *next;
}Node;

typedef struct{
    pthread_rwlock_t lock;
    Node *table;
    long long size;
    long long capacity;
}Hash;

Hash *HashNew(long long capacity);
void HashDestroy(Hash *hash);
bool HashErase(Hash *hash, void *key);
bool HashGet(Hash *hash,void *key,void **value);
bool HashSet(Hash *hash,void *key,void *value);
bool HashInsert(Hash *hash, void *key, void *value);
size_t HashSize(Hash *hash);
#endif //HASH_H
