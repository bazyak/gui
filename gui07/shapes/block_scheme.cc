#include "block_scheme.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QPolygon>
#include <QPoint>
#include <QRandomGenerator>

#include <unordered_map>

BlockScheme::BlockScheme(int const width, int const height, QObject* parent)
    : QObject(parent), QGraphicsItem()
    , width_(width)
    , height_(height)
{
    brush_.setStyle(Qt::BrushStyle::SolidPattern);
}

void BlockScheme::setBrush(QBrush const& brush)
{
    this->brush_ = brush;
    emit reDraw();
}

void BlockScheme::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    constexpr auto star = [](auto const& sh) -> QPolygon {
        QPolygon star_polygon;
        star_polygon << QPoint(sh.pt.x(), sh.pt.y() + sh.height * 0.3)
                     << QPoint(sh.pt.x() + sh.width * 1, sh.pt.y() + sh.height * 0.3)
                     << QPoint(sh.pt.x() + sh.width * 0.1, sh.pt.y() + sh.height * 1)
                     << QPoint(sh.pt.x() + sh.width * 0.5, sh.pt.y())
                     << QPoint(sh.pt.x() + sh.width * 0.9, sh.pt.y() + sh.height * 1);
        return star_polygon;
    };

    for (auto const& sh : shapes_)
    {
        brush_.setColor(sh.color);
        painter->setBrush(brush_);
        auto const center_x = sh.pt.x() + sh.width * 0.5;
        auto const center_y = sh.pt.y() + sh.height * 0.5;
        painter->save();
        painter->translate(center_x, center_y);
        painter->rotate(sh.rotate);
        painter->translate(-center_x, -center_y);
        switch(sh.geometry)
        {
        case Geometry::Rectangle:
            painter->drawRect(sh.pt.x(), sh.pt.y(), sh.width, sh.height);
            break;
        case Geometry::Ellips:
            painter->drawEllipse(sh.pt.x(), sh.pt.y(), sh.width, sh.height);
            break;
        case Geometry::Star:
            painter->drawPolygon(star(sh), Qt::WindingFill);
            break;
        default:
            break;
        }
        painter->restore();
    }
}

QRectF BlockScheme::boundingRect() const
{
    return QRectF(0, 0, width_, height_);
}

std::optional<std::size_t> BlockScheme::isPointInsideShape(QPoint const& point)
{
    constexpr auto inside = [](auto const& pt, auto const& sh) -> bool
    {
        return pt.x() >= sh.pt.x() && pt.x() <= sh.pt.x() + sh.width &&
               pt.y() >= sh.pt.y() && pt.y() <= sh.pt.y() + sh.height;
    };

    for (auto it = shapes_.rbegin(); it < shapes_.rend(); ++it)
    {
        if (inside(point, *it))
        {
            auto const idx = std::distance(shapes_.begin(), it.base()) - 1;
            return std::make_optional(idx);
        }
    }
    return std::nullopt;
}

void BlockScheme::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    bpoint_ = event->pos().toPoint();

    auto idx = isPointInsideShape(bpoint_);
    if (idx)
    {
        moving_ = true;
        current_ = *idx;
    }

    // using "moving_" as "inside flag"
    if (event->button() == Qt::RightButton && moving_)
    {
        moving_ = false;
        shapes_.erase(std::next(shapes_.begin(), current_));
    }
    else if (event->button() == Qt::LeftButton && !moving_)
    {
        auto const geom = !shapes_.size()
                ? Geometry::First
                : next(shapes_[shapes_.size() - 1].geometry);

        shapes_.emplace_back(Shape { { bpoint_.x(), bpoint_.y() },
                                     sizes_[geom].first,
                                     sizes_[geom].second,
                                     0,
                                     QColor::fromRgb(QRandomGenerator::global()->generate()),
                                     geom });
    }
    emit reDraw();
}

void BlockScheme::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && moving_)
    {
        moving_ = false;
    }
}

void BlockScheme::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (moving_ && current_ < shapes_.size())
    {
        QPoint p = event->pos().toPoint() - bpoint_;
        /*
        // no move shape over borders
        auto new_pt = shapes_[current_].pt + p;
        if (new_pt.x() < 0 || new_pt.y() < 0 ||
            new_pt.x() + shapes_[current_].width > width_ ||
            new_pt.y() + shapes_[current_].height > height_)
            return;
        */
        shapes_[current_].pt += p;
        bpoint_ = event->pos().toPoint();
        emit reDraw();
    }
}

void BlockScheme::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit dblClick();
    }
}

void BlockScheme::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (event->buttons() & Qt::MiddleButton)
    {
        auto const pt = event->pos().toPoint();
        auto const idx = isPointInsideShape(pt);
        if (idx)
        {
            shapes_[*idx].rotate = (shapes_[*idx].rotate + event->delta() / 8) % 360;
            emit reDraw();
        }
    }
    else
    {
        emit whlEvent(event);
    }
}
