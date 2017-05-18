#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    log = Log::getInstance();
    this->listen(QHostAddress::Any,FILE_PORT_TCP);
}

void Server::incomingConnection(int socketId)
{

    emit log->logStr(Log::COMMON_LOG,"accept connection");
    FileRecvTask *fileRecvTask = new FileRecvTask();
    QThread *recvThread = new QThread();
    fileRecvTask->moveToThread(recvThread);
    connect(this,SIGNAL(startTask(int,QThread*)),fileRecvTask,SLOT(startRecvTask(int,QThread*)));
    connect(fileRecvTask,SIGNAL(taskOver(FileRecvTask*)),this,SLOT(releaseRecvThread(FileRecvTask*)));
    recvThread->start();
    emit startTask(socketId,recvThread);
}

void Server::releaseRecvThread(FileRecvTask *task)
{
    task->getThread()->quit();
    task->deleteLater();
    emit fileRecvOver(task->getFileName());
}

