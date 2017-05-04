#include "filesendtask.h"

FileSendTask::FileSendTask() : QRunnable()
{
    socket = new QTcpSocket();
    signaling = new Signaling();
    sem.release(1);

    fileNum = 0;
    fileDistributedNum = 0;
    currentFileSize = 0;
    currentFileSizeDistributed = 0;
    numBytesSend = 0;
}

void FileSendTask::setClientIp(QHostAddress ip)
{
    clientIp = ip;
}

void FileSendTask::setFileList(QList<QString> list)
{
    fileList = list;
    fileNum = list.size();
}

void FileSendTask::run()
{
    sem.release(1);
    foreach (QString filePath, fileList) {
        //一个文件发送完毕
        sendFile(filePath);
        delete currentSendFile;
    }
}

void FileSendTask::sendFile(QString filePath)
{
    openFileRead(filePath);
    //发送时
    while(1){
        currentFileSizeDistributed += numBytesSend;
        if(currentFileSizeDistributed >= currentFileSize){
            return;
        }
        fileBlock = currentSendFile->read(1024);
        QDataStream out(&sndBlock,QIODevice::WriteOnly);
        out<<FILE_DATA<<fileBlock.size()<<fileBlock;

        //等待成功发送的信号
        sem.acquire(1);
        socket->write(sndBlock);
    }
}

void FileSendTask::updateSendProgress(quint64 numBytes)
{
    numBytesSend = numBytes;
    sem.release(1);
}

void FileSendTask::connectToClient()
{
    socket->connectToHost(clientIp,FILE_PORT_TCP);
}

void FileSendTask::openFileRead(QString filePath)
{
    currentSendFile = new QFile(filePath);
    QFileInfo fi = QFileInfo(filePath);
    quint64 nextFileSize = fi.size();
    emit signaling->oneFileSendOver(nextFileSize);
    if(!currentSendFile->open(QFile::ReadOnly)){
        qDebug()<<"open file error-read"<<endl;
        return;
    }
    currentFileSizeDistributed = 0;
    currentFileSize = fi.size();

    sndBlock.clear();
    QDataStream out(&sndBlock,QIODevice::WriteOnly);

    //此处应该发送文件与工作目录的相对路径
    out<<FILE_NAME<<currentFileSize<<fi.fileName();

    sem.acquire(1);
    socket->write(sndBlock);
}

