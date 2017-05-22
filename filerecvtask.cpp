#include "filerecvtask.h"

FileRecvTask::FileRecvTask(QObject *parent):QObject(parent)
{
    dataType = 0;
    nextBlockSize = 0;
    fileManagement = FileManagement::getInstance();
    log = Log::getInstance();
}

void FileRecvTask::startRecvTask(int socketId, QThread *t)
{
    thread = t;
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketId);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readSocket()));
    //connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
      //      this,SLOT(socketStateChange(QAbstractSocket::SocketState)));
    emit log->logStr(Log::SRC_IP,socket->peerAddress().toString());
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
            emit taskOver(this);
            return;
        }
        fileManagement->addFilePath(fileName);
        emit log->logStr(Log::FILE_NAME_RECV,recvFile->fileName());
        emit log->logStr(Log::FILE_SIZE_RECV,fileSize);
        break;
    case FILE_DATA:
        fileBlock.clear();
        quint32 size;
        in>>size;
        fileBlock = in.device()->read(size);
        rcvSize += fileBlock.size();
        if(-1 == recvFile->write(fileBlock)){
            emit log->logStr(Log::COMMON_LOG,recvFile->errorString());
        }

        emit log->logStr(Log::RECV_SIZE,rcvSize);
        if(rcvSize >= fileSize){
            emit log->logStr(Log::COMMON_LOG,QString("file %1 recv complete").arg(recvFile->fileName()));
            recvFile->close();
            socket->disconnectFromHost();
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

void FileRecvTask::socketStateChange(QAbstractSocket::SocketState state)
{
    switch(state)
    {
    case QAbstractSocket::ClosingState:
        emit taskOver(this);
        break;
    case QAbstractSocket::UnconnectedState:
        emit taskOver(this);
        break;
    }
}
