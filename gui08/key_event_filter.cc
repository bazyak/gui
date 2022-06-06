#include "key_event_filter.h"

#include <QEvent>
#include <QObject>
#include <QLineEdit>
#include <QKeyEvent>
#include <QKeySequence>
#include <QKeyCombination>

KeyEventFilter::KeyEventFilter(QObject* parent) : QObject(parent)
{
}

bool KeyEventFilter::eventFilter(QObject* obj, QEvent* event)
{
    auto const widget = dynamic_cast<QLineEdit*>(obj);
    if (widget == nullptr)
        return false;

    if (widget->hasFocus() && event->type() == QEvent::KeyPress)
    {
        auto const trueEvent = dynamic_cast<QKeyEvent*>(event);

        auto const key = trueEvent->key();
        auto const modifiers = trueEvent->modifiers();
        auto const hotKey = QKeyCombination(modifiers, Qt::Key(key));

        // setup new hotkey
        emit changeHotKey(widget->accessibleName(), QKeySequence(hotKey));

        // pretty print hotkey
        auto const comb = QKeyCombination(Qt::Key(key));
        auto const strKey = QKeySequence(comb).toString(QKeySequence::NativeText);
        auto strHotKey = QKeySequence(hotKey).toString(QKeySequence::NativeText);
        if (strHotKey.count(strKey) > 1)
        {
            auto const pos = strHotKey.lastIndexOf(strKey);
            strHotKey.remove(pos, strKey.length());
        }
        widget->setText(strHotKey);

        return true;
    }
    return false;
}
