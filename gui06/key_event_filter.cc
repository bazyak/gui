#include "key_event_filter.h"

#include <QDebug>
#include <QEvent>
#include <QObject>
#include <QLineEdit>
#include <QKeyEvent>
#include <QKeySequence>
#include <QKeyCombination>
#include <QShortcut>

KeyEventFilter::KeyEventFilter(hk_map_t& m, QObject* parent)
    : QObject(parent)
    , map_(m)
{
}


bool KeyEventFilter::eventFilter(QObject* obj, QEvent* event)
{
    auto widget = dynamic_cast<QLineEdit*>(obj);
    if (widget == nullptr)
        return false;

    if (widget->hasFocus() && event->type() == QEvent::KeyPress)
    {
        auto true_event = dynamic_cast<QKeyEvent*>(event);

        auto const key = true_event->key();
        auto const modifiers = true_event->modifiers();
        auto const hot_key = QKeyCombination(modifiers, Qt::Key(key));

        // setup new hotkey
        map_[widget->objectName()]->setKey(QKeySequence(hot_key));

        // pretty print hotkey
        auto const kk = QKeyCombination(Qt::Key(key));
        auto const s_key = QKeySequence(kk).toString(QKeySequence::NativeText);
        auto s_hot_key = QKeySequence(hot_key).toString(QKeySequence::NativeText);
        if (s_hot_key.count(s_key) > 1)
        {
            auto const pos = s_hot_key.lastIndexOf(s_key);
            s_hot_key.remove(pos, s_key.length());
        }
        widget->setText(s_hot_key);

        return true;
    }
    return false;
}
