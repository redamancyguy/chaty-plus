#include "data.h"

#include <QMessageBox>
#include <QMessageBox>
#include <QThread>
#include <iostream>

#include <QUdpSocket>

void Data::Core(){
    QHostAddress serverAddress = QHostAddress(this->ipAddress);
    quint16 serverPort = this->port;
    QUdpSocket socket;
    QHostAddress clientAddress;

    socket.bind(clientAddress, 0,QUdpSocket::ShareAddress);
    socket.open(QIODevice::ReadWrite);

    Package dataBuf;
    QHostAddress tempAddress;
    quint16 tempPort;
    dataBuf.code =  TOUCH;
    dataBuf.id = 32131232131232;
    socket.writeDatagram((char*)&dataBuf,sizeof(Package),serverAddress,serverPort);
    socket.waitForBytesWritten();
    for(int i=0;i<5&&socket.readDatagram((char*)&dataBuf, sizeof(Package),&tempAddress, &tempPort) <= 0;i++){
        QThread::sleep(1);
        if(i==5){
            //ERROR
        }
    }
    qDebug()<<"temp ID"<<dataBuf.id;
    this->id = dataBuf.id;
    while (true) {
        qint64 len = socket.readDatagram((char*)&dataBuf, sizeof(Package),&tempAddress, &tempPort);
        if (len == -1) {
          QThread::msleep(100);
        }else{
            this->ReceiveMessageMutex.lock();
            this->ReceiveMessageQueue.push(dataBuf);
            this->ReceiveMessageMutex.unlock();
        }
        this->SendMessageMutex.lock();
        if(!this->SendMessageQueue.empty()){
            dataBuf = SendMessageQueue.front();
            this->SendMessageQueue.pop();
            this->SendMessageMutex.unlock();
            dataBuf.id = this->id;
            socket.writeDatagram((char*)&dataBuf,sizeof(Package),serverAddress,serverPort);
            socket.waitForBytesWritten();
        }else{
            this->SendMessageMutex.unlock();
            QThread::msleep(100);
        }
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
    this->receive = new std::thread(&Data::Core,this);
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

