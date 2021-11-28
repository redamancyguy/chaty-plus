//
// Created by 孙文礼 on 2021/11/27.
//

#include "Array.h"
Array *ArrayListNew(long long capacity){
    Array *array = (Array *) malloc(sizeof(Array));
    if (pthread_rwlock_init(&array->rwlock, NULL) != 0) {
        exit(99);
    }
    array->array = (void *) malloc(sizeof(void *) * capacity);
    if (array->array == NULL) {
        exit(98);
    }
    array->size = 0;
    array->capacity = capacity;
    return array;
}

void ArrayDestroy(Array *array) {
    pthread_rwlock_destroy(&array->rwlock);
    free(array->array);
    free(array);
}

bool ArraySetCapacity(Array *array, unsigned long long capacity) {
    void *temp = realloc(array->array, sizeof(void *) * capacity);
    if (temp == NULL) {
        return false;
    }
    if (temp != array->array) {
        array->array = temp;
    }
    array->capacity = capacity;
    return true;
}

bool ArrayPushBack(Array *array, void *object) {
    pthread_rwlock_wrlock(&array->rwlock);
    if (array->size < array->capacity) {
        array->array[array->size++] = object;
        pthread_rwlock_unlock(&array->rwlock);
        return true;
    } else {
        if (ArraySetCapacity(array, array->capacity * 2)) {
            array->array[array->size++] = object;
            pthread_rwlock_unlock(&array->rwlock);
            return true;
        }
        pthread_rwlock_unlock(&array->rwlock);
        return false;
    }
}

bool ArrayInsert(Array *array, unsigned long long index, void *object) {
    pthread_rwlock_wrlock(&array->rwlock);
    if(array->size > array->capacity){
        if(!ArraySetCapacity(array,array->capacity*2)){
            pthread_rwlock_unlock(&array->rwlock);
            return false;
        }
    }
    for (unsigned long long i = array->size; i > index; i--) {
        array->array[i] = array->array[i - 1];
    }
    array->array[index] = object;
    ++array->size;
    pthread_rwlock_unlock(&array->rwlock);
    return true;
}

bool ArrayErase(Array *array, unsigned long long index) {
    pthread_rwlock_wrlock(&array->rwlock);
    if(index >= array->size){
        pthread_rwlock_unlock(&array->rwlock);
        return false;
    }
    for (unsigned long long i = index; i < array->size; i++) {
        array->array[i] = array->array[i + 1];
    }
    --array->size;
    pthread_rwlock_unlock(&array->rwlock);
    return true;
}
bool ArrayDelete(Array *array,void *value) {
    pthread_rwlock_wrlock(&array->rwlock);
    unsigned long long index;
    for (index = 0; index < array->size; index++) {
        if(array->array[index] == value){
            break;
        }
    }
    for (unsigned long long i = index; i < array->size; i++) {
        array->array[i] = array->array[i + 1];
    }
    --array->size;
    pthread_rwlock_unlock(&array->rwlock);
    return true;
}
size_t ArraySize(Array *array){
    pthread_rwlock_rdlock(&array->rwlock);
    size_t result = array->size;
    pthread_rwlock_unlock(&array->rwlock);
    return result;
}

void *ArrayGet(Array *array,size_t index){
    pthread_rwlock_rdlock(&array->rwlock);
    void *result = array->array[index];
    pthread_rwlock_unlock(&array->rwlock);
    return result;
}

void ArraySet(Array *array,size_t index,void *value){
    pthread_rwlock_wrlock(&array->rwlock);
    array->array[index] = value;
    pthread_rwlock_unlock(&array->rwlock);
}


bool ArrayContain(Array *array, void *object) {
    pthread_rwlock_rdlock(&array->rwlock);
    for (unsigned long long i = 0; i < array->size; i++) {
        if(array->array[i] == object){
            pthread_rwlock_unlock(&array->rwlock);
            return true;
        }
    }
    pthread_rwlock_unlock(&array->rwlock);
    return false;
}

