#ifndef TASKFRAME_H
#define TASKFRAME_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QLineEdit>
#include <QMouseEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QRegExpValidator>

#include "floatingmenu.h"



class NewTaskDialog : public QDialog
{
    Q_OBJECT
public:
    NewTaskDialog(QWidget *parent = 0);

    void setInitConfig(QString lsNameTask, int liSeconds)
    {
        int liHours =  (liSeconds / (60 * 60));
        int liMinutes =(liSeconds / 60);
        lpTime->setText(QString ("%1:%2").arg(QString::number(liHours,'0',2)).arg(QString::number(liMinutes,'0',2)));
        lpTaskName->setText(lsNameTask);
    }

protected:
    void showEvent(QShowEvent*) { lpTaskName->selectAll(); }

private slots:
    void dlgAccepted ();

signals:
    void sndNameAndTime(QString, int);

private:
    QLineEdit *lpTaskName;
    QLineEdit *lpTime;
};






class DummyTaskFrame : public QFrame
{
    Q_OBJECT

public:
    DummyTaskFrame(QWidget* parent = 0);

protected slots:
    void setAcumSeconds(int);

protected:
    QLabel *mpTaskLabel = 0, *mpTimeLabel = 0, *mpNewTaskLabel = 0;
    int miAcumSeconds = 0;
    QString msTimeToCopy = "0m";
    QTimer *mpTimer = 0;
    bool mbTaskCreated = false;
};








class TaskFrame : public DummyTaskFrame
{
    Q_OBJECT
public:
    TaskFrame(QWidget *parent = 0);
    TaskFrame(QString loTaskName, int liSecondsSpent, bool lbTimerActive, QWidget* parent = 0);

    bool isTaskActive(){return (mpTimer)?mpTimer->isActive():false;}
    QLabel* getTimeLabel() {return mpTimeLabel;}
    QLabel* getTaskLabel() {return mpTaskLabel;}

    QString getTaskName() {return mpTaskLabel->text();}
    int getSecondsSpent() {return miAcumSeconds;}

    static TaskFrame* createActiveTask (QString loTaskName, int liSecondsSpent, bool lbTimerActive);

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        moMovePoint = event->pos();
        moMovePoint = mapToGlobal(moMovePoint);
        event->ignore();
    }
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

public slots:
    void createActiveTask ();
    void toogleTaskState()
    {
        if(isTaskActive()) stopTask();
        else startTask();
    }
    void stopTask();
    void copyTimeInClipboard();
    void showConfigDialog();

private slots:
    void startTask();
    void incTimeSpend();
    void rcvConfigTask(QString lsTaskText, int liAcumSeconds);

signals:
    void sndNewTaskCreated();
    void sndDeleteThisTask();
    void sndTaskActive();
    void currentTime(int);
    void taskPlaying(TaskFrame*);

private:
    QPoint moMovePoint;
    FloatingMenu *mpFloatingMenu = 0;
};


#endif // TASKFRAME_H
