#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QDirModel>
#include <QDir>
#include <QModelIndex>
#include <QTime>

#include "filemanagement.h"
#include "clientmanagement.h"
#include "settingdialog.h"
#include "qrcodedialog.h"
#include "deviceinfo.h"
#include "log.h"

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
    //QrcodeDialog *dialog;
    SettingDialog *settingDialog;
    ClientManagement *clientManagement;

    QDirModel *fileModel;
    FileManagement *fileManagement;
    void readSettings();
    void writeSettings();
    void initFileTreeView();

    Log* log;

protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void on_actionOptions_triggered();
    void updateFileTreeView();
    void showLog(quint8 logType, QVariant logContent);
    void showLog(QString l);
private slots:

    void on_actionShowLog_changed();
};

#endif // MAINWINDOW_H
