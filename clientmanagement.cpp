#include "clientmanagement.h"

ClientManagement::ClientManagement(QObject *parent) : QObject(parent)
{
    signalingParseModule = new SignalingParseModule;
    fileManagement = FileManagement::getInstance();

    server = new Server;
    localClient = new Client;
    localClient->setClientIp(localHostAddr);
    localClient->setWorkDir(fileManagement->getWorkDirectory());
    connect(localClient,SIGNAL(taskOver()),this,SLOT(oneSendTaskOver()));
    connect(server,SIGNAL(fileRecvOver(QString)),this,SLOT(fileRecvOver(QString)));
    connect(signalingParseModule,SIGNAL(getTaskInfo(int,quint64)),this,SLOT(setTaskInfo(int,quint64)));
    connect(signalingParseModule,SIGNAL(pushFile(QString,QString)),localClient,SLOT(pushFile(QString,QString)));
}

void ClientManagement::setTaskInfo(int fileNum, quint64 totalSize)
{
    this->fileNum = fileNum;
    this->totalSize = totalSize;
}

void ClientManagement::oneSendTaskOver()
{
    QJsonObject jo;
    jo.insert("SIGNALING_TYPE","FILE_SEND_OVER");
    signalingParseModule->sendSignaling(jo);
}

void ClientManagement::getLocalHostAddress()
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(hostName);
    foreach (QHostAddress address, info.addresses()) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            localHostAddr = address;
        }
    }
}

void ClientManagement::fileRecvOver(QString fileName)
{
    QJsonObject jo;
    jo.insert("SIGNALING_TYPE","FILE_RECV_OVER");
    jo.insert("FILE_NAME",fileName);
    signalingParseModule->sendSignaling(jo);
}
