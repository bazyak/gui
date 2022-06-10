#pragma once

#include <QObject>
#include <QString>

#include <vector>

class QFile;
class QDataStream;
class QByteArray;

class TaskService : public QObject
{
    Q_OBJECT

public:
    explicit TaskService(QObject* parent = nullptr);

    Q_INVOKABLE
    void loadTasks();
    Q_INVOKABLE
    void preSaveTask(QString const& title, QString const& deadline, QString const& progress);
    Q_INVOKABLE
    void saveTasks();

signals:
    void initEnd(bool succeed, int count);
    void loadTask(QString const& title, QString const& deadline, QString const& progress);

private:
    struct Task
    {
        QString title { };
        QString deadline { };
        QString progress { };
    };

    bool readTask();
    void writeTask(Task const& task);

    QFile* file { nullptr };
    std::vector<Task> loadedTasks { };
    std::vector<Task> tasksToSave { };
};
