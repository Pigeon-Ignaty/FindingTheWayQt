#include "CustomGraphicsView.h"
#include "GridItem.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView (parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//Маштабируем в центре мыши
    setCursor(Qt::ArrowCursor);
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    double currentScaleValue = transform().m11();//текущий зум по x (по y одинаково)

    if(event->angleDelta().y() > 0)//Если прокрутили вверх
    {
        if(currentScaleValue > MaxZoom)
            return;
        scale(ZoomStep,ZoomStep);
    }
    else{ //Если вниз прокрутили
        if(currentScaleValue < MinZoom)
            return;
        scale(1/ZoomStep,1/ZoomStep);

    }
    event->accept();
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton){//Средняя клавиша активирует режим перетаскивания
        m_lastPosition = event->pos();
        m_modeMoving = true;
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_modeMoving){
        QPoint move = event->pos() - m_lastPosition;//Получаем сдвиг курсора
        //Двигаем сцену
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - move.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - move.y());
        m_lastPosition = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton){//Отпускаем колёсико и отключаем режим перемещения
        m_modeMoving = false;
        setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::mouseReleaseEvent(event);
}
