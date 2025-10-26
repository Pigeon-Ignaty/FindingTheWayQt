#pragma once

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <GridModel.h>
class GridItem : public QGraphicsItem
{
public:
    GridItem(GridModel *model);
    QRectF getBoundingRect() const { return boundingRect(); }
    int getSellSize() {return m_sellSize;}
protected:
    QRectF boundingRect() const override;//размеры сетки
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;//нажатие мыши на квадрат
    void paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) override;
    void drawPathCell(QPainter &painter, const QRect& rect);
private:
    GridModel *m_model = nullptr;
    const int m_sellSize = 30;//по умолчанию 30 пикселей
};
