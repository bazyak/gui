#include "progress_delegate.h"

#include <QItemDelegate>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QProgressBar>
#include <QPixmap>

ProgressDelegate::ProgressDelegate(QObject* parent) : QItemDelegate(parent)
{
}

void ProgressDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
    QStyleOptionViewItem opt = option;
    opt.palette.setColor(QPalette::Highlight, Qt::white);
    QItemDelegate::paint(painter, opt, index);

    auto progress = index.model()->data(index, Qt::DisplayRole).toInt();
    auto rect = option.rect.adjusted(4, 4, -4, -4);

    QProgressBar prog;
    prog.setMinimum(0);
    prog.setMaximum(10);
    prog.setStyleSheet("background-color: white");
    prog.setValue(progress);
    prog.setTextVisible(false);
    auto pxmap { prog.grab() };

    painter->save();
    painter->drawPixmap(rect, pxmap);
    painter->restore();
}
