#include "filerecvtask.h"

FileRecvTask::FileRecvTask()
{
    socket = new QTcpSocket;
    dataType = 0;
    nextBlockSize = 0;
    fileNumRecv = 0;
    totalRecvSize = 0;
    fileManagement = FileManagement::getInstance();
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
        if(!currentFile->open(QFile::WriteOnly)){
            return;
        }
        break;
    case FILE_DATA:
        fileBlock.clear();
        fileBlock = in.device()->read(nextBlockSize-sizeof(dataType));
        rcvSize += fileBlock.size();
        totalRecvSize += fileBlock.size();
        currentFile->write(fileBlock);
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

