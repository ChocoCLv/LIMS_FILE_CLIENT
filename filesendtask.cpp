#include "filesendtask.h"

FileSendTask::FileSendTask(QObject *parent) : QObject(parent)
{
    log = Log::getInstance();
    fileSize = 0;
    fileSizeDistributed = 0;
    sndFile = new QFile;
}

void FileSendTask::setWorkDir(QString dir)
{
    workDir = dir;
}

void FileSendTask::setClientIp(QHostAddress ip)
{
    clientIp = ip;
}

void FileSendTask::setFileName(QString fn)
{
    fileName = fn;
}

void FileSendTask::startTask()
{
    openFileRead(fileName);
}

void FileSendTask::updateSendProgress(qint64 numBytes)
{
    sendFileData();
}

void FileSendTask::sendFileData()
{
    emit log->logStr(QString("send file:%1,file total size:%2,send size:%3").arg(sndFile->fileName()).
                     arg(fileSize).arg(fileSizeDistributed));
    fileBlock = sndFile->read(SEND_BUFF_SIZE);
    if(fileBlock.size()==0){
        emit log->logStr(QString("file:%1 send complete").arg(sndFile->fileName()));

        sndFile->close();
        emit taskOver();
        return;
    }
    sndBlock.clear();
    QDataStream out(&sndBlock,QIODevice::WriteOnly);
    out<<quint16(0)<<FILE_DATA<<fileBlock;
    out.device()->seek(0);
    out<<quint16(sndBlock.size()-sizeof(quint16));

    socket->write(sndBlock);
}

void FileSendTask::connectToClient()
{
    socket = new QTcpSocket();
    connect(socket,SIGNAL(connected()),this,SLOT(startTask()));
    connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateSendProgress(qint64)));
    socket->connectToHost(clientIp,FILE_PORT_TCP);
}

void FileSendTask::openFileRead(QString rFilePath)
{
    QString aFilePath = workDir +  rFilePath;
    sndFile->setFileName(aFilePath);
    QFileInfo fi = QFileInfo(aFilePath);

    if(!sndFile->open(QFile::ReadOnly)){
        emit log->logStr(QString("open %1,error:%2").arg(rFilePath).arg(sndFile->errorString()));
        return;
    }
    fileSizeDistributed = 0;
    fileSize = fi.size();

    sndBlock.clear();
    QDataStream out(&sndBlock,QIODevice::WriteOnly);

    //发送文件与工作目录的相对文件路径
    out<<quint16(0)<<FILE_NAME<<fileSize<<rFilePath;
    out.device()->seek(0);
    out<<quint16(sndBlock.size()-sizeof(quint16));

    socket->write(sndBlock);
    emit log->logStr(QString("open file:%1").arg(sndFile->fileName()));
}

FileSendTask::~FileSendTask()
{
    sndFile->deleteLater();
    socket->deleteLater();
}


