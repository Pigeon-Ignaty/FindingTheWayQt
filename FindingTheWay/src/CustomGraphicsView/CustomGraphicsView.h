#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent = nullptr);
private:
    static constexpr double ZoomStep = 1.10;//коэфф. увеличения или уменьшения колёсиком мыши
    static constexpr double MinZoom = 1.0;//мин отдаление
    static constexpr double MaxZoom = 5.0;//макс приближение

    bool m_modeMoving = false;
    QPoint m_lastPosition{-1,-1};
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};
