#include "filerecvtask.h"

FileRecvTask::FileRecvTask()
{
    socket = new QTcpSocket;
    dataType = 0;
    nextBlockSize = 0;
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
    switch(dataType)
    {
    case FILE_NAME:
        in>>currentFileSize>>currentFileName;
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
        currentFile->write(fileBlock);
        if(rcvSize >= currentFileSize){
            currentFile->close();
        }

        break;
    default:
        break;
    }
    nextBlockSize = 0;
}

