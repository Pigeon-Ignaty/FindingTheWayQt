#pragma once

#include <QObject>
#include <QGraphicsScene>
#include <QWheelEvent>

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomGraphicsScene(QObject *parent = nullptr);
    void createGrid(int columns = 10, int rows = 10);//Метод для нарисования поля из квадратов

    static constexpr int squareSize = 30; //Размер квадрата
};
