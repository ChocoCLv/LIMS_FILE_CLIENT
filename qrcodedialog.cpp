#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"

QrcodeDialog::QrcodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QrcodeDialog)
{
    ui->setupUi(this);
    //ui->edtDeviceIp->setText(CommModule::getInstance()->getLocalIpAddr());
    ui->edtDeviceId->setText(DeviceInfo::getInstance()->getDeviceId());

    codeText.insert("DEVICE_TYPE",DeviceInfo::getInstance()->getDeviceType());
    codeText.insert("DEVICE_ID",DeviceInfo::getInstance()->getDeviceId());
    //codeText.insert("DEVICE_IP",CommModule::getInstance()->getLocalIpAddr());

    ui->qrcodeWidget->generateString(QString(QJsonDocument(codeText).toJson()));
}

QrcodeDialog::~QrcodeDialog()
{
    delete ui;
}
