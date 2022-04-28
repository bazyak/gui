#include "lang_model.h"

#include <QStandardItem>
#include <QApplication>
#include <QList>
#include <QStyle>
#include <QIcon>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QString>
#include <memory>


// Exercise 2
LangModel::LangModel(QObject* parent)
    : model_(std::make_unique<QStandardItemModel>(parent))
{
    QList<QStandardItem*> lang_list;

    lang_list.emplace_back(new QStandardItem(
                                QIcon(QApplication::style()->standardIcon(
                                          QStyle::SP_MediaPlay)),
                                "С++"));
    lang_list.emplace_back(new QStandardItem(
                                QIcon(QApplication::style()->standardIcon(
                                          QStyle::SP_ComputerIcon)),
                                "Python"));
    lang_list.emplace_back(new QStandardItem(
                                QIcon(QApplication::style()->standardIcon(
                                          QStyle::SP_DirIcon)),
                                "Java"));
    lang_list.emplace_back(new QStandardItem(
                                QIcon(QApplication::style()->standardIcon(
                                          QStyle::SP_DriveCDIcon)),
                                "C#"));
    lang_list.emplace_back(new QStandardItem(
                                QIcon(QApplication::style()->standardIcon(
                                          QStyle::SP_FileLinkIcon)),
                                "PHP"));
    lang_list.emplace_back(new QStandardItem(
                                QIcon(QApplication::style()->standardIcon(
                                          QStyle::SP_ArrowUp)),
                                "JavaScript"));
    for (auto&& el : lang_list)
    {
        el->setFlags(el->flags() & ~(Qt::ItemIsDropEnabled));
        model_->appendRow(el);
    }
}


QAbstractItemModel* LangModel::GetModel() const
{
    return model_.get();
}


void LangModel::AddItem()
{
    auto item = new QStandardItem(QIcon(QApplication::style()->standardIcon(
                                            QStyle::SP_TrashIcon)),
                                  "Новый язык");
    item->setFlags(item->flags() & ~(Qt::ItemIsDropEnabled));
    model_->appendRow(item);
}


QModelIndex LangModel::GetLastIndex() const
{
    return model_->index(model_->rowCount()-1, 0);
}


int LangModel::Count() const
{
    return model_->rowCount();
}


QString LangModel::GetValueByIndex(int row) const
{
    return model_->data(model_->index(row, 0)).toString();
}


bool LangModel::RemoveItemByIndex(int row)
{
    return model_->removeRow(row);
}
