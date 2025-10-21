#include "CustomGraphicsView.h"
#include "Square.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView (parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//Маштабируем в центре мыши
    setDragMode(QGraphicsView::ScrollHandDrag);//Возможность перемещения мышью
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
