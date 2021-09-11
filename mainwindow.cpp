#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QDebug>
#include <QObject>
#include <QPushButton>

#include <iostream>
#include <list>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), GRID_SIZE(10)
{
    ui->setupUi(this);
    this->createGrid(GRID_SIZE);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createGrid(int n)
{
    QWidget* grid = new QWidget();
    ui->mainlayout->addWidget(grid);

    this->resize( 30*n+20, 30*n+20);

    int counter = 0;
    for ( int i = 0; i < n; i++ )
    {
        for ( int j = 0; j < n; j++ )
        {
            QPushButton* element = new QPushButton(grid);
            element->setGeometry(j*30, i*30, 29, 29);
            element->setStyleSheet("QPushButton {"
                                   "background-color: rgba(46, 204, 113, 0.4); border: none;"
                                   "}");
            element->setText(QString::number(counter));
            counter++;
            QObject::connect(element, &QPushButton::pressed,
                             this, &MainWindow::pressedButton);
        }
    }
}

void MainWindow::pressedButton()
{
    QPushButton *button = qobject_cast<QPushButton *> (sender());
    button->setStyleSheet("QPushButton {"
                              "background-color: rgba(80, 50, 113, 0.4); border: none;"
                              "}");
    std::list <int> blocked;
    findPath(21, 99, blocked);
}

std::vector <int> MainWindow::findPath(int start, int end, std::list <int> blocked)
{
    std::vector <int> previous(GRID_SIZE^2);
    std::list <int> queue;
    std::list <int> visited;
    queue.push_back(start);
    visited.push_back(start);

    int current = start;
    bool foundEnd = false;
    while ( !queue.empty() && !foundEnd)
    {
        current = queue.front();
        queue.pop_front();
        for ( int neighbor : neighbors(current, GRID_SIZE) )
        {
            if ( std::count(visited.begin(), visited.end(), neighbor) == 0)
            {
                queue.push_back(neighbor);
                visited.push_back(neighbor);
                previous[neighbor] = current;
            }
            if ( neighbor == end ) { foundEnd = true; break; }
        }
    }
    printShortestPath(previous);
    return previous;
}

void MainWindow::printShortestPath(std::vector <int> prev)
{
    int cell = prev.back();
    while ( cell != 0 )
    {
        qDebug() << cell;
        cell = prev[cell];
    }
}

std::list <int> MainWindow::neighbors(int cell, int const GRID_SIZE)
{
    std::list <int> adjacent_cells;

    int coor_x = (cell+1 <= GRID_SIZE ?  cell : cell % GRID_SIZE);
    int coor_y = cell / GRID_SIZE;

    if ( coor_y - 1 >= 0 ) adjacent_cells.push_back(cell - GRID_SIZE);
    if ( coor_x + 1 < GRID_SIZE ) adjacent_cells.push_back( cell + 1 );
    if ( coor_y + 1 < GRID_SIZE ) adjacent_cells.push_back( cell + GRID_SIZE );
    if ( coor_x - 1 >= 0 ) adjacent_cells.push_back( cell - 1 );

    return adjacent_cells;
}
