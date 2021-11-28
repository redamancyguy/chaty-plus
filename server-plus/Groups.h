//
// Created by 孙文礼 on 2021/11/28.
//
#include "Utils/Array.h"
#include "Utils/Hash.h"
#ifndef GROUPS_H
#define GROUPS_H


typedef struct {
    Array *array;
}Group;

typedef struct {
    Hash *hash[65536];
}Groups;
Group *GroupNew();
void GroupDestroy(Group *group);

Groups *GroupsNew();
void GroupsDestroy(Groups *groups);
bool GroupsInsert(Groups *groups,void *key,Group *group);
bool GroupsErase(Groups *groups,void *key);
bool GroupsSet(Groups *groups,void *key,Group *group);
bool GroupsGet(Groups *groups,void *key,Group **group);
#endif //GROUPS_H
