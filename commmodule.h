#ifndef COMMMODULE_H
#define COMMMODULE_H

#include <QObject>
#include <QHostAddress>
#include <QHostInfo>

class CommModule : public QObject
{
    Q_OBJECT
public:
    ~CommModule();
    static CommModule* getInstance();
    QString getLocalIpAddr();

private:
    static CommModule* commModule;
    explicit CommModule(QObject *parent = 0);
    QString localIp;

signals:

public slots:
};

#endif // COMMMODULE_H
