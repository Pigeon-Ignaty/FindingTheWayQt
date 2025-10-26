#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QGraphicsRectItem>
#include <QIntValidator>
#include <QMessageBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat); //Открываем ini файл
    resetDefaultSettings(settings);
    loadSettings(settings);
    saveSettings(settings);
}

MainWindow::~MainWindow()
{
    if (m_watcher) {
        m_watcher->cancel();
        m_watcher->waitForFinished();
        m_watcher->deleteLater();
        m_watcher = nullptr;
    }

    if (m_thread) {
        m_worker->requestStop();
        m_thread->quit();
        m_thread->wait();
        m_thread->deleteLater();
        m_thread = nullptr;
    }
}

void MainWindow::setupUi()
{
    setWindowTitle("Поиск минимального пути");
    setWindowIcon(QIcon(":/logo.ico"));

    this->resize(1280, 720);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    QFont font;
    font.setPointSize(12);

    //Левая часть
    QVBoxLayout *leftLayout = new QVBoxLayout();

    //Сцена и поле
    m_scene = new QGraphicsScene();
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


    connect(m_createFieldButton, &QPushButton::clicked, this, &MainWindow::slotCreateField);

    //Создаём поток для работы ctrl режима
    m_worker = new PathWorker;
    m_thread = new QThread(this);
    m_worker->moveToThread(m_thread);

    connect(m_thread, &QThread::finished,m_worker, &QObject::deleteLater);
    connect(m_worker, &QObject::destroyed, [this]() { m_worker = nullptr; });

    connect(m_worker, &PathWorker::pathFound,this, [this](const GridModel &result){//Если найден путь, то копируем в основую модель и рисуем
        *m_model = result;
        m_gridItem->update();
    });
    connect(m_worker, &PathWorker::pathNotFound, this,[this](){//Если не найден, то стираем путь
        m_model->clearAfterCtrl();
        m_gridItem->update();
    });
    m_thread->start();
    connect(m_generateWallButton, &QPushButton::clicked, this, [this](){
        m_generateWallButton->setEnabled(false);
        if(!m_model || !m_gridItem) {
            m_generateWallButton->setEnabled(true);
            return;
        }
        m_model->generateWalls(0.5);
        m_gridItem->update();
        m_generateWallButton->setEnabled(true);
    });

    m_watcher = new QFutureWatcher<QPair<bool,GridModel>>(this);
    connect(m_watcher, &QFutureWatcher<QPair<bool, GridModel>>::finished, this, [this](){
        auto pair = m_watcher->result();
        if(!pair.first){//если false, то путь не найден
            QMessageBox::warning(this, "Ошибка", "Не удалось найти путь!");
        }
        else{//Иначе обновляем модель и перерисовываем сетку
            *m_model = pair.second;
            m_gridItem->update();
        }
        m_findTheWayButton->setEnabled(true);

    }, Qt::UniqueConnection);

    connect(m_findTheWayButton, &QPushButton::clicked, this, &MainWindow::slotFindTheWayButton);

    //Реализуем работу в ctrl режиме
    connect(m_view, &CustomGraphicsView::signalCtrlMode, this, &MainWindow::slotCtrlMode);

    setStyle();
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

void MainWindow::setStyle()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f2f3f5;
            color: #202020;
            font-family: "Segoe UI", "Noto Sans", "Ubuntu", "DejaVu Sans", sans-serif;
        }

        QPushButton {
            background-color: #e6e8eb;
            border: 1px solid #c5c7ca;
            border-radius: 6px;
            padding: 6px;
        }
        QPushButton:hover {
            background-color: #dfe1e5;
        }
        QPushButton:pressed {
            background-color: #d0d3d8;
        }

        QLineEdit {
            background-color: #ffffff;
            border: 1px solid #c5c7ca;
            border-radius: 4px;
            padding: 3px;
        }

        QLabel {
            color: #202020;
        }

        QGraphicsView {
            background-color: #fafafa;
            border: 1px solid #c5c7ca;
        }
    )");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat); //сохранение настроек пользователя
    saveSettings(settings);

    event->accept();
}

void MainWindow::slotCreateField()
{
    m_createFieldButton->setEnabled(false);
    int width = m_widthLineEdit->text().toInt();
    int height = m_heightLineEdit->text().toInt();

    //проверка выхода значений за границы
    bool widthError = (width < minSizeField || width > maxSizeField);
    bool heightError = (height < minSizeField || height > maxSizeField);

    QString errors;

    if(widthError || heightError) {
        errors = "Значение";
        if (widthError && heightError)
            errors += " ширины (W) и высоты (H) ";
        else if (widthError)
            errors += " ширины (W) ";
        else if (heightError)
            errors += " высоты (H) ";

        errors += QString("выходит за допустимые пределы: Мин - %1, Макс - %2.").arg(minSizeField).arg(maxSizeField);
        QMessageBox::warning(this, "Ошибка", errors);

        m_createFieldButton->setEnabled(true);
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
    m_createFieldButton->setEnabled(true);
}

void MainWindow::slotFindTheWayButton()
{
    if (m_watcher->isRunning()) {
        QMessageBox::information(this, "Внимание", "Поиск пути уже выполняется!");
        return;
    }
    m_findTheWayButton->setEnabled(false);

    if(!m_model || m_model->getPointA() == QPoint(-1,-1) || m_model->getPointB() == QPoint(-1,-1)){
        QMessageBox::warning(this, "Ошибка", "Не все точки установлены!");
        m_findTheWayButton->setEnabled(true);
        return;
    }

    m_model->clearPath(); //Очищаем пути из модели

    GridModel copyModel = *m_model;//Копия модели, чтобы без гонок

    QFuture<QPair<bool, GridModel>> future = QtConcurrent::run([copyModel]() mutable -> QPair<bool, GridModel>{
        return copyModel.findTheWayBFS();//Запускаем поиск пути и возвращаем изменённую модель, если путь найден
    });
    //Если работает поток, то отменяем
    if (m_watcher->isRunning()) {
        m_watcher->cancel();
        m_watcher->waitForFinished();
    }
    m_watcher->setFuture(future);
}

void MainWindow::slotCtrlMode(bool m_modeCtrl, QPointF pos)
{
    if(!m_model || !m_gridItem || !m_worker) return;

    if(m_modeCtrl){
        //Преобразовываем глобальные координаты в координаты виджета
        QList<QGraphicsItem*> itemsUnder = m_scene->items(pos);
        if(itemsUnder.isEmpty()) return;

        GridItem *grid = nullptr;
        for(auto it : itemsUnder){
            grid = qgraphicsitem_cast<GridItem*>(it);
            if(grid) break;
        }
        if(!grid) return;

        QPointF itemPos = grid->mapFromScene(pos);

        const int cellSize = grid->getCellSize();
        int x = static_cast<int>(std::floor(itemPos.x() / double(cellSize)));
        int y = static_cast<int>(std::floor(itemPos.y() / double(cellSize)));

        if(!m_model->inBounds(x,y)) return;
        if(x == m_lastX && y == m_lastY) return; //Если координаты не изменились с последней попытки, то выходим

        m_lastX = x;
        m_lastY = y;

        GridModel copy = *m_model;
        copy.setPointC(x, y); //Устанавливаем временную цель в копии

        //Остановим прошлый поиск
        if(m_worker) m_worker->requestStop();

        //Запускаем новый поиск в фоне
        QMetaObject::invokeMethod(m_worker, "findTheWayBFSAsync", Qt::QueuedConnection, Q_ARG(GridModel, copy));
    }
    else{
        //Если отпустили последний ctrl, прерываем поток и очищаем пути
        if(m_worker) m_worker->requestStop();
        m_model->clearAfterCtrl();
        m_model->setPointC(-1,-1);
        m_gridItem->update();
        m_lastX = m_lastY = -1;
    }
}
