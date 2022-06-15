#include <QApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>

#include "task_service.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/gui11/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<TaskService>("com.bazyak.TaskService", 1, 0, "TaskService");

    engine.load(url);

    return app.exec();
}
