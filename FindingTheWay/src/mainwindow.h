#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void setupUi();// Собираем UI

    QGraphicsScene *m_scene;
    QGraphicsView *m_field;

    QLineEdit *m_widthLineEdit;
    QLineEdit *m_heightLineEdit;
    QPushButton *m_generateButton;
};
