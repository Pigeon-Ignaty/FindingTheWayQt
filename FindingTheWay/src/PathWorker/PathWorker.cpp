#include "PathWorker.h"

PathWorker::PathWorker(QObject *parent) : QObject(parent){

}

void PathWorker::findTheWayBFSAsync(GridModel model) {
    m_stop = false;
    GridModel copy = model; //Копируем модель и очищаем путь
    copy.clearPath();

    auto pair = copy.findTheWayBFSInterruptible(m_stop);//Получаем путь в копии и отправляем сигналы
    bool found = pair.first;
    if (!m_stop && found && !(copy == model)) {
        emit pathFound(copy);
    }
    else{
        emit pathNotFound();

    }

    emit finished();
}


void PathWorker::requestStop() {
    m_stop = true;
}
