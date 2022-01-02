#ifndef DATA_H
#define DATA_H

#include <QDateTime>
#include <QUdpSocket>
#include <thread>
#include <queue>

#include <unordered_map>
#include "Communication.h"
#include <mutex>

class Data
{
public:
    void Chat(unsigned long long groupId,QString data);
    void JoinGroup(unsigned long long groupId);
    void DetachGroup(unsigned long long groupId);
    void DeleteGroup(unsigned long long groupId);
    void NewGroup();
    void GetMessage(Package *dataBuf);
    void Touch();
    Data();
    ~Data();
private:
    size_t id;
    const char *ipAddress = "0.0.0.0";
    const unsigned short port = 19999;
    std::mutex ReceiveMessageMutex;
    std::queue<Package> ReceiveMessageQueue;
    std::mutex SendMessageMutex;
    std::queue<Package> SendMessageQueue;
    void Core();
    std::thread *receive;
};

#endif // DATA_H
