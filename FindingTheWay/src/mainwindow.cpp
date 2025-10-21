#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
    QSettings settings("config.ini", QSettings::IniFormat); //Открываем ini файл
    resetDefaultSettings(settings);
    loadSettings(settings);
    saveSettings(settings);
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
    m_scene = new CustomGraphicsScene();
    m_view = new CustomGraphicsView(this);
    m_view->setScene(m_scene);
    m_scene->createGrid();

    m_view->setMinimumSize(500, 500);
    m_view->setRenderHint(QPainter::Antialiasing);

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

    leftLayout->addWidget(m_view);
    leftLayout->addLayout(m_sizesLayout);

    //Область с кнопкой генерировать
    QVBoxLayout *rightLayout = new QVBoxLayout();
    m_generateButton = new QPushButton("Генерировать");
    m_generateButton->setFixedSize(200,50);
    m_generateButton->setFont(font);
    rightLayout->addWidget(m_generateButton);
    rightLayout->addStretch();


    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout);

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
}

void MainWindow::resetDefaultSettings(QSettings &settings)
{
    //Сбрасываем значения по умолчанию, если они не были найдены
    if (!settings.contains("App/Geometry")) {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();

        int width = static_cast<int>(screenGeometry.width() * 0.6);
        int height = static_cast<int>(screenGeometry.height() * 0.6);
        this->resize(width,height);
        this->move(screenGeometry.x() + (screenGeometry.width() - width) / 2,
                   screenGeometry.y() + (screenGeometry.height() - height) / 2);
        settings.setValue("App/Geometry", saveGeometry());
    }

    if (!settings.contains("UI/WidthLineEdit")) {
        settings.setValue("UI/WidthLineEdit", 10);
    }
    if (!settings.contains("UI/HeightLineEdit")) {
        settings.setValue("UI/HeightLineEdit", 10);
    }

}

void MainWindow::loadSettings(QSettings &settings)
{
    //Геометрия
    QByteArray geometry = settings.value("App/Geometry").toByteArray();
    if(!geometry.isEmpty())
        restoreGeometry(geometry);
    //Длина
    m_widthLineEdit->setText(settings.value("UI/WidthLineEdit").toString());
    //Высота
    m_heightLineEdit->setText(settings.value("UI/HeightLineEdit").toString());
}

void MainWindow::saveSettings(QSettings &settings)
{
    //Геометрия
    settings.setValue("App/Geometry", saveGeometry());
    //Длина
    settings.setValue("UI/WidthLineEdit", m_widthLineEdit->text());
    //Высота
    settings.setValue("UI/HeightLineEdit", m_heightLineEdit->text());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("config.ini", QSettings::IniFormat); //сохранение настроек пользователя
    saveSettings(settings);
    event->accept();
}
