#include "taskframe.h"

#include <QStyle>
#include <QMenu>
#include <QClipboard>
#include <QApplication>


inline void updateWidgetStyle (QWidget* lWidget)
{
   lWidget->style ()->unpolish (lWidget);
   lWidget->style ()->polish (lWidget);
   lWidget->update ();
}



TaskFrame::TaskFrame(QWidget *parent):DummyTaskFrame (parent)
{
    mpTaskLabel->setVisible(false);
    mpNewTaskLabel->setVisible(true);
    mpTimeLabel->setVisible(false);

    setObjectName("nonActive");

    mpTimer = new QTimer(this);
    mpTimer->setInterval(1000);
    connect(mpTimer, SIGNAL(timeout()), this, SLOT(incTimeSpend()));

    mpFloatingMenu = new FloatingMenu(this);
    connect (mpFloatingMenu, SIGNAL(sndConfigSelected()), this, SLOT(showConfigDialog()));
    connect (mpFloatingMenu, SIGNAL(sndCopySelected()), this, SLOT(copyTimeInClipboard()));
    connect (mpFloatingMenu, SIGNAL(sndDeleteSelected()), this, SIGNAL(sndDeleteThisTask()));

    setMouseTracking(true);
}

TaskFrame::TaskFrame (QString loTaskName, int liSecondsSpent, bool lbTimerActive, QWidget* parent) : TaskFrame(parent)
{
    rcvConfigTask(loTaskName,liSecondsSpent);

    mpNewTaskLabel->setVisible(false);
    mpTaskLabel->setVisible(true);
    mpTimeLabel->setVisible(true);

    if(lbTimerActive) startTask();
    else setObjectName("paused");

    mbTaskCreated = true;
}

void TaskFrame::createActiveTask ()
{
    if(mbTaskCreated) return;

    setObjectName("paused");

    mpNewTaskLabel->setVisible(false);
    mpTaskLabel->setVisible(true);
    mpTimeLabel->setVisible(true);

    updateWidgetStyle(this);

    emit sndNewTaskCreated();
    showConfigDialog();

    mbTaskCreated = true;
}

void TaskFrame::startTask()
{
    setObjectName("playing");
    updateWidgetStyle(this);

    incTimeSpend();

    mpTimer->start();

    emit taskPlaying(this);

    emit sndTaskActive();
}

void TaskFrame::stopTask()
{
    mpTimer->stop();

    setObjectName("paused");
    updateWidgetStyle(this);

    emit taskPlaying(0);
}

void TaskFrame::incTimeSpend()
{
    setAcumSeconds(miAcumSeconds);
    miAcumSeconds++;
    emit currentTime(miAcumSeconds);
}

void TaskFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(mpFloatingMenu->isVisible()) mpFloatingMenu->close();
    event->ignore();
}
void TaskFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if ((mapToGlobal(event->pos()) - moMovePoint).manhattanLength() > 2) return;

    if(event->button() == Qt::LeftButton)
    {
        if (mbTaskCreated)
        {
            if(mpTimer->isActive()) stopTask();
            else startTask();
        }
        else
            createActiveTask();
    }
    else if(event->button() == Qt::RightButton)
    {
        mpFloatingMenu->move(mapToGlobal(event->pos()));
        mpFloatingMenu->show ();
    }
}

void TaskFrame::copyTimeInClipboard()
{
    QClipboard *lpClipboard = QApplication::clipboard();
    lpClipboard->setText (msTimeToCopy);
}

void TaskFrame::showConfigDialog()
{
    NewTaskDialog *lpDlg = new NewTaskDialog(this);
    lpDlg->setInitConfig(mpTaskLabel->text(),miAcumSeconds);
    connect(lpDlg, SIGNAL(sndNameAndTime(QString,int)), this, SLOT(rcvConfigTask(QString,int)));

    int liRet = lpDlg->exec();
    if(liRet == QDialog::Rejected && !mbTaskCreated)
        emit sndDeleteThisTask();
}

void TaskFrame::rcvConfigTask(QString lsTaskText,int liAcumSeconds)
{
    mpTaskLabel->setText(lsTaskText);
    miAcumSeconds = liAcumSeconds;
    setAcumSeconds(miAcumSeconds);
}



















NewTaskDialog::NewTaskDialog(QWidget *parent) : QDialog(parent)
{
    setWindowModality(Qt::WindowModal);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *lpVLayout = new QVBoxLayout;
    lpVLayout->setContentsMargins(10,10,10,10);
    lpVLayout->setSpacing(0);

    QLabel *lpTaskNameLabel = new QLabel(tr("Task name"));
    lpTaskNameLabel->setContentsMargins(0,0,0,0);
    lpTaskNameLabel->setObjectName("cnfDlgLabel");
    QLabel *lpTimeeLab = new QLabel (tr("Time already spent"));
    lpTimeeLab->setContentsMargins(0,0,0,0);
    lpTimeeLab->setObjectName("cnfDlgLabel");

    lpTaskName = new QLineEdit(this);
    lpTaskName->setContentsMargins(0,0,0,0);
    lpTaskName->setObjectName("lineEditName");
    lpTime = new QLineEdit (QString ("00h 00m"), this);
    lpTime->setContentsMargins(0,0,0,0);
    QString inputmask = "99\\h 99m";
    lpTime->setInputMask (inputmask);
    lpTime->setObjectName("lineEditTime");

    QPushButton *lpAcceptButton  = new QPushButton (tr("Accept"), this);
    QPushButton *lpCancelButton  = new QPushButton (tr("Cancel"), this);
    connect(lpAcceptButton, SIGNAL(clicked(bool)), this, SLOT(accept ()));
    connect(lpCancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(dlgAccepted ()));
    QHBoxLayout *lpHLayout = new QHBoxLayout;
    lpHLayout->setContentsMargins(0,0,0,0);
    lpHLayout->setSpacing(0);
    lpHLayout->addWidget(lpAcceptButton);
    lpHLayout->addStretch(0);
    lpHLayout->addWidget(lpCancelButton);

    lpVLayout->addWidget(lpTaskNameLabel);
    lpVLayout->addWidget(lpTaskName);
    lpVLayout->addWidget(lpTimeeLab);
    lpVLayout->addWidget(lpTime);
    lpVLayout->addLayout(lpHLayout);

    setLayout(lpVLayout);
}

void NewTaskDialog::dlgAccepted ()
{
    QString lsHours = lpTime->text().split("h ").at(0);
    QString lsMinutes = lpTime->text().split("h ").at(1);
    lsMinutes.chop(1);
    emit sndNameAndTime(lpTaskName->text(), lsHours.toInt() * 60 * 60 + lsMinutes.toInt() * 60);
}





















DummyTaskFrame::DummyTaskFrame(QWidget *parent):QFrame (parent)
{
    setContentsMargins(0,0,0,0);

    QHBoxLayout *lpHBoxLayout = new QHBoxLayout (this);
    lpHBoxLayout->setContentsMargins(0,0,0,0);
    lpHBoxLayout->setSpacing(0);

    mpTaskLabel = new QLabel("Task", this);
    mpTaskLabel->setContentsMargins(0,0,0,0);
    mpTaskLabel->setObjectName("taskLabel");

    mpNewTaskLabel = new QLabel("CREATE NEW TASK", this);
    mpNewTaskLabel->setContentsMargins(0,0,0,0);
    mpNewTaskLabel->setObjectName("newTaskLabel");
    mpNewTaskLabel->setVisible(false);

    mpTimeLabel = new QLabel (this);
    mpTimeLabel->setText("0m ");
    mpTimeLabel->setObjectName("timeLabel");

    lpHBoxLayout->addSpacerItem(new QSpacerItem(10,1));
    lpHBoxLayout->addWidget(mpTaskLabel);
    lpHBoxLayout->addStretch(0);
    lpHBoxLayout->addWidget(mpNewTaskLabel);
    lpHBoxLayout->addStretch(0);
    lpHBoxLayout->addWidget(mpTimeLabel);

    setLayout(lpHBoxLayout);

    setObjectName("playing");
}

void DummyTaskFrame::setAcumSeconds(int liAcumSeconds)
{
    QString lsTimeString;

    int liWeeks =   liAcumSeconds / (60 * 60 * 8 * 5);
    int liDays =   (liAcumSeconds / (60 * 60 * 8)) % 5;
    int liHours =  (liAcumSeconds / (60 * 60)) % 8;
    int liMinutes =(liAcumSeconds / 60) % 60;
    int liSeconds = liAcumSeconds % 60;

    if(liWeeks) lsTimeString += QString("%1w ").arg(liWeeks);
    if(liDays) lsTimeString += QString("%1d ").arg(liDays);
    if(liHours) lsTimeString += QString("%1h ").arg(liHours);
    lsTimeString += QString("%1m").arg(liMinutes);
    msTimeToCopy = lsTimeString;
    lsTimeString += (liSeconds % 2)?" ":"'";

    mpTimeLabel->setText(lsTimeString);
}
