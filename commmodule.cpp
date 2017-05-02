#include "commmodule.h"

CommModule * CommModule::commModule = NULL;

CommModule::CommModule(QObject *parent) : QObject(parent)
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(hostName);
    foreach (QHostAddress address, info.addresses()) {

        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            localIp = address.toString();
        }
    }
}

CommModule * CommModule::getInstance()
{
    if(commModule == NULL){
        commModule = new CommModule();
    }
    return commModule;
}

QString CommModule::getLocalIpAddr()
{
    return localIp;
}

CommModule::~CommModule()
{

}
