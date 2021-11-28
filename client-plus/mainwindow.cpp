#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTcpSocket>
#define BufSize 16384
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->TouchingThread = new std::thread(&MainWindow::Touching,this);
    this->GettingThread = new std::thread(&MainWindow::Getting,this);
}
void MainWindow::Touching(){
    while (true) {
        this->data.Touch();
        QThread::msleep(3000);
    }
}
void MainWindow::Getting(){
    while (true) {
        Package dataBuf;
        this->data.GetMessage(&dataBuf);
        switch (dataBuf.code) {
        case TOUCH:{
            qDebug()<<"TOUCHING";
            break;
        }
        case ERROR:{
            qDebug()<<"error";
            break;
        }
       case NEW_GROUP:{
            GroupPackage *package = (GroupPackage *)(dataBuf.data);
            this->ui->listWidget->addItem(QString::number(package->groupId));
            break;
        }
        case CHAT:{
            ChatPackage *package = (ChatPackage *)(dataBuf.data);
            ui->textBrowser->append(QString::number(package->groupId));
            ui->textBrowser->append(package->data);
            QTextCursor cursor = ui->textBrowser->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->textBrowser->setTextCursor(cursor);
            break;
        }
        case DELETE_GROUP:
            break;
        case JOIN_GROUP:
            break;
        case DETACH_GROUP:
            break;
        case UNKNOWN:
            break;
        }
    }
}
MainWindow::~MainWindow()
{
    this->TouchingThread->detach();
        delete this->TouchingThread;
    this->GettingThread->detach();
    delete this->GettingThread;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
    QString text = ui->textEdit->toPlainText();
    this->ui->textEdit->clear();
    for(auto i:this->ui->listWidget->selectedItems()){
        this->data.Chat(i->text().toULongLong(),text);
    }
  }
}

void MainWindow::on_pushButton_clicked()
{
    this->data.NewGroup();
}


void MainWindow::on_pushButton_2_clicked()
{
    for(auto i:this->ui->listWidget->selectedItems()){
        this->ui->listWidget->takeItem(this->ui->listWidget->currentIndex().row());
        this->data.DeleteGroup(i->text().toULongLong());
    }
}

struct fileInfo {
    bool flag;
    long long size;
    char fileName[128];
};
void MainWindow::on_pushButton_5_clicked()
{
    QString filePath = QFileDialog::getOpenFileName();
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QList<QString> list = filePath.split("/");


    for(auto i:this->ui->listWidget->selectedItems()){
        this->data.Chat(i->text().toULongLong(),"File : "+list[list.size() - 1]);
    }



    fileInfo info;
    strcpy(info.fileName,list[list.size() - 1].toStdString().c_str());
    QTcpSocket tcp;
    info.flag = true;
    info.size = file.size();

    tcp.connectToHost("0.0.0.0",9999);

    if(!tcp.waitForConnected(30000))
    {
        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
        return;
    }
    tcp.write((char*)&info,sizeof(fileInfo));
    tcp.waitForBytesWritten(300000);
    long long i=0;
    while(i<info.size){
        char buf[BufSize];
        long long temp = file.read(buf,BufSize);
        if(temp < 0){
            break;
        }
        qDebug()<<temp;
        tcp.write(buf,temp);
        tcp.waitForBytesWritten(300000);
        i+=temp;
    }
    qDebug()<<i;
    QThread::msleep(1000);
    tcp.disconnectFromHost();
    file.close();
}


void MainWindow::on_pushButton_6_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory();
    QFile file(filePath+"/"+this->ui->lineEdit->text());
    file.open(QIODevice::WriteOnly);
    fileInfo info;
    strcpy(info.fileName,this->ui->lineEdit->text().toStdString().c_str());
    QTcpSocket tcp;
    info.flag = false;
//    info.size = file.size();

    tcp.connectToHost("0.0.0.0",9999);

    if(!tcp.waitForConnected(30000))
    {
        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
        return;
    }
    tcp.write((char*)&info,sizeof(fileInfo));
    tcp.waitForBytesWritten(300000);
    tcp.flush();

    tcp.read((char*)&info,sizeof(fileInfo));//????????????????????
    tcp.waitForReadyRead(300000);


    qDebug()<<tcp.read((char*)&info,sizeof(fileInfo));
    tcp.waitForReadyRead(300000);
     qDebug()<<info.size;
    long long i=0;
    while(i<info.size){
        char buf[BufSize];
        long long temp = tcp.read(buf,BufSize);
        tcp.waitForReadyRead(300000);
        if(temp < 0){
            break;
        }
        file.write(buf,temp);
        i+=temp;
    }
    tcp.disconnectFromHost();
    file.close();
}

