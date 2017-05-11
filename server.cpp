#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{

    this->listen(QHostAddress::Any,FILE_PORT_TCP);
}

void Server::incomingConnection(int socketId)
{
    fileRecvTask = new FileRecvTask;
    fileRecvTask->socket->setSocketDescriptor(socketId);
    connect(fileRecvTask->socket,SIGNAL(readyRead()),
            this,SLOT(socketReadyRead()));
}

void Server::socketReadyRead()
{
    fileRecvTask->readSocket();
}
