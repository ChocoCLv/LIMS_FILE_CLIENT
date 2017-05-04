#ifndef FILERECVTASK_H
#define FILERECVTASK_H

#include <QRunnable>
#include <QFile>
#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include <QSemaphore>
#include <QDataStream>

#include "config.h"
#include "filemanagement.h"


class Signaling:public QObject
{
    Q_OBJECT

signals:
    oneFileRecvOver(quint64 nextFileSize);
};

class FileRecvTask:public QRunnable
{
public:
    FileRecvTask();
    void readSocket();
    QTcpSocket *socket;

private:
    const static quint8 FILE_NAME = 1;
    const static quint8 FILE_DATA = 2;
    quint8 dataType;
    quint16 nextBlockSize;
    quint64 currentFileSize;
    QString currentFileName;
    quint64 rcvSize;
    QFile *currentFile;
    QByteArray fileBlock;
    FileManagement *fileManagement;

};

#endif // FILERECVTASK_H
