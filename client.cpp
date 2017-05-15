#include "client.h"

Client::Client(QObject *parent):QObject(parent)
{
}

void Client::setWorkDir(QString dir)
{
    workDir = dir;
}

void Client::setClientIp(QHostAddress ip)
{
    clientIp = ip;
}

QHostAddress Client::getClientHostAddress()
{
    return clientIp;
}

void Client::pushFile(QString dst, QString fileName)
{
    QHostAddress clientAddr;
    clientAddr.setAddress(dst);

    FileSendTask* fileSendTask = new FileSendTask();
    fileSendTask->setClientIp(clientAddr);
    fileSendTask->setFileName(fileName);
    fileSendTask->setWorkDir(workDir);
    QThread *sendThread = new QThread();
    fileSendTask->moveToThread(sendThread);
    connect(this,SIGNAL(startTask()),fileSendTask,SLOT(connectToClient()));
    connect(fileSendTask,SIGNAL(taskOver()),fileSendTask,SLOT(deleteLater()));
    connect(fileSendTask,SIGNAL(taskOver()),sendThread,SLOT(deleteLater()));
    connect(fileSendTask,SIGNAL(taskOver()),this,SIGNAL(taskOver()));
    sendThread->start();
    emit startTask();
}

