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


//void MainDialogHeader::initPixmaps()
//{
//    auto loadSVG = [](QString lsSVGFile, QSize &size, QPixmap& loOutputPixmap) -> void
//    {
//        QImage image (size, QImage::Format_ARGB32);
//        image.fill (Qt::transparent);
//        QPainter painter (&image);

//        QSvgRenderer renderer (lsSVGFile);
//        renderer.render (&painter);
//        loOutputPixmap = QPixmap::fromImage(image);
//    };

//    QSize loIconSize (60,30);
//    QSize loIconSize_H (120,30);
//    loadSVG(QString(":/images/resources/bt_config.svg"), loIconSize, moConfIcon);
//    loadSVG(QString(":/images/resources/bt_config_hover.svg"), loIconSize_H, moConfIcon_H);
//    loadSVG(QString(":/images/resources/bt_clipboard.svg"), loIconSize, moCopyIcon);
//    loadSVG(QString(":/images/resources/bt_clipboard_hover.svg"), loIconSize_H, moCopyIcon_H);
//    loadSVG(QString(":/images/resources/bt_delete.svg"), loIconSize, moDeleteIcon);
//    loadSVG(QString(":/images/resources/bt_delete_hover.svg"), loIconSize_H, moDeleteIcon_H);

//    mpConfIcon = &moConfIcon;
//    mpCopyIcon = &moCopyIcon_H;
//    mpDeleteIcon = &moDeleteIcon;
//}

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
