#include "maindialog.h"

MainDialog::MainDialog(QWidget *parent) : QDialog (parent)
{
    setWindowTitle("Time Manager");
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    setContentsMargins(0,0,0,0);

    QVBoxLayout *lvGlobalLay = new QVBoxLayout(this);
    lvGlobalLay->setContentsMargins(0,0,0,0);
    lvGlobalLay->setSpacing(0);

    mpDialogHeader = new MainDialogHeader(this);
    connect(mpDialogHeader, SIGNAL(sndClose()), this, SLOT(close()));
    connect(mpDialogHeader, SIGNAL(sndMoveDialog(QPoint)), this, SLOT(moveDialog (QPoint)));
    mpTaskList = new TaskListWidget(this);

    lvGlobalLay->addWidget(mpDialogHeader);
    lvGlobalLay->addWidget(mpTaskList);
    setLayout(lvGlobalLay);
}

void MainDialog::moveDialog (QPoint loMovePoint)
{
    move (pos() + loMovePoint);
}







MainDialogHeader::MainDialogHeader(QWidget *parent) : QFrame(parent)
{
    setContentsMargins(0,0,0,0);

    QHBoxLayout *lpHGlobalLay = new QHBoxLayout (this);
    lpHGlobalLay->setContentsMargins(0,0,0,0);
    lpHGlobalLay->setSpacing(0);

    mpCloseButton = new QPushButton(this);
    mpCloseButton->setObjectName("closeButton");
    connect(mpCloseButton, SIGNAL(clicked(bool)), this, SIGNAL(sndClose()));

    lpHGlobalLay->addStretch(0);
    lpHGlobalLay->addWidget(mpCloseButton);

    setLayout(lpHGlobalLay);
}

void MainDialogHeader::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
        oldPosition= event->pos();
}

void MainDialogHeader::mouseMoveEvent(QMouseEvent * event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint despl = event->pos() - oldPosition;
        emit sndMoveDialog(despl);
    }
}
