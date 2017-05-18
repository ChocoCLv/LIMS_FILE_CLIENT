#ifndef SIGNALINGPARSEMODULE_H
#define SIGNALINGPARSEMODULE_H

#include <QObject>
#include <QUdpSocket>
#include <QJsonObject>
#include <QJsonParseError>
#include <QHostAddress>
#include <QTextCodec>
#include <QDataStream>

#include "config.h"
#include "log.h"

/**
 * @brief The SignalingParseModule class
 * 负责处理请求
 */

class SignalingParseModule : public QObject
{
    Q_OBJECT
public:
    explicit SignalingParseModule(QObject *parent = 0);
    void sendSignaling(QJsonObject s);
    ~SignalingParseModule();

private:
    QUdpSocket *udpSocket;
    QHostAddress serverIpAddr;
    Log *log;

    void processSignaling(QByteArray signaling,QHostAddress addr);

    void detectClient();
    void replyHello();

signals:
    void pushFile(QString clientIp,QString fileName);
    void getTaskInfo(int fileNum,quint64 totalSize);

public slots:
    void processPendingDatagrams();
};

#endif // SIGNALINGPARSEMODULE_H
