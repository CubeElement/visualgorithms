cmake_minimum_required(VERSION 3.2)

project(visualgorthms LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/grid.cpp
        src/grid.h
        src/grid.ui
        src/breadthfs.h
        src/breadthfs.cpp
        src/depthfs.h src/depthfs.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(visualgorthms
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(visualgorthms SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(visualgorthms
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(visualgorthms PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
