#pragma once

#include <QObject>
#include <QSqlDatabase>

class QString;
class TasksTableModel;
class AllTasksWindow;

class TaskService : public QObject
{
    Q_OBJECT

public:
    explicit TaskService(QObject* parent = nullptr);
    ~TaskService();

    Q_INVOKABLE
    void loadBase();
    Q_INVOKABLE
    void preSaveTask(QString const& title, QString const& deadline, QString const& progress);
    Q_INVOKABLE
    void showAll() const;

signals:
    void initEnd(bool succeed, int count);
    void taskSaved(bool saved, QString const& title, QString const& deadline, QString const& progress);

private:
    QSqlDatabase db { };
    TasksTableModel* model { nullptr };
    AllTasksWindow* allTasksWindow { nullptr };
};
