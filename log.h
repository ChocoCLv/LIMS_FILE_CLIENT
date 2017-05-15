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
    static const quint8 FILE_SIZE = 1;
    static const quint8 FILE_NAME = 2;
    static const quint8 RECV_SIZE = 3;
    static const quint8 RECEIVE_FILE_COMPLETE = 4;
    static const quint8 COMMON_LOG = 5;
    static const quint8 TOTAL_SIZE = 6;
    static const quint8 TOTAL_SIZE_RECV = 7;
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
