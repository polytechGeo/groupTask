#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
    //server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
        QTcpSocket *newSocket;
        newSocket = mTcpServer->nextPendingConnection();
        allSockets.push_back(newSocket);
        newSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(newSocket, &QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
        connect(newSocket,&QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* newSocket = (QTcpSocket*)sender();
    QString res="";
    while(newSocket->bytesAvailable()>0)
    {
         QByteArray array=newSocket->readAll();
         res.append(array);
    }
    newSocket->write(res.toUtf8());

}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* newSocket = (QTcpSocket*)sender();
    qDebug() << allSockets.size();
    allSockets.remove(newSocket);
    newSocket->close();
}
