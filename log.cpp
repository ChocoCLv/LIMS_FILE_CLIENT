#include "log.h"

Log* Log::log = NULL;

Log::Log(QObject *parent) : QObject(parent)
{
    file = new QFile("log.txt");
    if(!file->open(QIODevice::WriteOnly|QIODevice::Text )){
        emit logStr(file->errorString());
        file = NULL;
    }
}

Log* Log::getInstance()
{
    if(log == NULL){
        log = new Log;
    }
    return log;
}

Log::~Log()
{
    delete log;
}

void Log::writeLog(QString l)
{
    QTextStream in(file);
    in<<l<<endl;
}
