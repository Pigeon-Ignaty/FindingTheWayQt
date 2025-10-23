#pragma once

#include <QObject>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <GridModel.h>
class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomGraphicsScene(QObject *parent = nullptr);
};
