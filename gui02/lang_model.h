#pragma once

#include <QStandardItemModel>
#include <QModelIndex>
#include <QString>
#include <memory>


// Exercise 2
class LangModel
{
public:
    LangModel(QObject* parent = nullptr);

    QAbstractItemModel* GetModel() const;
    QModelIndex GetLastIndex() const;
    int Count() const;
    QString GetValueByIndex(int row) const;

    void AddItem();
    bool RemoveItemByIndex(int row);

private:
    std::unique_ptr<QStandardItemModel> model_;
};

