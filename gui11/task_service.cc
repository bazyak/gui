#include "task_service.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QString>

#include "all_tasks_window.h"
#include "tasks_table_model.h"

TaskService::TaskService(QObject* parent)
    : QObject(parent)
    , db(QSqlDatabase::addDatabase("QSQLITE"))
    , model(new TasksTableModel(this, db))
    , allTasksWindow(new AllTasksWindow(*model, this))
{
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

TaskService::~TaskService()
{
    model->submitAll();
}

void TaskService::loadBase()
{
    db.setDatabaseName("tasks.db");

    if (!db.open())
    {
        emit initEnd(false, 0);
        return;
    }
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Tasks ( \
        id INTEGER NOT NULL UNIQUE, \
        title TEXT NOT NULL UNIQUE, \
        deadline TEXT NOT NULL, \
        progress INTEGER NOT NULL, \
        PRIMARY KEY(id AUTOINCREMENT))"
    );
    model->setTable("Tasks");
    emit initEnd(model->select(), model->rowCount());
}

void TaskService::preSaveTask(QString const& title, QString const& deadline, QString const& progress)
{
    QSqlRecord rec { };
    rec.append(QSqlField { "title" });
    rec.append(QSqlField { "deadline" });
    rec.append(QSqlField { "progress" });
    rec.setValue(0, title);
    rec.setValue(1, deadline);
    rec.setValue(2, progress);

    auto addRow = [](auto const& model, auto const& record) -> bool
    {
        for (auto i = 0; i < model->rowCount(); ++i)
        {
            if (model->data(model->index(i, 1), Qt::DisplayRole) == record.value(0))
                return false;
        }
        return model->insertRecord(-1, record);
    };
    emit taskSaved(addRow(model, rec), title, deadline, progress);
}

void TaskService::showAll() const
{
    allTasksWindow->show();
}
