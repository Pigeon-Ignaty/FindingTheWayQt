#include "CustomGraphicsScene.h"
#include "Square.h"

CustomGraphicsScene::CustomGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{

}

void CustomGraphicsScene::createGrid(int columns, int rows)
{
    this->clear();
    //Рисуем квадраты сверху вниз, слева направо
    for(int row = 0; row < rows; row++){
        for (int column = 0;column < columns; column++) {
            Square *square = new Square(column * squareSize, row * squareSize, squareSize);
            this->addItem(square);
        }
    }
}
