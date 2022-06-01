#pragma once

#include <QObject>
#include <QString>

#include <unordered_map>

class QEvent;
class QShortcut;
class MainWindow;

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    using HotkeysMap = std::unordered_map<QString, std::shared_ptr<QShortcut>>;

    explicit KeyEventFilter(HotkeysMap& m, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    HotkeysMap& map_;
    MainWindow* parent_ { nullptr };
};
