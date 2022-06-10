#include "task_service.h"

#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QString>

#include <vector>

TaskService::TaskService(QObject* parent) : QObject(parent)
{
}

void TaskService::loadTasks()
{
    file = new QFile("./testbase.bin", this);
    if (!file) return;

    if (!file->open(QIODevice::ReadWrite))
    {
        emit initEnd(false, 0);
        return;
    }
    for (; readTask(); );
    emit initEnd(true, loadedTasks.size());
}

void TaskService::preSaveTask(QString const& title, QString const& deadline, QString const& progress)
{
    tasksToSave.emplace_back(Task { title, deadline, progress });
}

void TaskService::saveTasks()
{
    for (auto const& task : tasksToSave)
    {
        writeTask(task);
    }
}

bool TaskService::readTask()
{
    if (file->atEnd()) return false;

    QDataStream stream(file);
    QByteArray byteArray { };
    int len = 0;

    auto const readProperty = [&]() -> QString
    {
        stream.readRawData(reinterpret_cast<char*>(&len), sizeof len);
        byteArray.resize(len);
        stream.readRawData(byteArray.data(), len);
        return QString::fromUtf8(byteArray);
    };

    auto const title = readProperty();
    auto const deadline = readProperty();
    auto const progress = readProperty();

    loadedTasks.emplace_back(Task { title, deadline, progress });
    emit loadTask(title, deadline, progress);

    return true;
}

void TaskService::writeTask(Task const& task)
{
    QDataStream stream(file);
    QByteArray bytes { };
    int len = 0;

    auto writeProperty = [&](auto const& str) mutable
    {
        bytes = str.toUtf8();
        len = bytes.length();
        stream.writeRawData(reinterpret_cast<char*>(&len), sizeof len);
        stream.writeRawData(bytes.data(), len);
    };

    writeProperty(task.title);
    writeProperty(task.deadline);
    writeProperty(task.progress);
}
