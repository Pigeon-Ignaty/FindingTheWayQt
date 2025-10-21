#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    setWindowTitle("Поиск минимального пути");
    this->resize(1280, 720);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    QFont font;
    font.setPointSize(12);

    //Левая часть
    QVBoxLayout *leftLayout = new QVBoxLayout();

    //Сцена и поле
    m_scene = new QGraphicsScene(0, 0, 300, 300);
    m_field = new QGraphicsView(m_scene);
    m_field->setMinimumSize(500, 500);
    m_field->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *m_sizesLayout = new QHBoxLayout;
    QLabel *labelWidth = new QLabel("W");
    QLabel *labelHeight = new QLabel("H");

    labelWidth->setFont(font);
    labelHeight->setFont(font);

    m_widthLineEdit = new QLineEdit;
    m_widthLineEdit->setFixedWidth(50);
    m_widthLineEdit->setFont(font);
    m_heightLineEdit = new QLineEdit;
    m_heightLineEdit->setFixedWidth(50);
    m_heightLineEdit->setFont(font);

    //Область для размеров
    m_sizesLayout->addStretch();
    m_sizesLayout->addWidget(labelWidth);
    m_sizesLayout->addWidget(m_widthLineEdit);
    m_sizesLayout->addSpacing(30);
    m_sizesLayout->addWidget(labelHeight);
    m_sizesLayout->addWidget(m_heightLineEdit);
    m_sizesLayout->addStretch();

    leftLayout->addWidget(m_field);
    leftLayout->addLayout(m_sizesLayout);

    //Область с кнопкой генерировать
    QVBoxLayout *rightLayout = new QVBoxLayout();
    m_generateButton = new QPushButton("Генерировать");
    m_generateButton->setFixedSize(200,30);
    m_generateButton->setFont(font);
    rightLayout->addWidget(m_generateButton);
    rightLayout->addStretch();


    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout);

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
}
