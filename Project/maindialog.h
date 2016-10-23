#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

#include "tasklistwidget.h"

class MainDialogHeader : public QFrame
{
    Q_OBJECT

public:
    MainDialogHeader(QWidget *parent = 0);
    ~MainDialogHeader(){}

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

signals:
    void sndMoveDialog(QPoint);
    void sndClose();

private:
    QPoint oldPosition;
    QPushButton *mpCloseButton = 0;
};

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = 0);
    ~MainDialog() {}

private slots:
    void moveDialog (QPoint loMovePoint);
//protected:
//    void mousePressEvent(QMouseEvent * event);
//    void mouseMoveEvent(QMouseEvent * event);

private:
    MainDialogHeader *mpDialogHeader = 0;
    TaskListWidget *mpTaskList = 0;
};

#endif // MAINDIALOG_H
