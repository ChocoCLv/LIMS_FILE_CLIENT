#include "signalingparsemodule.h"

SignalingParseModule::SignalingParseModule(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(CLIENT_SIGNALING_PORT_UDP);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    log = Log::getInstance();
}

SignalingParseModule::~SignalingParseModule()
{
    delete udpSocket;
}

void SignalingParseModule::processPendingDatagrams()
{
    QByteArray datagram;
    do{
        QHostAddress clientAddr;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size(),&clientAddr);
        QTextCodec *utf8codec = QTextCodec::codecForName("UTF-8");
        QString utf8str = utf8codec->toUnicode(datagram);
        datagram = utf8str.toStdString().data();
        processSignaling(datagram,clientAddr);
    }while(udpSocket->hasPendingDatagrams());
}

void SignalingParseModule::processSignaling(QByteArray signaling, QHostAddress addr)
{
    QJsonParseError jpe;

    QJsonDocument jd = QJsonDocument::fromJson(signaling,&jpe);
    QJsonObject jo;

    if(jpe.error != QJsonParseError::NoError)
    {
        return;
    }
    jo = jd.object();
    QString signalingType = jo.find("SIGNALING_TYPE").value().toString();
    if(signalingType == "PUSH_FILE_TO_CLIENT"){
        QString clientIp = jo.find("CLIENT_IP").value().toString();
        QString fileName = jo.find("FILE_NAME").value().toString();
        emit log->logStr(QString("get signaling:send %1 to %2").
                         arg(fileName).arg(clientIp));
        emit pushFile(clientIp,fileName);
    }if(signalingType == "ARE_YOU_OK"){
        serverIpAddr = addr;
        int fileNum = jo.find("FILE_NUM").value().toInt();
        quint64 totalSize = jo.find("FILE_TOTAL_SIZE").value().toVariant().toULongLong();
        emit log->logStr(Log::TASK_TOTAL_SIZE,totalSize);
        emit getTaskInfo(fileNum,totalSize);
        replyHello();
    }
}

void SignalingParseModule::replyHello()
{
    QJsonObject jo;
    QJsonDocument jd;
    jo.insert("SIGNALING_TYPE","I_AM_ALIVE");
    jd.setObject(jo);

    udpSocket->writeDatagram(jd.toJson(),serverIpAddr,SERVER_SIGNALING_PORT_UDP);
}

void SignalingParseModule::sendSignaling(QJsonObject s)
{
    QJsonDocument jd;
    jd.setObject(s);
    udpSocket->writeDatagram(jd.toJson(),serverIpAddr,SERVER_SIGNALING_PORT_UDP);
}
