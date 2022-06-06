#include "user_event.h"

#include <QEvent>
#include <QString>

UserEvent::UserEvent() : QEvent(static_cast<Type>(TypeEvent))
{
}

QString UserEvent::textMsg() const
{
    return msg;
}

void UserEvent::setMsg(QString const& msg)
{
    this->msg = msg;
}
