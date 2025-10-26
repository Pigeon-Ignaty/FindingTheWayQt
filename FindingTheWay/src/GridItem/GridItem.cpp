#include "GridItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <GridModel.h>

GridItem::GridItem(GridModel *model) : m_model(model)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

void GridItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos(); //Текущая позиция
    int x = static_cast<int>(pos.x() / m_sellSize);
    int y = static_cast<int>(pos.y() / m_sellSize);

    //Проверка, что тыкнули по сетке
    if(x >= 0 && x < m_model->width() && y >= 0 && y < m_model->height()){
        CellType currentCell = m_model->getCell(x,y);//Тип квадрата

        if(currentCell == CellType::Input){//Если снимаем А
            m_model->setCell(x,y,CellType::Empty);
            m_model->setPointA(-1,-1);
        }
        else if(currentCell == CellType::OutPut){//Если снимаем B
            m_model->setCell(x,y,CellType::Empty);
            m_model->setPointB(-1,-1);
        }
        else if(currentCell == CellType::Empty){//Если тыкнули на пустую ячейку
            if(m_model->getPointA() == QPoint(-1,-1)){//Если не задана A
                m_model->setCell(x,y,CellType::Input);
                m_model->setPointA(x,y);
            }
            else if(m_model->getPointB() == QPoint(-1,-1)){//Если не задана B
                m_model->setCell(x,y,CellType::OutPut);
                m_model->setPointB(x,y);
            }
        }
        update();
    }
}

QRectF GridItem::boundingRect() const
{
    return QRectF(0, 0,m_model->width() * m_sellSize, m_model->height() * m_sellSize);//Размер сетки
}

void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);//Так стенки клеток отрисовываются равномерно
    QPen pen(Qt::gray);
    pen.setCosmetic(true); // Толщина пера не масштабируется
    painter->setPen(pen);

    //Рисуем квадраты
    for(int y = 0; y < m_model->height(); y++){
        for(int x = 0; x < m_model->width(); x++){
            QRect rect(x * m_sellSize, y *m_sellSize,m_sellSize,m_sellSize);

            switch(m_model->getCell(x, y)) {
                case CellType::Wall:
                    painter->fillRect(rect, Qt::darkGray);
                    break;
                case CellType::Empty:
                    painter->fillRect(rect, Qt::white);
                    break;
                case CellType::Path:
                    painter->fillRect(rect, Qt::red);
                    drawPathCell(*painter,rect);
                    break;
                case CellType::Input:{
                    painter->fillRect(rect, Qt::red);
                    painter->setPen(QPen(Qt::white,1));
                    painter->drawText(rect,Qt::AlignCenter, "A");
                    break;
                }
                case CellType::OutPut:{
                    painter->fillRect(rect, Qt::red);
                    painter->setPen(QPen(Qt::white,1));
                    painter->drawText(rect,Qt::AlignCenter, "B");
                    break;
                }
                case CellType::Error:
                    painter->fillRect(rect, Qt::magenta);
                    break;
                default:
                    painter->fillRect(rect, Qt::white);
                    break;
            }
        }
    }

    //Рисуем линии
    painter->setPen(QPen(Qt::black, 1));
    for(int x = 0; x <= m_model->width(); x++){
        painter->drawLine(x * m_sellSize, 0, x* m_sellSize, m_model->height() * m_sellSize);
    }
    for(int y = 0; y <= m_model->height(); y++){
        painter->drawLine(0, y * m_sellSize, m_model->width() * m_sellSize, y * m_sellSize);
    }
}

void GridItem::drawPathCell(QPainter &painter, const QRect &rect)
{
    const int step = 2; // расстояние между линиями

    painter.save();
    painter.setClipRect(rect);//Ограничиваем рисовку ячейкой
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor red(255, 0, 0, 255);
    QColor white(255, 255, 255, 255);

    QPen pen;
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter.setPen(pen);

    for (int i = -rect.height(); i < rect.width(); i += step) {//Идём по диагонали
        QPen redPen = QPen(red, 1, Qt::SolidLine, Qt::RoundCap);
        QPen whitePen = QPen(white, 1, Qt::SolidLine, Qt::RoundCap);

        painter.setPen(((i / step) % 2 == 0) ? redPen : whitePen);//Чередуем белый с красным

        QPoint p1(rect.left() + i, rect.bottom());
        QPoint p2(rect.left() + i + rect.height(), rect.top());
        painter.drawLine(p1, p2);
    }

    painter.restore();
}
