#ifndef CLIENTMANAGEMENT_H
#define CLIENTMANAGEMENT_H

#include <QObject>
#include <QList>
#include <QJsonObject>
#include <QHostAddress>
#include <QQueue>
#include <QHostInfo>
#include <QMap>

#include "client.h"
#include "server.h"
#include "signalingparsemodule.h"
#include "filemanagement.h"

class ClientManagement : public QObject
{
    Q_OBJECT
public:
    explicit ClientManagement(QObject *parent = 0);

private:
    Server *server;
    Client *localClient;

    QString workDir;

    void getLocalHostAddress();

    int fileNum;
    quint64 totalSize;

    SignalingParseModule *signalingParseModule;

    FileManagement *fileManagement;

    QHostAddress localHostAddr;
signals:

public slots:
    void fileRecvOver(QString fileName);
    void oneSendTaskOver();
    void setTaskInfo(int fileNum,quint64 totalSize);
};

#endif // CLIENTMANAGEMENT_H
