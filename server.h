#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include "filerecvtask.h"
#include "log.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

private:
    FileRecvTask *fileRecvTask;
    Log* log;

protected:
    void incomingConnection(int socketId);

signals:
    void startTask(int socketId,QThread* thread);
    void fileRecvOver(QString fileName);
private slots:
    void releaseRecvThread(FileRecvTask* task);
};

#endif // SERVER_H
