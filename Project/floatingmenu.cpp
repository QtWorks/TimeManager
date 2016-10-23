#include "floatingmenu.h"

#include <QPainter>
#include <QVBoxLayout>

FloatingMenu::FloatingMenu(QWidget *parent) : QDialog(parent)
{
    setContentsMargins(0,0,0,0);
    setWindowModality (Qt::NonModal);
    setWindowFlags (Qt::Tool | Qt::FramelessWindowHint);
    setAttribute (Qt::WA_TranslucentBackground);
    setAttribute (Qt::WA_ShowWithoutActivating);

    setMouseTracking(true);

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* vlayglobal = new QVBoxLayout (this);
    vlayglobal->setContentsMargins (0, 0, 0, 0);
    vlayglobal->setSpacing(0);
    vlayglobal->addStretch(0);

    setLayout (vlayglobal);

    initPixmaps();
}

void FloatingMenu::initPixmaps()
{
    auto loadSVG = [](QString lsSVGFile, QSize &size, QPixmap& loOutputPixmap) -> void
    {
        QImage image (size, QImage::Format_ARGB32);
        image.fill (Qt::transparent);
        QPainter painter (&image);

        QSvgRenderer renderer (lsSVGFile);
        renderer.render (&painter);
        loOutputPixmap = QPixmap::fromImage(image);
    };

    QSize loIconSize (60,30);
    QSize loIconSize_H (120,30);
    loadSVG(QString(":/images/resources/bt_config.svg"), loIconSize, moConfIcon);
    loadSVG(QString(":/images/resources/bt_config_hover.svg"), loIconSize_H, moConfIcon_H);
    loadSVG(QString(":/images/resources/bt_clipboard.svg"), loIconSize, moCopyIcon);
    loadSVG(QString(":/images/resources/bt_clipboard_hover.svg"), loIconSize_H, moCopyIcon_H);
    loadSVG(QString(":/images/resources/bt_delete.svg"), loIconSize, moDeleteIcon);
    loadSVG(QString(":/images/resources/bt_delete_hover.svg"), loIconSize_H, moDeleteIcon_H);

    mpConfIcon = &moConfIcon;
    mpCopyIcon = &moCopyIcon_H;
    mpDeleteIcon = &moDeleteIcon;
}

void FloatingMenu::mouseMoveEvent(QMouseEvent* event)
{
    mpConfIcon = &moConfIcon;
    mpCopyIcon = &moCopyIcon;
    mpDeleteIcon = &moDeleteIcon;
    liSelIndex = 1;

    if(event->pos().y() < 30 && event->pos().x() < 150)
    {
        mpConfIcon = &moConfIcon_H;
        liSelIndex = 0;
    }
    else if(event->pos().y() < 60 && event->pos().x() < 150)
    {
        mpCopyIcon = &moCopyIcon_H;
        liSelIndex = 1;
    }
    else if(event->pos().y() < 90 && event->pos().x() < 150)
    {
        mpDeleteIcon = &moDeleteIcon_H;
        liSelIndex = 2;
    }
    repaint();

    event->accept();
}


void FloatingMenu::mouseReleaseEvent(QMouseEvent*)
{
    if(liSelIndex == 0)
    {
        close();
        emit sndConfigSelected();
    }
    else if(liSelIndex == 1)
    {
        emit sndCopySelected();
        close();
    }
    else if(liSelIndex == 2) emit sndDeleteSelected();
}

void FloatingMenu::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(QColor("#FBFBFB"));

    painter.drawPixmap(0,0,*mpConfIcon);
    painter.drawPixmap(0,30,*mpCopyIcon);
    painter.drawPixmap(0,60,*mpDeleteIcon);

    painter.end();
}

void FloatingMenu::move(const QPoint& loShowPoint)
{
    QPoint loFinalPoint = loShowPoint - QPoint(width()/4, height()/2);
    QDialog::move(loFinalPoint);
}
