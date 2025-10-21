#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QCloseEvent>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void setupUi();// Собираем UI
    void resetDefaultSettings(QSettings & settings);//Сбрасываем некорректные настройки
    void loadSettings(QSettings & settings);//Загружаем настройки
    void saveSettings(QSettings & settings);//Сохраняем

    QGraphicsScene *m_scene;
    QGraphicsView *m_field;

    QLineEdit *m_widthLineEdit;
    QLineEdit *m_heightLineEdit;
    QPushButton *m_generateButton;
protected:
    void closeEvent(QCloseEvent *event) override; //Выходим из приложения и сохраняем настройки в ini
};
