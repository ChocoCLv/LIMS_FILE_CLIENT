#include "filerecvtask.h"

FileRecvTask::FileRecvTask(QObject *parent):QObject(parent)
{
    socket = new QTcpSocket;
    dataType = 0;
    nextBlockSize = 0;
    fileNumRecv = 0;
    totalRecvSize = 0;
    fileManagement = FileManagement::getInstance();
    connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(socketStateChange(QAbstractSocket::SocketState)));
}

void FileRecvTask::socketStateChange(QAbstractSocket::SocketState state)
{
    qDebug()<<"socket state:"<<state;
}

void FileRecvTask::readSocket()
{
    QDataStream in(socket);
    if(nextBlockSize == 0){
        if(socket->bytesAvailable()<sizeof(dataType)){
            return;
        }
        in>>nextBlockSize;
    }
    if(socket->bytesAvailable()<nextBlockSize){
        return;
    }
    in>>dataType;
    qDebug()<<QString("get data");

    QString filePath;
    QDir fileDir;
    QDir fileParentDir;
    switch(dataType)
    {
    case FILE_NAME:
        in>>currentFileSize>>currentFileName;
        filePath = fileManagement->getWorkDirectory()+currentFileName;
        fileDir.setCurrent(filePath);
        fileParentDir.setCurrent(fileDir.absolutePath());
        if(!fileParentDir.exists()){
            fileParentDir.mkpath(fileDir.absolutePath());
        }

        rcvSize = 0;
        currentFile = new QFile(fileManagement->getWorkDirectory()+currentFileName);
        qDebug()<<QString("start recv file:%1").arg(currentFileName);
        if(!currentFile->open(QFile::WriteOnly)){
            qDebug()<<QString("cann't open file:%1").arg(currentFileName);
            return;
        }
        break;
    case FILE_DATA:
        fileBlock.clear();
        quint32 size;
        in>>size;
        fileBlock = in.device()->read(size);
        rcvSize += fileBlock.size();
        totalRecvSize += fileBlock.size();
        currentFile->write(fileBlock);
        qDebug()<<QString("recv file:%1,has recved %2 bytes").arg(currentFile->fileName()).arg(rcvSize);
        if(rcvSize >= currentFileSize){
            currentFile->close();
            fileNumRecv++;
        }

        break;
    case TASK_INFO:
        in>>fileNumTotal>>totalSize;
        break;
    default:
        break;
    }
    nextBlockSize = 0;
}

