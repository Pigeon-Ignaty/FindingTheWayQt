#pragma once

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
class Square : public QGraphicsRectItem
{
public:
    Square(qreal x, qreal y, qreal size, QGraphicsItem *parent = nullptr);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;//нажатие мыши на квадрат
};
