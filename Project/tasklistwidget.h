#ifndef TASKLISTWIDGET_H
#define TASKLISTWIDGET_H

#include <QScrollArea>
#include <QFrame>
#include <QVBoxLayout>
#include <QShortcut>

#include "taskframe.h"

class TaskListWidget : public QScrollArea
{
    Q_OBJECT

public:
    TaskListWidget(QWidget *parent = 0);
    ~TaskListWidget();

private:
    void readTasksData();
    void writeTasksData();

private slots:
    void addNewTask();
    void deleteTask();
    void sortActiveTask();
    void startLastTask();
    void rcvTaskPlaying(TaskFrame* lpActiveTask);
    void copyTimeActiveTask();
    void configureActiveTask();

private:
    QVBoxLayout *mpGlobalLayout = 0;
    QShortcut *mpNewTask, *mpStartTask;
    TaskFrame *mpActiveTask = 0;
};

#endif // TASKLISTWIDGET_H
