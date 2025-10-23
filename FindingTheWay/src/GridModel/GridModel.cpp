#include "GridModel.h"
#include "random"
GridModel::GridModel(int width, int height) : m_width(width), m_height(height)
{
    m_cells.resize(m_width * m_height);//Вектор пустых ячеек
    m_cells.fill(CellType::Empty);
}

CellType GridModel::getCell(int x, int y) const
{
    if(!inBounds(x,y)) return CellType::Error;
    return m_cells.value(y * m_width + x);
}

void GridModel::setCell(int x, int y, CellType type)
{
    if(!inBounds(x,y)) return;
    m_cells[y *m_width + x] = type;
}

bool GridModel::inBounds(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

void GridModel::setPointA(int x, int y)
{
    m_pointA.setX(x);
    m_pointA.setY(y);
}

void GridModel::setPointB(int x, int y)
{
    m_pointB.setX(x);
    m_pointB.setY(y);
}
