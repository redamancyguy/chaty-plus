//
// Created by 孙文礼 on 2021/11/27.
//

#include "Hash.h"
#include <string.h>
#include <stdlib.h>

Hash *HashNew(long long capacity) {
    Hash *hash = (Hash *) malloc(sizeof(Hash));
    hash->table = (Node *) malloc(sizeof(Node) * capacity);
    memset(hash->table, 0, sizeof(Node) * capacity);
    hash->capacity = capacity;
    if (pthread_rwlock_init(&hash->lock, NULL) != 0) {
        exit(99);
    }
    return hash;
}

void HashDestroy(Hash *hash) {
    if (pthread_rwlock_destroy(&hash->lock) != 0) {
        exit(99);
    }
    for (long long i = 0; i < hash->capacity; i++) {
        Node *j = hash->table[i].next;
        while (j != NULL) {
            void *temp = j;
            j = j->next;
            free(temp);
        }
    }
    free(hash->table);
    free(hash);
}
size_t HashSize(Hash *hash){
    pthread_rwlock_rdlock(&hash->lock);
    size_t result = hash->size;
    pthread_rwlock_unlock(&hash->lock);
    return result;
}
bool HashErase(Hash *hash, void *key) {
    pthread_rwlock_wrlock(&hash->lock);
    Node *p = &hash->table[(long long) (key) % hash->capacity];
    while (p->next != NULL) {
        if (p->next->key == key) {
            void *temp = p->next;
            p->next = p->next->next;
            free(temp);
            pthread_rwlock_unlock(&hash->lock);
            --hash->size;
            return true;
        }
        p = p->next;
    }
    pthread_rwlock_unlock(&hash->lock);
    return false;
}

bool HashSet(Hash *hash,void *key,void *value){
    pthread_rwlock_rdlock(&hash->lock);
    Node *p = hash->table[(long long) (key) % hash->capacity].next;
    while (p != NULL) {
        if (p->key == key) {
            p->value = value;
            pthread_rwlock_unlock(&hash->lock);
            return true;
        }
        p = p->next;
    }
    pthread_rwlock_unlock(&hash->lock);
    return false;
}
bool HashGet(Hash *hash,void *key,void **value){
    pthread_rwlock_rdlock(&hash->lock);
    Node *p = hash->table[(long long) (key) % hash->capacity].next;
    while (p != NULL) {
        if (p->key == key) {
            *value = p->value;
            pthread_rwlock_unlock(&hash->lock);
            return true;
        }
        p = p->next;
    }
    pthread_rwlock_unlock(&hash->lock);
    return false;
}

bool HashInsert(Hash *hash, void *key, void *value) {
    pthread_rwlock_wrlock(&hash->lock);
    Node *p = &hash->table[(long long) (key) % hash->capacity];
    while (p->next != NULL) {
        if (p->next->key == key) {
            pthread_rwlock_unlock(&hash->lock);
            return false;
        }
        p = p->next;
    }
    p = (p->next = (Node *) malloc(sizeof(Node)));
    p->key = key;
    p->value = value;
    pthread_rwlock_unlock(&hash->lock);
    ++hash->size;
    return true;
}