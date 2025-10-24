cmake_minimum_required(VERSION 3.14)

project(FindingTheWay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Concurrent REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/CustomGraphicsScene/CustomGraphicsScene.cpp
    src/CustomGraphicsView/CustomGraphicsView.cpp
    src/GridItem/GridItem.cpp
    src/GridModel/GridModel.cpp
)

set(HEADERS
    src/mainwindow.h
    src/CustomGraphicsScene/CustomGraphicsScene.h
    src/CustomGraphicsView/CustomGraphicsView.h
    src/GridItem/GridItem.h
    src/GridModel/GridModel.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/CustomGraphicsScene
    src/CustomGraphicsView
    src/GridItem
    src/GridModel
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Concurrent)
