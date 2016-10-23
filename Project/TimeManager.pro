#-------------------------------------------------
#
# Project created by QtCreator 2016-10-12T18:47:26
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeManager
TEMPLATE = app


SOURCES += main.cpp\
    tasklistwidget.cpp \
    taskframe.cpp \
    maindialog.cpp \
    floatingmenu.cpp

HEADERS  += \
    tasklistwidget.h \
    taskframe.h \
    maindialog.h \
    floatingmenu.h

RESOURCES += \
    resources.qrc

win32:RC_ICONS += appicon.ico