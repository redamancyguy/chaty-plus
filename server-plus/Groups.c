//
// Created by 孙文礼 on 2021/11/28.
//

#include "Groups.h"
Group *GroupNew(){
    Group *group = malloc(sizeof(Group));
    group->array = ArrayListNew(1024);
    return group;
}

void GroupDestroy(Group *group){
    ArrayDestroy(group->array);
    free(group);
}


Groups *GroupsNew(){
    Groups *groups = malloc(sizeof(Groups));
    for(int i=0;i<65536;i++){
        groups->hash[i] = HashNew(1024);
    }
    return groups;
}
void GroupsDestroy(Groups *groups){
    for (int i=0;i<65536;i++){
        HashDestroy(groups->hash[i]);
    }
    free(groups);
}

bool GroupsInsert(Groups *groups,void *key,Group *group){
    unsigned long long index = (unsigned long long )key;
    return HashInsert(groups->hash[index % 65536],(void*)(index/65536),group);
}
bool GroupsErase(Groups *groups,void *key){
    unsigned long long index = (unsigned long long )key;
    return HashErase(groups->hash[index % 65536],(void*)(index/65536));
}
bool GroupsSet(Groups *groups,void *key,Group *group){
    unsigned long long index = (unsigned long long )key;
    return HashSet(groups->hash[index % 65536],(void*)(index/65536),group);
}
bool GroupsGet(Groups *groups,void *key,Group **group){
    unsigned long long index = (unsigned long long )key;
    return HashGet(groups->hash[index % 65536],(void*)(index/65536),(void**)group);
}