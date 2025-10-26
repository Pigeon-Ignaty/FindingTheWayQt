#pragma once

#include <QObject>
#include "GridModel.h"
class PathWorker : public QObject
{
    Q_OBJECT
public:
    PathWorker(QObject *parent = nullptr);
    void requestStop(); //Метод остановки потока

public slots:
    void findTheWayBFSAsync(GridModel model);//Выполнение поиска асинхронно

signals:
    //Сигналы о состоянии поиска пути
    void pathFound(const GridModel &result);
    void pathNotFound();

    void finished();

private:
    std::atomic<bool> m_stop{false};
};
