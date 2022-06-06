#pragma once

#include <QObject>

class QEvent;
class QString;
class QKeySequence;

class KeyEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit KeyEventFilter(QObject* parent = nullptr);

signals:
    void changeHotKey(QString const& objName, QKeySequence const& keySeq);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};
