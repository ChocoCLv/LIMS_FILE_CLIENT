#include "filerecvtask.h"

FileRecvTask::FileRecvTask(QObject *parent):QObject(parent)
{
    dataType = 0;
    nextBlockSize = 0;
    fileManagement = FileManagement::getInstance();
    log = Log::getInstance();
    emit log->logStr(fileManagement->getWorkDirectory());
}

void FileRecvTask::startRecvTask(int socketId, QThread *t)
{
    thread = t;
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketId);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readSocket()));
}

QThread * FileRecvTask::getThread()
{
    return thread;
}

QString FileRecvTask::getFileName()
{
    return fileName;
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

    emit log->logStr("get data");

    QString filePath;
    QDir fileDir;
    QFileInfo fi;
    switch(dataType)
    {
    case FILE_NAME:
        in>>fileSize>>fileName;
        filePath = fileManagement->getWorkDirectory()+fileName;
        fi.setFile(filePath);
        fileDir = QDir(fi.absolutePath());
        if(!fileDir.exists()){
            fileDir.mkpath(fileDir.absolutePath());
        }

        rcvSize = 0;
        recvFile = new QFile(filePath);
        emit log->logStr(Log::COMMON_LOG,QString("start recv file:%1,file size:%2").
                         arg(fileName).arg(fileSize));
        if(!recvFile->open(QFile::WriteOnly)){
            emit log->logStr(Log::COMMON_LOG,QString("cann't open file:%1,error:%2").
                             arg(filePath).arg(recvFile->errorString()));
            return;
        }
        fileManagement->addFilePath(fileName);
        emit log->logStr(Log::FILE_NAME,recvFile->fileName());
        emit log->logStr(Log::FILE_SIZE,fileSize);
        break;
    case FILE_DATA:
        fileBlock.clear();
        quint32 size;
        in>>size;
        fileBlock = in.device()->read(size);
        rcvSize += fileBlock.size();
        recvFile->write(fileBlock);
        emit log->logStr(Log::RECV_SIZE,rcvSize);
        emit log->logStr(Log::COMMON_LOG,QString("recv file:%1,has recved %2 bytes").arg(recvFile->fileName()).arg(rcvSize));
        if(rcvSize >= fileSize){
            emit log->logStr(Log::COMMON_LOG,QString("file %1 recv complete").arg(recvFile->fileName()));
            recvFile->close();
            emit taskOver(this);
        }
        break;
    default:
        break;
    }
    nextBlockSize = 0;
    if(socket->bytesAvailable()>0){
        readSocket();
    }
}

