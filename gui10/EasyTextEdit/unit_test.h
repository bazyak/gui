#pragma once

#include <QObject>

class UnitTest : public QObject
{
    Q_OBJECT

public:
    explicit UnitTest(QObject* parent = nullptr);

private slots:
    void testGui();
};
