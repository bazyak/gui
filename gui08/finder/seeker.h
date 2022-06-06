#pragma once

#include <QObject>
#include <QThread>
#include <QString>

class Seeker : public QThread
{
    Q_OBJECT

public:
    explicit Seeker(QObject* parent);
    void stopWork();
    void setInputValues(QString const& toFind, QString const& root);

signals:
    void resultReady(bool stopped);

protected:
    void run() override;

private:
    bool isWork { true };
    QString toFind { };
    QString root { };
};
