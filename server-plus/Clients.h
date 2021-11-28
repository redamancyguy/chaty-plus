//
// Created by 孙文礼 on 2021/11/28.
//
#include <stdbool.h>
#include <netinet/in.h>
#include "Utils/Hash.h"
#ifndef CLIENTS_H
#define CLIENTS_H



typedef struct {
        time_t time;
        struct sockaddr_in address;
        socklen_t length;
        Hash *group;
}Client;

typedef struct {
    long long size;
    Hash *hash[65536];
}Clients;

Client *ClientNew();

void ClientDestroy(Client *client);

Clients *ClientsNew();

void ClientsDestroy(Clients *clients);

bool ClientsInsert(Clients *clients,struct sockaddr_in address,Client *client);
bool ClientsErase(Clients *clients,struct sockaddr_in address);
bool ClientsGet(Clients *clients,struct sockaddr_in address,Client **client);
bool ClientsSet(Clients *clients,struct sockaddr_in address,Client *client);

#endif //CLIENTS_H
