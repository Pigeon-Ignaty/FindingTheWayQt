#include "GridModel.h"
#include "random"
GridModel::GridModel(int width, int height) : m_width(width), m_height(height)
{
    m_cells.resize(m_width * m_height);//Вектор пустых ячеек
    m_cells.fill(CellType::Empty);
}

void GridModel::generateWalls(double chance)
{
    randomFillWalls(chance);

    //Реализуем клеточное блуждание
    std::mt19937 value(std::random_device{}());
    std::uniform_int_distribution<> dirDist(0, 3); //генерируем 4 направления стенки


    const int maxSteps = 5; //5 итераций над стенками
    for(int step = 0;step < maxSteps; step++) {
        auto newCells = m_cells; //Делаем копию, чтобы новое положение не влияло во время текущего прохода

        //Берём каждую ячейку и генерируем направление движения
        for(int index = 0; index < m_cells.size(); index++){
            if(m_cells[index] == CellType::Wall){
                int direction = dirDist(value); //слуйчаное направление
                int x = index % m_width;
                int y = index / m_width;

                switch(direction) {
                    case 0: y -=1; break;
                    case 1: y +=1; break;
                    case 2: x -=1; break;
                    case 3: x +=1; break;

                }
                //Проверка, что не вышли из сетки
                if(x >= 0 && x < m_width && y >= 0 && y < m_height){
                    int newIndex = y * m_width + x;
                    //Шагаем стеной в новую клету, а старую чистим
                    if(m_cells[newIndex] == CellType::Empty){
                        newCells[newIndex] = CellType::Wall;
                        newCells[index] = CellType::Empty;
                    }
                }
            }
        }
        m_cells = newCells;
    }
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

void GridModel::randomFillWalls(double chance, int maxAttempts)
{
    bool isEmptyField = true;//Если не сгенерировалось хотя бы одной стенки
    std::mt19937 value(std::random_device{}());
    std::uniform_real_distribution<>dist(0.0, 1.0);

    for(auto &cell : m_cells) {
        if(dist(value) < chance && cell != CellType::Input && cell != CellType::OutPut) {
            cell = CellType::Wall;
            isEmptyField = false;
        }
        else if(cell != CellType::Input && cell != CellType::OutPut) {
            cell = CellType::Empty;
        }
    }
    //Даём 10 попыток, если поле оказалось пустым
    if(isEmptyField && maxAttempts > 0){
        randomFillWalls(chance, maxAttempts - 1);
    }
}
