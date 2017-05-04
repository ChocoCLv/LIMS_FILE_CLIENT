#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "filerecvtask.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

private:
    FileRecvTask *fileRecvTask;

protected:
    void incomingConnection(int socketId);

signals:

public slots:
    void socketReadyRead();
};

#endif // SERVER_H
