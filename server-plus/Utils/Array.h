//
// Created by 孙文礼 on 2021/11/27.
//
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
    pthread_rwlock_t rwlock;
    void **array;
    long long size;
    long long unsigned capacity;
} Array;

Array *ArrayListNew(long long capacity);

void ArrayDestroy(Array *array);

bool ArrayPushBack(Array *array, void *object);

bool ArrayInsert(Array *array, unsigned long long index, void *object);

bool ArrayErase(Array *array, unsigned long long index);
bool ArrayDelete(Array *array,void *value);
bool ArrayContain(Array *array, void *object);
size_t ArraySize(Array *array);
void *ArrayGet(Array *array,size_t index);

void ArraySet(Array *array,size_t index,void *value);

#endif //ARRAY_H
