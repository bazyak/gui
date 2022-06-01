#include "main_window.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QRectF>

#include "block_scheme.h"

MainWindow::MainWindow(QWidget* parent) : QGraphicsView(parent)
{
    setFixedSize(kWidth, kHeight);
    setWindowTitle("Tasks 7.2 & 7.3");

    scene_ = new QGraphicsScene(this);
    scene_->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene_);

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    scheme_ = new BlockScheme(kWidth, kHeight, scene_);
    scene_->addItem(scheme_);

    connect(scheme_, &BlockScheme::reDraw, this, &MainWindow::onReDraw);
    connect(scheme_, &BlockScheme::whlEvent, this, &MainWindow::onWhlEvent);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onReDraw()
{
    scene_->update();
}

void MainWindow::onWhlEvent(QGraphicsSceneWheelEvent* event)
{
    scaleView(pow(2., -event->delta() / 240.0));
}

void MainWindow::zoomIn()
{
    scaleView(qreal(1.2));
}

void MainWindow::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void MainWindow::drawBackground(QPainter* painter, QRectF const& rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    painter->drawRect(sceneRect);
}

void MainWindow::scaleView(qreal const scale_factor)
{
    auto const factor = transform().scale(scale_factor, scale_factor)
                                   .mapRect(QRectF(0, 0, 1, 1))
                                   .width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scale_factor, scale_factor);
}
