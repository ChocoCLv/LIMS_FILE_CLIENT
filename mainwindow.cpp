#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    log = Log::getInstance();
    DeviceInfo::getInstance()->setDeviceId("KB115-001");
    //dialog = new QrcodeDialog(this);
    settingDialog = new SettingDialog(this);

    fileManagement = FileManagement::getInstance();

    connect(fileManagement,SIGNAL(workDirUpdated()),this,SLOT(updateFileTreeView()));
    connect(log,SIGNAL(logStr(quint8,QVariant)),this,SLOT(showLog(quint8,QVariant)));
    connect(log,SIGNAL(logStr(QString)),this,SLOT(showLog(QString)));

    //dialog->show();

    initFileTreeView();
    readSettings();
    updateFileTreeView();
    clientManagement = new ClientManagement(this);
}

void MainWindow::showLog(QString l)
{
    QTime time = QTime::currentTime();
    QString textLog = time.toString(Qt::TextDate)+"  "+l;
    ui->edtLog->append(textLog);
    log->writeLog(textLog);
}

void MainWindow::showLog(quint8 logType, QVariant logContent)
{
    switch(logType)
    {
    case Log::RECEIVE_FILE_COMPLETE:
        ui->edtFileNameRecv->setText(logContent.toString());
        break;
    case Log::RECV_SIZE:
        ui->pbRecv->setValue(logContent.toInt());
        break;
    case Log::FILE_NAME_RECV:
        ui->edtFileNameRecv->setText(logContent.toString());
        break;
    case Log::FILE_SIZE_RECV:
        ui->pbRecv->setMaximum(logContent.toInt());
        break;
    case Log::COMMON_LOG:
        showLog(logContent.toString());
        break;
    case Log::FILE_NAME_SEND:
        ui->edtFileNameSend->setText(logContent.toString());
        break;
    case Log::FILE_SIZE_SEND:
        ui->pbSend->setMaximum(logContent.toInt());
        break;
    case Log::SEND_SIZE:
        ui->pbSend->setValue(logContent.toInt());
        break;
    case Log::DST_IP:
        ui->edtDstIp->setText(logContent.toString());
        break;
    case Log::SRC_IP:
        ui->edtSrcIp->setText(logContent.toString());
        break;
    case Log::TASK_TOTAL_SIZE:
        //ui->pbRecvTotal->setMaximum(logContent.toInt());
        break;
    case Log::TASK_SIZE_RECV:
        //ui->pbRecvTotal->setValue(logContent.toInt());
        break;
    default:
        showLog(logContent.toString());
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::initFileTreeView()
{
    fileModel = new QDirModel;
    fileModel->setReadOnly(false);
    fileModel->setSorting(QDir::DirsFirst|QDir::IgnoreCase|QDir::Name);
    ui->fileTreeView->setModel(fileModel);
    ui->fileTreeView->header()->setStretchLastSection(true);
    ui->fileTreeView->header()->setSortIndicator(0,Qt::AscendingOrder);
    ui->fileTreeView->header()->setSortIndicatorShown(true);

    ui->edtWorkDir->setText(fileManagement->getWorkDirectory());
}

void MainWindow::writeSettings()
{
     QSettings *settings = new QSettings("config.ini",QSettings::IniFormat);
    settings->setValue("geometry",this->saveGeometry());
    settings->setValue("work directory",fileManagement->getWorkDirectory());
}

void MainWindow::readSettings()
{
    QSettings *settings = new QSettings("config.ini",QSettings::IniFormat);
    restoreGeometry(settings->value("geometry").toByteArray());
    fileManagement->setWorkDirectory(settings->value("work directory").toString());
}

void MainWindow::updateFileTreeView()
{
    QModelIndex index = fileModel->index(fileManagement->getWorkDirectory());
    ui->fileTreeView->expand(index);
    ui->fileTreeView->scrollTo(index);
    ui->fileTreeView->resizeColumnToContents(0);

    ui->edtWorkDir->setText(fileManagement->getWorkDirectory());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOptions_triggered()
{
    settingDialog->show();
}

void MainWindow::on_actionShowLog_changed()
{
    if(ui->actionShowLog->isChecked()){
        ui->edtLog->show();
    }else{
        ui->edtLog->hide();
    }
}
