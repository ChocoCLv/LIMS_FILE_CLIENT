#include "deviceinfo.h"

DeviceInfo* DeviceInfo::deviceInfo = NULL;

DeviceInfo::DeviceInfo()
{
    deviceType = "电脑";
}

DeviceInfo* DeviceInfo::getInstance()
{
    if(deviceInfo == NULL){
        deviceInfo = new DeviceInfo();
    }
    return deviceInfo;
}

QString DeviceInfo::getDeviceId()
{
    return deviceId;
}

void DeviceInfo::setDeviceId(QString _deviceId)
{
    this->deviceId = _deviceId;
}

QString DeviceInfo::getDeviceType()
{
    return deviceType;
}
