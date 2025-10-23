#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QGraphicsRectItem>
#include <QIntValidator>
#include <QMessageBox>

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
    //Ограничиваем значение в qlineedit
    QIntValidator *validator = new QIntValidator(minSizeField, maxSizeField,this);
    m_widthLineEdit->setValidator(validator);
    m_heightLineEdit->setValidator(validator);

    m_sizesLayout->addStretch();
    m_sizesLayout->addWidget(labelWidth);
    m_sizesLayout->addWidget(m_widthLineEdit);
    m_sizesLayout->addSpacing(30);
    m_sizesLayout->addWidget(labelHeight);
    m_sizesLayout->addWidget(m_heightLineEdit);
    m_sizesLayout->addStretch();

    leftLayout->addWidget(m_view);
    leftLayout->addLayout(m_sizesLayout);

    //Правая часть с кнопками
    QVBoxLayout *rightLayout = new QVBoxLayout();
    m_createFieldButton = new QPushButton("Создать поле");
    m_createFieldButton->setFixedSize(200,50);
    m_createFieldButton->setFont(font);

    m_generateWallButton = new QPushButton("Сгенерировать стены");
    m_generateWallButton->setFixedSize(200,50);
    m_generateWallButton->setFont(font);

    m_findTheWayButton = new QPushButton("Найти путь");
    m_findTheWayButton->setFixedSize(200,50);
    m_findTheWayButton->setFont(font);

    rightLayout->addWidget(m_createFieldButton);
    rightLayout->addWidget(m_generateWallButton);
    rightLayout->addWidget(m_findTheWayButton);
    rightLayout->addStretch();

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setSpacing(10);


    connect(m_createFieldButton, &QPushButton::clicked, this, [this](){
        int width = m_widthLineEdit->text().toInt();
        int height = m_heightLineEdit->text().toInt();

        //проверка выхода значений за границы
        bool widthError = (width < minSizeField || width > maxSizeField);
        bool heightError = (height < minSizeField || height > maxSizeField);

        QString errors;

        if(widthError || heightError) {
            errors = "Значение длины";

            if(widthError && heightError)
                errors += " W и H ";
            else if(widthError)
                errors += " W ";
            else if(heightError)
                errors += " H ";

            errors += QString("выходит за допустимые пределы. Мин - %1, Макс - %2.").arg(minSizeField).arg(maxSizeField);

            QMessageBox::warning(this, "Ошибка", errors);
            return;
        }

        //Удаляем старые элементы сцены
        for(auto item : m_scene->items()) {
            m_scene->removeItem(item);
            delete item;
        }
        m_gridItem = nullptr;

        //Удаляем старую модель
        if(m_model) {
            delete m_model;
            m_model = nullptr;
        }

        //Создание новой модели
        m_model = new GridModel(width, height);
        m_gridItem = new GridItem(m_model);

        //Установим фокус зума в центре элемента
        m_gridItem->setTransformOriginPoint(m_gridItem->getBoundingRect().center());

        QRectF rect = m_gridItem->getBoundingRect();
        m_gridItem->setPos(-rect.width() / 2, -rect.height() / 2);

        m_scene->addItem(m_gridItem);

        m_scene->setSceneRect(m_scene->itemsBoundingRect());
        m_view->centerOn(m_gridItem);
    });
    connect(m_generateWallButton, &QPushButton::clicked, this, [this](){
        //Если не сделан предыдущий шаг, то выходим
        if(!m_model || m_model->getPointA() == QPoint(-1,-1) || m_model->getPointB() == QPoint(-1,-1)){
            QMessageBox::warning(this, "Ошибка", "Не все точки установлены!");
            return;
        }
        m_model->generateWalls(0.5);
        m_gridItem->update();
    });
}


void MainWindow::resetDefaultSettings(QSettings &settings)
{
    // Сбрасываем значения по умолчанию, если они не были найдены
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
