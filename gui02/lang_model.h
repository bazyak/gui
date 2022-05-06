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

    QAbstractItemModel* getModel() const;
    QModelIndex getLastIndex() const;
    int count() const;
    QString getValueByIndex(int row) const;

    void addItem();
    void addItem(QIcon const& icon, QString const& text);
    bool removeItemByIndex(int row);

private:
    std::unique_ptr<QStandardItemModel> model_;
};

