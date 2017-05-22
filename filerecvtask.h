#ifndef FILERECVTASK_H
#define FILERECVTASK_H


#include <QFile>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QObject>
#include <QHostAddress>
#include <QSemaphore>
#include <QDataStream>
#include <QDir>

#include "config.h"
#include "filemanagement.h"
#include "log.h"

/**
 * @brief The FileRecvTask class
 *  每个接收任务只负责接收一个文件 对应一个sendtask
 */

class FileRecvTask:public QObject
{
    Q_OBJECT
public:
    explicit FileRecvTask(QObject *parent = 0);
    QThread * getThread();
    QString getFileName();
    quint64 getFileSize();
    QTcpSocket *socket;
    void setTotalRecvSize(quint64 value);

private:
    const static quint8 FILE_NAME = 1;
    const static quint8 FILE_DATA = 2;
    quint8 dataType;
    quint16 nextBlockSize;
    quint64 fileSize;
    QString fileName;
    quint64 rcvSize;
    quint64 totalRecvSize;
    QThread *thread;
    QFile *recvFile;
    QByteArray fileBlock;
    FileManagement *fileManagement;
    Log* log;

signals:
    void taskOver(FileRecvTask*);

private slots:
    void startRecvTask(int socketId,QThread *t);
    void readSocket();
    void socketStateChange(QAbstractSocket::SocketState state);
};

#endif // FILERECVTASK_H
