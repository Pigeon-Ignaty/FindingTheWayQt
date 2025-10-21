#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QCloseEvent>
#include "CustomGraphicsView.h"
#include "CustomGraphicsScene.h"

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

    CustomGraphicsScene *m_scene = nullptr;
    CustomGraphicsView * m_view = nullptr;
    QLineEdit *m_widthLineEdit;
    QLineEdit *m_heightLineEdit;
    QPushButton *m_generateButton;
protected:
    void closeEvent(QCloseEvent *event) override; //Выходим из приложения и сохраняем настройки в ini
};
