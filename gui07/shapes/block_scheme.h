#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QBrush>
#include <QColor>
#include <QPoint>

#include <vector>
#include <unordered_map>
#include <utility>
#include <optional>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneMouseEvent;
class QRectF;

class BlockScheme : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(QBrush brush)

public:
    explicit BlockScheme(int const width, int const height, QObject* parent = nullptr);
    void setBrush(QBrush const& brush);

signals:
    void reDraw();
    void dblClick();
    void whlEvent(QGraphicsSceneWheelEvent* event);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    enum class Geometry : uint { Rectangle = 1, Ellips, Star, First = Rectangle, Last = Star };
    struct Shape
    {
        QPoint pt { 0, 0 };
        int width = 0;
        int height = 0;
        int rotate = 0;
        QColor color { };
        Geometry geometry = Geometry::First;
    };

    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) override;
    QRectF boundingRect() const override;
    std::optional<std::size_t> isPointInsideShape(QPoint const& point);

    static inline std::unordered_map<Geometry, std::pair<int, int>> sizes_
    {
        { Geometry::Rectangle, { 200, 100 } },
        { Geometry::Ellips, { 150, 90 } },
        { Geometry::Star, { 100, 100 } }
    };
    std::vector<Shape> shapes_ { };
    QPoint bpoint_ { };
    QBrush brush_ { };
    std::size_t current_ = 0;
    bool moving_ = false;
    int width_ = 0;
    int height_ = 0;
};

template <class E, class = std::enable_if_t<std::is_enum<E>{ }>>
E next(E e)
{
  return e == E::Last ? E::First
                      : static_cast<E>(static_cast<std::underlying_type_t<E>>(e) + 1);
}
