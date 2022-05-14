#pragma once

#include <QObject>
#include <unordered_map>
#include <QString>

class QEvent;
class QShortcut;

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    using hk_map_t = std::unordered_map<QString, std::shared_ptr<QShortcut>>;

    explicit KeyEventFilter(hk_map_t& map, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    hk_map_t& map_;
};
