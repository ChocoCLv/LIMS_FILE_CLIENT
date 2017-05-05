#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    fileRecvTask = new FileRecvTask;
    this->listen(QHostAddress::Any,FILE_PORT_TCP);
    connect(fileRecvTask->socket,SIGNAL(readyRead()),
            this,SLOT(socketReadyRead()));
}

void Server::incomingConnection(int socketId)
{
    fileRecvTask->socket->setSocketDescriptor(socketId);
}

void Server::socketReadyRead()
{
    fileRecvTask->readSocket();
}
