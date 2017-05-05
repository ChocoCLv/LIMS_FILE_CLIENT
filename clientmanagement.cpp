#include "clientmanagement.h"

ClientManagement::ClientManagement(QObject *parent) : QObject(parent)
{
    signalingParseModule = new SignalingParseModule;
    fileManagement = FileManagement::getInstance();
}


void ClientManagement::pushFile(QString clientIp)
{
    Client *client = new Client;
    QHostAddress clientAddr;
    clientAddr.setAddress(clientIp);
    client->setClientIp(clientAddr);
    client->setWorkDir(fileManagement->getWorkDirectory());
    client->setFileList(fileManagement->getCurrentFileList());
    client->prepareDistribute();
}


void ClientManagement::getLocalHostAddress()
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(hostName);
    foreach (QHostAddress address, info.addresses()) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            localHostAddr = address;
            tempServerIpQueue.enqueue(address);
        }
    }
}
