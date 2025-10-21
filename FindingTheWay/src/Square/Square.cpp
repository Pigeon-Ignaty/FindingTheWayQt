#include "Square.h"
#include <QGraphicsSceneMouseEvent>

Square::Square(qreal x, qreal y, qreal size, QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    setRect(x, y, size, size);
    setBrush(Qt::white);
    setPen(QPen(Qt::gray));
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Qt::KeyboardModifiers mods = event->modifiers();

    //Если зажат ctrl, то запретить случайное окрашивание квадрата
    if(mods & Qt::ControlModifier){
        event->ignore();
        return;
    }
    //Выделяем красным, если пустой или наоборот красный в белый
    if(brush().color() == Qt::white)
        setBrush(Qt::red);
    else {
        setBrush(Qt::white);
    }

    QGraphicsRectItem::mouseMoveEvent(event);
}
