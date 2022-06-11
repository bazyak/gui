#include "unit_test.h"

#include <QtTest/QTest>
#include <QTabWidget>

#include "main_window.h"
#include "settings_dialog.h"

UnitTest::UnitTest(QObject* parent) : QObject(parent)
{
}

void UnitTest::testGui()
{
    MainWindow w;
    w.show();

    while (!w.isVisible())
    {
         QTest::qWait(200);
    }
    QTest::qWait(1000);

    auto tabs = w.findChild<QTabWidget*>();
    auto dialog = w.findChild<SettingsDialog*>();

    QCOMPARE(tabs->count(), 1);

    auto obj = w.childAt(850, 300);
    if (obj)
    {
        QTest::mouseClick(obj, Qt::LeftButton, Qt::NoModifier, QPoint(20, 10));
    }
    QTest::qWait(1000);

    obj = dialog->childAt(430, 130);
    if (obj)
    {
        QTest::keyClick(obj, Qt::Key_I, Qt::ControlModifier);
    }
    QTest::qWait(1000);

    obj = dialog->childAt(400, 205);
    if (obj)
    {
        QTest::mouseClick(obj, Qt::LeftButton, Qt::NoModifier, QPoint(20, 10));
    }
    QTest::qWait(1000);

    QTest::keyClick(&w, Qt::Key_I, Qt::ControlModifier);
    QTest::qWait(1000);

    QCOMPARE(tabs->count(), 2);

    obj = w.childAt(850, 95);
    if (obj)
    {
        QTest::mouseClick(obj, Qt::LeftButton, Qt::NoModifier, QPoint(20, 10));
    }
    QTest::qWait(1000);

    QCOMPARE(tabs->count(), 3);

    QTest::qWait(2000);
}
