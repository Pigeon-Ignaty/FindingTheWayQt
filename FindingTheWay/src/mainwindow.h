#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QCloseEvent>
#include "CustomGraphicsView.h"
#include "GridModel.h"
#include <GridItem.h>
#include <QFuture>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <PathWorker.h>
#include <QGraphicsScene>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    static constexpr int minSizeField = 2;//Минимальный размер поля
    static constexpr int maxSizeField = 100;//Максимальный размер

    void setupUi();// Собираем UI
    void resetDefaultSettings(QSettings & settings);//Сбрасываем некорректные настройки
    void loadSettings(QSettings & settings);//Загружаем настройки
    void saveSettings(QSettings & settings);//Сохраняем
    void setStyle();//Стили виджетов

    QGraphicsScene *m_scene = nullptr;
    CustomGraphicsView * m_view = nullptr;
    GridModel *m_model = nullptr;
    GridItem *m_gridItem = nullptr;
    QLineEdit *m_widthLineEdit;
    QLineEdit *m_heightLineEdit;
    QPushButton *m_createFieldButton = nullptr;
    QPushButton *m_generateWallButton = nullptr;
    QPushButton *m_findTheWayButton = nullptr;
    QFutureWatcher<QPair<bool,GridModel>> *m_watcher = nullptr;
    PathWorker *m_worker = nullptr;
    QThread *m_thread = nullptr;

    //Координаты последней последней ячейки для построения пути в ctrl
    int m_lastX = -1;
    int m_lastY = -1;
protected:
    void closeEvent(QCloseEvent *event) override; //Выходим из приложения и сохраняем настройки в ini
public slots:
    void slotCreateField();
    void slotFindTheWayButton();
    void slotCtrlMode(bool m_modeCtrl, QPointF pos);
};
