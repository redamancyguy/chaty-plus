#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "client-plus_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

//    QHostAddress serverAddr = QHostAddress("0.0.0.0");
//    quint16 port = 19999;
//    QUdpSocket socket;
//    socket.bind(serverAddr, 0,QUdpSocket::ShareAddress);
//    socket.open(QIODevice::ReadWrite);
//    Package pkg;
//    socket.writeDatagram((char*)&pkg,sizeof(Package),serverAddr,port);
//    QThread::sleep(1);
    MainWindow w;
    w.show();
    return a.exec();
}
