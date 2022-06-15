#include "tasks_table_model.h"

#include <QSqlTableModel>
#include <QVariant>
#include <QModelIndex>
#include <QString>
#include <QFont>

#include "all_tasks_window.h"

TasksTableModel::TasksTableModel(QObject* parent, QSqlDatabase const& db)
    : QSqlTableModel(parent, db)
{
}

QVariant TasksTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            switch (section)
            {
            case 0: return "Id";
            case 1: return "Title";
            case 2: return "Deadline";
            case 3: return "Progress";
            default: return QString("Column %1").arg(section + 1);
            }
        }
        if (role == Qt::FontRole)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    }
    return QVariant();
}

QVariant TasksTableModel::data(QModelIndex const& index, int role) const
{
    if (index.column() == 2 && role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    if (role == Qt::ToolTipRole && (index.column() == 1 || index.column() == 3))
    {
        return index.data().toString();
    }
    return QSqlTableModel::data(index, role);
}
