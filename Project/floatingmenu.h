#ifndef FLOATINGMENU_H
#define FLOATINGMENU_H

#include <QDialog>
#include <QPaintEvent>
#include <QSvgRenderer>
#include <QImage>

class FloatingMenu : public QDialog
{
    Q_OBJECT
public:
    FloatingMenu(QWidget *parent = 0);

    void move(const QPoint&);

protected:
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    void showEvent(QShowEvent*)
    {
        mpConfIcon = &moConfIcon;
        mpCopyIcon = &moCopyIcon_H;
        mpDeleteIcon = &moDeleteIcon;
    }

private:
    void initPixmaps();

signals:
    void sndConfigSelected();
    void sndCopySelected();
    void sndDeleteSelected();

private:
    QPixmap moConfIcon, moConfIcon_H, moCopyIcon, moCopyIcon_H, moDeleteIcon, moDeleteIcon_H;
    QPixmap *mpConfIcon, *mpCopyIcon, *mpDeleteIcon;
    int liSelIndex = 1;
};

#endif // FLOATINGMENU_H
