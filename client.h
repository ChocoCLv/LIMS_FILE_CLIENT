#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QHostAddress>


#include "filesendtask.h"
#include "log.h"

/**
 * @brief The ClientInfo class
 * 记录每个本次实验下载过文件的客户端信息
 * 管理自己的socket
 */

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    void setClientIp(QHostAddress ip);
    void setWorkDir(QString dir);
    QHostAddress getClientHostAddress();

private:
    QString workDir;
    QHostAddress clientIp;
    Log *log;

    QList<QString> currentFileList;
    quint64 taskRecvSize;

signals:
    void startTask(QThread*);
    void taskOver();


public slots:
    void releaseThreadResourse(FileSendTask* task);
    void pushFile(QString,QString);
};

#endif // CLIENTINFO_H
