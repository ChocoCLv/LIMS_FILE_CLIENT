#include "client.h"

Client::Client(QObject *parent):QObject(parent)
{
    log = Log::getInstance();
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
    emit log->logStr(QString("prepare send file:%1 to client:%2").arg(fileName).arg(dst));
    QHostAddress clientAddr;
    clientAddr.setAddress(dst);

    FileSendTask* fileSendTask = new FileSendTask();
    fileSendTask->setClientIp(clientAddr);
    fileSendTask->setFileName(fileName);
    fileSendTask->setWorkDir(workDir);
    QThread *sendThread = new QThread();
    fileSendTask->moveToThread(sendThread);
    connect(this,SIGNAL(startTask(QThread*)),fileSendTask,SLOT(startTask(QThread*)));
    connect(fileSendTask,SIGNAL(taskOver(FileSendTask*)),this,SLOT(releaseThreadResourse(FileSendTask*)));
    sendThread->start();
    emit startTask(sendThread);
}

void Client::releaseThreadResourse(FileSendTask *task)
{
    task->getThread()->quit();
    task->deleteLater();
    emit taskOver();
}

