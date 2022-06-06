#include "seeker.h"

#include <QObject>
#include <QThread>
#include <QString>
#include <QDirIterator>
#include <QCoreApplication>

#include "user_event.h"

Seeker::Seeker(QObject* parent) : QThread(parent)
{
}

void Seeker::stopWork()
{
    isWork = false;
}

void Seeker::setInputValues(QString const& toFind, QString const& root)
{
    this->toFind = toFind;
    this->root = root;
}

void Seeker::run()
{
    if (!toFind.length() || !root.length()) return;

    isWork = true;
    QDirIterator it(root, QStringList() << toFind,
                    QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    while (isWork && it.hasNext())
    {
        auto const path = it.next();
        auto const event = new UserEvent();
        event->setMsg(path);
        QCoreApplication::postEvent(parent(), event);
    }
    emit resultReady(!isWork);
}
