#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QString>
#include <QVariant>

class Log : public QObject
{
    Q_OBJECT
public:
    static Log* getInstance();
    static const quint8 FILE_SIZE_RECV = 1;
    static const quint8 FILE_NAME_RECV = 2;
    static const quint8 RECV_SIZE = 3;
    static const quint8 RECEIVE_FILE_COMPLETE = 4;
    static const quint8 COMMON_LOG = 5;
    static const quint8 FILE_SIZE_SEND = 6;
    static const quint8 FILE_NAME_SEND = 7;
    static const quint8 SEND_SIZE = 8;
    static const quint8 SEND_FILE_COMPLETE = 9;
    static const quint8 SRC_IP = 10;
    static const quint8 DST_IP = 11;
    static const quint8 TASK_TOTAL_SIZE = 12;
    static const quint8 TASK_SIZE_RECV = 13;

private:
    static Log* log;
    explicit Log(QObject *parent = 0);
    ~Log();

signals:
    void logStr(quint8 logType,QVariant logContent);
    void logStr(QString logContent);

public slots:
};

#endif // LOG_H
