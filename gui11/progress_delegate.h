#pragma once

#include <QItemDelegate>
#include <QObject>

class QModelIndex;
class QPainter;
class QStyleOptionViewItem;

class ProgressDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ProgressDelegate(QObject* parent = nullptr);
    void paint(QPainter* painter, QStyleOptionViewItem const& option,
               QModelIndex const& index) const override;
};
