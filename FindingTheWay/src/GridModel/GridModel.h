#pragma once

#include <QObject>
#include <QVector>
#include <QPoint>

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
    void generateWalls(double chance);    //Генерация блуждающих стен
    QPair<bool,GridModel> findTheWayBFS(); //Поиск в ширину для concurrent
    QPair<bool,GridModel> findTheWayBFSInterruptible(std::atomic_bool &stopFlag); //Поиск в ширину для worker + qthread с возможностью остановить итерацию

    //Получение высоты и длины
    inline int width() {return m_width; }
    inline int height() {return m_height; }
    CellType getCell(int x, int y) const; //получаем тип ячейки
    void setCell(int x, int y, CellType type = CellType::Empty);//Задаём тип ячейки
    bool inBounds(int x, int y) const;//Проверка, что точка в сетке

    //Методы для работы с точками A и B
    QPoint getPointA(){return m_pointA;}
    QPoint getPointB(){return m_pointB;}
    QPoint getPointC(){return m_pointC;}

    void setPointA(int x, int y);
    void setPointB(int x, int y);
    void setPointC(int x, int y);

    bool operator== (const GridModel &other) const{
        return m_cells == other.m_cells && m_pointA == other.m_pointA && m_pointB == other.m_pointB;
    }
    void clearPath();//Очищаем отрисованные пути перед новым поиском
    void clearAfterCtrl();//Убираем точку B с поля для режима ctrl
private:
    int m_width;
    int m_height;
    QVector <CellType> m_cells; //Все клетки в одномерно массиве
    QPoint m_pointA{-1,-1};
    QPoint m_pointB{-1,-1};
    QPoint m_pointC{-1,-1};//Временная для ctrl режима

    void randomFillWalls(double chance, int maxAttempts = 10); //Случайно заполняем стенами поле
    void setPathCell( QVector<QPoint> &pathPoints);//Устанавливаем в m_cells все пути
};

Q_DECLARE_METATYPE(GridModel)
