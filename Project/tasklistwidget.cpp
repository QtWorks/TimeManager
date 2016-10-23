#include "tasklistwidget.h"

#include <QSizeGrip>

TaskListWidget::TaskListWidget(QWidget *parent): QScrollArea (parent)
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);

    setContentsMargins(0,0,0,0);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    QFrame *lpGlobalFrame = new QFrame (this);
    lpGlobalFrame->setContentsMargins(0,0,0,0);
    lpGlobalFrame->setObjectName("tasksListFrame");

    QSizeGrip *lpSizeGrip = new QSizeGrip (this);

    mpGlobalLayout = new QVBoxLayout(this);
    mpGlobalLayout->setContentsMargins(0,0,0,0);
    mpGlobalLayout->setSpacing(0);
    mpGlobalLayout->addStretch(0);
    mpGlobalLayout->addWidget(lpSizeGrip);

    lpGlobalFrame->setLayout(mpGlobalLayout);

    setWidget(lpGlobalFrame);

    mpNewTask = new QShortcut(QKeySequence("Ctrl+N"),this);
    mpStartTask = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(mpStartTask, SIGNAL(activated()), this, SLOT(startLastTask()));
    mpStartTask = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(mpStartTask, SIGNAL(activated()), this, SLOT(copyTimeActiveTask()));

    addNewTask();
}

void TaskListWidget::addNewTask()
{
    //it should always be an empty FrameTask in order to be able to add new tasks
    TaskFrame *newFrame = new TaskFrame(this);
    connect(newFrame, SIGNAL(sndNewTaskCreated()), this, SLOT(addNewTask()));
    connect(newFrame, SIGNAL(sndDeleteThisTask()), this, SLOT(deleteTask()));
    connect(newFrame, SIGNAL(sndTaskActive()), this, SLOT(sortActiveTask()));
    connect(newFrame, SIGNAL(taskPlaying(TaskFrame*)), this, SLOT(rcvTaskPlaying(TaskFrame*)));

    connect(mpNewTask, SIGNAL(activated()), newFrame, SLOT(createActiveTask()));

    mpGlobalLayout->insertWidget(0, newFrame);

    sortActiveTask();
}

void TaskListWidget::rcvTaskPlaying(TaskFrame* lpActiveTask)
{
    if(mpActiveTask && lpActiveTask && mpActiveTask != lpActiveTask)
        mpActiveTask->stopTask();
    mpActiveTask = lpActiveTask;
}

void TaskListWidget::sortActiveTask()
{
    for(int i = 1; i < mpGlobalLayout->count() - 2; i++)
    {
        TaskFrame* tempTask = (TaskFrame*)mpGlobalLayout->itemAt(i)->widget();
        if (tempTask && tempTask == mpActiveTask)
        {
            mpGlobalLayout->insertWidget(1, mpGlobalLayout->takeAt(i)->widget());
            return;
        }
    }
}

void TaskListWidget::deleteTask()
{
    TaskFrame* lpTask = qobject_cast<TaskFrame*>(sender());

    for(int i = 1; i < mpGlobalLayout->count() - 2; i++)
    {
        if(lpTask == (TaskFrame*)mpGlobalLayout->itemAt(i)->widget())
        {
            mpGlobalLayout->takeAt(i)->widget();
            delete lpTask;
            return;
        }
    }
}

void TaskListWidget::startLastTask()
{
    if(mpGlobalLayout->count() > 3)
    {
        TaskFrame* tempTask = (TaskFrame*)mpGlobalLayout->itemAt(1)->widget();
        if(tempTask) tempTask->toogleTaskState();
    }
}

void TaskListWidget::copyTimeActiveTask()
{
    if(mpActiveTask) mpActiveTask->copyTimeInClipboard();
}
