#include "GridModel.h"
#include "random"
#include <QQueue>
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

bool GridModel::findTheWayBFS()
{
    if(m_pointA == QPoint(-1,-1) || m_pointB == QPoint(-1,-1) ){
        return false;
    }
    int W = m_width;
    int H = m_height;

    QVector<bool> visited(W * H, false);//список проверенных клеток
    QVector<int> parent(W * H, -1);//родитель для индекса
    QQueue<int>queue; //очеред из клеток. Сначала соседи, потом соседи соседей

    int indexA = m_pointA.y() * W +m_pointA.x(); //индексы точек для m_cells
    int indexB = m_pointB.y() * W +m_pointB.x();

    //Отмечаем точку А как посещённую и добавляем в очередь
    visited[indexA] = true;
    queue.enqueue(indexA);

    //Сдвиги в 4 направления от точки
    const int moveX[4] = {1, -1, 0, 0};
    const int moveY[4] = {0, 0, 1, -1};

    bool isFoundB = false;
    //Исследуем соседей, пока они не закончатся
    while(!queue.isEmpty()){
        int index = queue.dequeue();//Берём из очереди индекс
        if(index == indexB){//Сравниваем с индексом B
            isFoundB = true;
            break;
        }
        //Координаты из индекса этой точки
        int x = index%W;
        int y = index/W;
        //проходимся по 4 соседям клетки
        for(int dir = 0; dir < 4; dir++){
            int neighborX = x + moveX[dir];
            int neighborY = y + moveY[dir];

            if(!inBounds(neighborX, neighborY))//Если соседняя вне сетки, берём другю соседнюю клетку
                continue;

            int neighborIndex = neighborY * W +neighborX;//Индекс соседней клетки

            if(visited[neighborIndex])//Если соседняя посещена, то след
                continue;
            if(getCell(neighborX, neighborY) == CellType::Wall)//И если соседняя ячейка стенка, то след
                continue;

            //Отмечаем клетку посещённой, задаём родител и кидаем в очередь
            visited[neighborIndex] = true;
            parent[neighborIndex] = index;
            queue.enqueue(neighborIndex);
        }
    }
    //Если не найден путь
    QVector<QPoint> pathPoints;
    if(!isFoundB)
        return false;

    int curr = indexB;
    while(curr != -1){ //пока не придём к точку A
        //Получаем координаты точки
        int currentX = curr % W;
        int currentY = curr / W;
        //Записываем в вектор
        pathPoints.push_back(QPoint(currentX, currentY));
        curr = parent[curr];//Берём родительскую точку и повторяем
    }
    std::reverse(pathPoints.begin(), pathPoints.end());

    //Меняем состояние блоков в qvector
    setPathCell(pathPoints);

    return true;
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

void GridModel::clearPath()
{
    for(auto &cell : m_cells){
        if(cell == CellType::Path)
            cell = CellType::Empty;
    }
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

void GridModel::setPathCell(QVector<QPoint> &pathPoints)
{
    for(auto& point : pathPoints){
        int index = point.y() * m_width +point.x();
        if(m_cells[index] != CellType::Wall && m_cells[index] != CellType::Input && m_cells[index] != CellType::OutPut){
            m_cells[index] = CellType::Path;
        }
    }
}
