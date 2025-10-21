cmake_minimum_required(VERSION 3.14)

project(FindingTheWay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Файлы проекта
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

set(HEADERS
    src/mainwindow.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
