//
// Created by sunwenli on 2021/11/16.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <netinet/in.h>


enum Code {
    NEW_GROUP,
    DELETE_GROUP,
    JOIN_GROUP,
    DETACH_GROUP,
    ERROR,
    TOUCH,
    CHAT,
    UNKNOWN,
};
typedef struct {
    enum Code code;
    char data[2048];
} Package;


typedef struct {
    unsigned long long groupId;
    char data[1024];
} ChatPackage;

typedef struct {
    unsigned long long groupId;
} GroupPackage;

struct TouchPackage {
};


#endif //COMMUNICATION_H
