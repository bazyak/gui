#include "main_window.h"

#include <QApplication>
#include <QtTest/QTest>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QDebug>
#include <QSysInfo>

#include "global_consts.h"
#include "unit_test.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    QCommandLineOption testOption({"t", "makeTest"},
                                  QCoreApplication::translate("main", "Make a tests"));
    QCommandLineOption fileOption({"f", "fileName"},
                                  QCoreApplication::translate("main", "Save results to <file>"),
                                  QCoreApplication::translate("main", "file"));

    parser.addOption(testOption);
    parser.addOption(fileOption);
    parser.process(app);

    if (parser.isSet(testOption))
    {
        QString dir = QSysInfo::productType() == "macos"
                ? QApplication::applicationDirPath() + "/../../../"
                : QApplication::applicationDirPath() + "/";

        auto fileName = parser.value(fileOption);
        fileName = fileName.length()
                ? dir + fileName
                : dir + "test_result.txt";

        QStringList testCmd;
        testCmd << " " << "-o" << fileName;
        QTest::qExec(new UnitTest(), testCmd);
        return EXIT_SUCCESS;
    }

    MainWindow w;
    w.setStyleSheet(qss_styles::MAIN);
    w.show();

    return app.exec();
}
