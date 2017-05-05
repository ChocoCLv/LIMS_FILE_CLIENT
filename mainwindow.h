#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QDirModel>
#include <QDir>
#include <QModelIndex>

#include "filemanagement.h"
#include "clientmanagement.h"
#include "settingdialog.h"
#include "qrcodedialog.h"
#include "deviceinfo.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QrcodeDialog *dialog;
    SettingDialog *settingDialog;
    Server *server;
    ClientManagement *clientManagement;

    QDirModel *fileModel;
    FileManagement *fileManagement;
    void readSettings();
    void writeSettings();
    void initFileTreeView();


protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void on_actionOptions_triggered();
    void updateFileTreeView();
};

#endif // MAINWINDOW_H
