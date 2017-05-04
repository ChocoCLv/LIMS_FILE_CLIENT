#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingdialog.h"
#include "qrcodedialog.h"
#include "deviceinfo.h"

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

public slots:
    void on_actionOptions_triggered();
};

#endif // MAINWINDOW_H
