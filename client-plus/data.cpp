#include "data.h"

#include <QMessageBox>
#include <QMessageBox>
#include <QThread>
#include <iostream>

#include <QUdpSocket>

void Data::Receive(){
    QHostAddress serverAddr;
    quint16 port;
    QUdpSocket socket;
    socket.bind(serverAddr, 0,QUdpSocket::ShareAddress);
    socket.open(QIODevice::ReadWrite);
    while (true) {
        Package dataBuf;
        qint64 len = socket.readDatagram((char*)&dataBuf, sizeof(Package),&serverAddr, &port);
        if (len == -1) {
          QThread::msleep(100);
        }else{
            this->ReceiveMessageMutex.lock();
            this->ReceiveMessageQueue.push(dataBuf);
            this->ReceiveMessageMutex.unlock();
        }
        this->SendMessageMutex.lock();
        if(!this->SendMessageQueue.empty()){
            socket.writeDatagram((char*)&this->SendMessageQueue.front(),sizeof(Package),this->serverAddress,this->serverPort);
            this->SendMessageQueue.pop();
        }else{
            QThread::msleep(100);
        }
        this->SendMessageMutex.unlock();
    }
}
void Data::JoinGroup(unsigned long long groupId){
    Package dataBuf;
    dataBuf.code = JOIN_GROUP;
    GroupPackage *package = (GroupPackage *)(dataBuf.data);
    package->groupId = groupId;
    this->SendMessageMutex.lock();
    this->SendMessageQueue.push(dataBuf);
    this->SendMessageMutex.unlock();
}
void Data::DetachGroup(unsigned long long groupId){
    Package dataBuf;
    dataBuf.code = DETACH_GROUP;
    GroupPackage *package = (GroupPackage *)(dataBuf.data);
    package->groupId = groupId;
    this->SendMessageMutex.lock();
    this->SendMessageQueue.push(dataBuf);
    this->SendMessageMutex.unlock();
}
void Data::DeleteGroup(unsigned long long groupId){
    Package dataBuf;
    dataBuf.code = DELETE_GROUP;
    GroupPackage *package = (GroupPackage *)(dataBuf.data);
    package->groupId = groupId;
    this->SendMessageMutex.lock();
    this->SendMessageQueue.push(dataBuf);
    this->SendMessageMutex.unlock();
}
void Data::Chat(unsigned long long groupId,QString data){
    Package dataBuf;
    dataBuf.code = CHAT;
    ChatPackage *package = (ChatPackage *)(dataBuf.data);
    package->groupId = groupId;
    strcpy(package->data,data.toStdString().c_str());
    this->SendMessageMutex.lock();
    this->SendMessageQueue.push(dataBuf);
    this->SendMessageMutex.unlock();
}

void Data::GetMessage(Package *dataBuf){
    this->ReceiveMessageMutex.lock();
    while(this->ReceiveMessageQueue.empty()){
        this->ReceiveMessageMutex.unlock();
        QThread::msleep(100);
        this->ReceiveMessageMutex.lock();
    }
    *dataBuf = this->ReceiveMessageQueue.front();
    this->ReceiveMessageQueue.pop();
    this->ReceiveMessageMutex.unlock();
}
Data::Data(){
    this->serverAddress = QHostAddress(this->ipAddress);  // server address there
    this->serverPort = this->port;
    this->receive = new std::thread(&Data::Receive,this);
}
Data::~Data(){
    this->receive->detach();
    delete this->receive;
}
void Data::Touch(){
    Package dataBuf;
    dataBuf.code = TOUCH;
    this->SendMessageMutex.lock();
    this->SendMessageQueue.push(dataBuf);
    this->SendMessageMutex.unlock();
}
void Data::NewGroup(){
    Package dataBuf;
    dataBuf.code = NEW_GROUP;
    this->SendMessageMutex.lock();
    this->SendMessageQueue.push(dataBuf);
    this->SendMessageMutex.unlock();
}

