#pragma once

#include <QSqlTableModel>
#include <QSqlDatabase>

class QVariant;
class QModelIndex;

class TasksTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit TasksTableModel(QObject* parent = nullptr, QSqlDatabase const& db = QSqlDatabase());

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(QModelIndex const& index, int role) const override;
};
