#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QWheelEvent>
class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent = nullptr);
protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    static constexpr double ZoomStep = 1.10;//коэфф. увеличения или уменьшения колёсиком мыши
    static constexpr double MinZoom = 2.0;//мин отдаление
    static constexpr double MaxZoom = 5.0;//макс приближение
};
