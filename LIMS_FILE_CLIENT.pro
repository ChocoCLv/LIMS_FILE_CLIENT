#-------------------------------------------------
#
# Project created by QtCreator 2017-05-02T17:31:29
#
#-------------------------------------------------


QT       += network
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LIMS_CLIENT_DESKTOP
TEMPLATE = app

LIBS += C:\Users\choco\Project\Qt\LIMS_FILE_CLIENT\LIMS_FILE_CLIENT\qrcodelib.lib
LIBS += C:\Users\choco\Project\Qt\LIMS_FILE_CLIENT\LIMS_FILE_CLIENT\qrcodelib.dll


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    qrcodegenerate.cpp \
    qrcodedialog.cpp \
    deviceinfo.cpp \
    client.cpp \
    clientmanagement.cpp \
    filemanagement.cpp \
    filesendtask.cpp \
    settingdialog.cpp \
    signalingparsemodule.cpp \
    filerecvtask.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    qrcodegenerate.h \
    qrcodedialog.h \
    deviceinfo.h \
    client.h \
    clientmanagement.h \
    config.h \
    filemanagement.h \
    filesendtask.h \
    settingdialog.h \
    signalingparsemodule.h \
    filerecvtask.h \
    server.h

FORMS    += mainwindow.ui \
    qrcodegenerate.ui \
    qrcodedialog.ui \
    settingdialog.ui
