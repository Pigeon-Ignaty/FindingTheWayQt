#pragma once

#include <QObject>
#include <QVector>

enum class CellType{
    Error = -1,
    Empty = 0,
    Wall = 1,
    Path = 2,
    Input = 3,
    OutPut = 4
};

class GridModel
{

public:
    GridModel(int width = 10, int height = 10);
    //Получение высоты и длины
    inline int width() {return m_width; }
    inline int height() {return m_height; }
    CellType getCell(int x, int y) const; //получаем тип ячейки
    void setCell(int x, int y, CellType type = CellType::Empty);//Задаём тип ячейки
    bool inBounds(int x, int y) const;//Проверка, что точка в сетке

    //Методы для работы с точками A и B
    QPoint getPointA(){return m_pointA;}
    QPoint getPointB(){return m_pointB;}

    void setPointA(int x, int y);
    void setPointB(int x, int y);
private:
    int m_width;
    int m_height;
    QVector <CellType> m_cells; //Все клетки в одномерно массиве
    QPoint m_pointA{-1,-1};
    QPoint m_pointB{-1,-1};
};

