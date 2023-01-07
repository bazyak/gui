#pragma once

#include <unordered_map>

#include <QObject>
#include <QString>

class QEvent;
class QShortcut;

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    using hk_map_t = std::unordered_map<QString, std::shared_ptr<QShortcut>>;

    explicit KeyEventFilter(hk_map_t& m, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    hk_map_t& map_;
};
