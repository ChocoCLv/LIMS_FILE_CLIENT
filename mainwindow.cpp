#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DeviceInfo::getInstance()->setDeviceId("KB115-001");
    dialog = new QrcodeDialog(this);
    dialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
