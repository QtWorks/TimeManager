#include "maindialog.h"

#include <QFontDatabase>
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile cssfile(":/style/resources/style01.css");
    cssfile.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(cssfile.readAll());
    a.setStyleSheet(StyleSheet);

    QFontDatabase::addApplicationFont(":/fonts/resources/Ubuntu-R.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/CaptureIt.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/alarm clock.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/secrcode.ttf");

    MainDialog *w = new MainDialog;
    w->show();

    return a.exec();
}
