//
// Created by 孙文礼 on 2021/11/28.
//
#include <stdlib.h>
#include "Clients.h"


Client *ClientNew(){
    Client *client = (Client*) malloc(sizeof(Client));
    client->group = HashNew(16);
    return client;
}

void ClientDestroy(Client *client){
    HashDestroy(client->group);
    free(client);
}

Clients *ClientsNew(){
    Clients *clients = malloc(sizeof(Clients));
    for(int i=0;i<65536;i++){
        clients->hash[i] = HashNew(1024);
    }
    clients->size = 0;
    return clients;
}

void ClientsDestroy(Clients *clients){
    for(int i=0;i<65536;i++){
        HashDestroy(clients->hash[i]);
    }
    free(clients);
}

bool ClientsInsert(Clients *clients,struct sockaddr_in address,Client *client){
    return HashInsert(clients->hash[address.sin_port],(void*)(long long)(address.sin_addr.s_addr),client);
}
bool ClientsErase(Clients *clients,struct sockaddr_in address){
    return HashErase(clients->hash[address.sin_port],(void*)(long long)(address.sin_addr.s_addr));
}
bool ClientsGet(Clients *clients,struct sockaddr_in address,Client **client){
    return HashGet(clients->hash[address.sin_port],(void*)(long long)(address.sin_addr.s_addr),(void**)client);
}
bool ClientsSet(Clients *clients,struct sockaddr_in address,Client *client){
    return HashSet(clients->hash[address.sin_port],(void*)(long long)(address.sin_addr.s_addr),client);
}