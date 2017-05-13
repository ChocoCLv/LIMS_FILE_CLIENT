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

class FileRecvTask:public QObject
{
    Q_OBJECT
public:
    explicit FileRecvTask(QObject *parent = 0);
    void readSocket();
    QTcpSocket *socket;

private:
    const static quint8 FILE_NAME = 1;
    const static quint8 FILE_DATA = 2;
    const static quint8 TASK_INFO = 3;
    quint8 dataType;
    quint16 nextBlockSize;
    quint16 fileNumTotal;
    quint16 fileNumRecv;
    quint64 totalSize;
    quint64 currentFileSize;
    QString currentFileName;
    quint64 rcvSize;
    quint64 totalRecvSize;
    QFile *currentFile;
    QByteArray fileBlock;
    FileManagement *fileManagement;

public slots:
    void socketStateChange(QAbstractSocket::SocketState state);
};

#endif // FILERECVTASK_H
