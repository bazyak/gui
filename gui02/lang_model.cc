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
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay)), "С++");
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon)), "Python");
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)), "Java");
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveCDIcon)), "С#");
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileLinkIcon)), "PHP");
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp)), "JavaScript");
}

QAbstractItemModel* LangModel::getModel() const
{
    return model_.get();
}

void LangModel::addItem()
{
    addItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon)), "Новый язык");
}

void LangModel::addItem(QIcon const& icon, QString const& text)
{
    auto item = new QStandardItem(icon, text);
    item->setFlags(item->flags() & ~(Qt::ItemIsDropEnabled));
    model_->appendRow(item);
}

QModelIndex LangModel::getLastIndex() const
{
    return model_->index(model_->rowCount()-1, 0);
}

int LangModel::count() const
{
    return model_->rowCount();
}

QString LangModel::getValueByIndex(int row) const
{
    return model_->data(model_->index(row, 0)).toString();
}

bool LangModel::removeItemByIndex(int row)
{
    return model_->removeRow(row);
}
