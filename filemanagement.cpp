#include "filemanagement.h"

FileManagement* FileManagement::fileManagement = NULL;

FileManagement::FileManagement(QObject *parent) : QObject(parent)
{

}

void FileManagement::addFilePath(QString filePath)
{
    emit workDirUpdated();
}

FileManagement* FileManagement::getInstance()
{
    if(fileManagement == NULL){
        fileManagement = new FileManagement();
    }
    return fileManagement;
}

void FileManagement::setWorkDirectory(QString dir)
{
    workDir = dir;
    emit workDirUpdated();
}

QString FileManagement::getWorkDirectory()
{
    return workDir ;
}

