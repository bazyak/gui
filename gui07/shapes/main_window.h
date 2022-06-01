#pragma once

#include <QGraphicsView>

class BlockScheme;
class QGraphicsScene;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class QPainter;
class QRectF;

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onReDraw();
    void onWhlEvent(QGraphicsSceneWheelEvent* event);
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void drawBackground(QPainter* painter, QRectF const& rect) override;

    void scaleView(qreal const scale_factor);

private:
    uint const kWidth = 800;
    uint const kHeight = 600;

    QGraphicsScene* scene_;
    BlockScheme* scheme_;
};
