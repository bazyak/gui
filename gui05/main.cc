#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QString>

#include "global_consts.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    // solution 1
    w.setStyleSheet(qss_styles::MAIN);

    // solution 2 (slowly)
    /*
    QFile file { ":/themes.qss" };
    file.open(QIODevice::ReadOnly);
    QString const qss_style { file.readAll() };
    if (!qss_style.isEmpty())
    {
        w.setStyleSheet(qss_style);
    }
    */

    w.show();

    return a.exec();
}
