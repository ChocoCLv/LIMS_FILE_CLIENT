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
    showLog(Log::COMMON_LOG,l);
}

void MainWindow::showLog(quint8 logType, QVariant logContent)
{
    switch(logType)
    {
    case Log::RECEIVE_FILE_COMPLETE:
        ui->edtCurrentDownloadFileName->setText(logContent.toString());
        break;
    case Log::RECV_SIZE:
        ui->progressBarCurrentFile->setValue(logContent.toInt());
        break;
    case Log::FILE_NAME:
        ui->edtCurrentDownloadFileName->setText(logContent.toString());
        break;
    case Log::FILE_SIZE:
        ui->progressBarCurrentFile->setMaximum(logContent.toInt());
        break;
    case Log::COMMON_LOG:
        ui->edtLog->append(logContent.toString());
        break;
    case Log::TOTAL_SIZE:
        ui->progressBarTotal->setMaximum(logContent.toInt());
        break;
    case Log::TOTAL_SIZE_RECV:
        ui->progressBarTotal->setValue(logContent.toInt());
        break;
    default:
        ui->edtLog->append(logContent.toString());
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"close event";
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
    QSettings settings("UESTC","LIMS FILE CLIENT");
    settings.setValue("geometry",this->saveGeometry());
    settings.setValue("work directory",fileManagement->getWorkDirectory());
}

void MainWindow::readSettings()
{
    QSettings settings("UESTC","LIMS FILE CLIENT");
    restoreGeometry(settings.value("geometry").toByteArray());
    fileManagement->setWorkDirectory(settings.value("work directory").toString());
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
