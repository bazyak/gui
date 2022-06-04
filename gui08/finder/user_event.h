#pragma once

#include <QEvent>
#include <QString>

class UserEvent : public QEvent
{
public:
    enum { TypeEvent = User + 1 };

    explicit UserEvent();

    QString textMsg() const;
    void setMsg(QString const& msg);

private:
    QString msg;
};
