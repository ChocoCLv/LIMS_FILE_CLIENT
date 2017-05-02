#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QDebug>

class DeviceInfo
{
public:
    static DeviceInfo* getInstance();

    QString getDeviceId();
    QString getDeviceType();

    void    setDeviceId(QString _deviceId);

private:
    DeviceInfo();
    static DeviceInfo* deviceInfo;

    QString deviceId;
    QString deviceType;
};

#endif // DEVICEINFO_H
