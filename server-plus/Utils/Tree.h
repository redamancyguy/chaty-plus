//
// Created by sunwenli on 2021/10/28.
//
#include <stdbool.h>
#include "Array.h"
#ifndef TREE_H
#define TREE_H

typedef struct Tree_* Tree;
Tree TreeNew();
int TreeWLock(Tree tree);
int TreeRLock(Tree tree);
int TreeUnLock(Tree tree);
Array TreeToArray(Tree tree);
unsigned long long TreeSize(Tree tree);
bool TreeDelete(Tree tree, void *key);
void *TreeGet(Tree tree,void *key);
void ShowTree(Tree tree);
bool TreeInsert(Tree tree, void *key, void *value);
void TreeDestroy(Tree tree);
Array TreeKeyToArray(Tree tree);
void *TreeMinimumKey(Tree tree);
void Clear(Tree tree);
#endif //TREE_H
