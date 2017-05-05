#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DeviceInfo::getInstance()->setDeviceId("KB115-001");
    //dialog = new QrcodeDialog(this);
    settingDialog = new SettingDialog(this);
    server = new Server(this);
    clientManagement = new ClientManagement(this);
    fileManagement = FileManagement::getInstance();

    connect(fileManagement,SIGNAL(workDirUpdated()),this,SLOT(updateFileTreeView()));

    //dialog->show();

    initFileTreeView();
    readSettings();
    updateFileTreeView();
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
