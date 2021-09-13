#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QDebug>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>
#include <QStringList>

#include <iostream>
#include <list>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), GRID_SIZE(15)
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
            this->grid_elements.push_back(element);
            element->setGeometry(j*30, i*30, 29, 29);
            this->setCellColor(element, "bg");
            element->setText(QString::number(counter));
            counter++;
            QObject::connect(element, &QPushButton::pressed,
                             this, &MainWindow::leftMouseButton);
            element->installEventFilter(this);
        }
    }
}

void MainWindow::setCellColor(QPushButton *cell, QString type)
{
    QStringList types;
    types << "bg" << "start" << "end" << "blocked" << "path";
    switch( types.indexOf(type) )
    {
    case 0:
        cell->setStyleSheet("background-color: rgba(201, 195, 167, 0.9); "
                            "border: none;");
        break;
    case 1:
        cell->setStyleSheet("background-color: rgba(67, 168, 73, 0.9); "
                            "border: none;");
        break;
    case 2:
        cell->setStyleSheet("background-color: rgba(67, 168, 73, 0.9); "
                            "border: none;");
        break;
    case 3:
        cell->setStyleSheet("background-color: rgba(120, 83, 31, 0.9); "
                            "border: none;");
        break;
    case 4:
        cell->setStyleSheet("background-color: rgba(174, 103, 245, 0.9); "
                            "border: none;");
        break;
    }
}

void MainWindow::leftMouseButton()
{
    QPushButton* button = qobject_cast<QPushButton*> (sender());
    setRouteDestinations(button);
}

void MainWindow::setObstacles(QPushButton* cell)
{
    this->setCellColor(cell, "blocked");
    this->blockedCells.push_back(cell->text().toInt());
}

void MainWindow::setRouteDestinations(QPushButton* cell)
{
    int cell_val = (cell->text()).toInt();
    if ( route.empty() )
    {
        this->route.push_back(cell_val);
        this->setCellColor(cell, "start");
    } else if ( route.size() == 1 )
    {
        this->route.push_back(cell_val);
        this->setCellColor(cell, "end");
        std::list <int>::iterator it = this->route.begin();
        int start = *it;
        int end = *std::next(it);
        findPath(start, end);
    } else if ( route.size() == 2 )
    {
        this->clearGrid(this->grid_elements);
    }
}

std::vector <int> MainWindow::findPath(int start, int end)
{
    std::vector <int> previous(GRID_SIZE*GRID_SIZE);
    std::list <int> queue;
    std::list <int> visited = this->blockedCells;
    queue.push_back(start);
    visited.push_back(start);

    int current = start;
    bool foundEnd = false;
    while ( !queue.empty() && !foundEnd)
    {
        current = queue.front();
        queue.pop_front();
        for ( int neighbor : neighborsList(current, GRID_SIZE) )
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
    drawShortestPath(previous, end, start);
    return previous;
}

void MainWindow::drawShortestPath(std::vector <int> prev, int end, int start)
{
    int cell = end;
    while ( cell != start )
    {
        this->setCellColor(this->grid_elements[cell], "path");
        cell = prev[cell];
    }
}

void MainWindow::clearGrid (std::vector<QPushButton*> cells)
{
    this->route.clear();
    this->blockedCells.clear();
    for (auto cell : cells)
    {
//        this->delay(8);
        this->setCellColor(cell, "bg");
    }
}

void MainWindow::delay(int msec)
{
    QEventLoop loop;
    QTimer timer;
    timer.connect(&timer, &QTimer::timeout,
                  &loop, &QEventLoop::quit);
    timer.start(msec);
    loop.exec();
}

std::list <int> MainWindow::neighborsList(int cell, int const GRID_SIZE)
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

bool MainWindow::eventFilter(QObject* obj, QEvent *event)
{
    QPushButton* cell = qobject_cast<QPushButton*>(obj);
    if ( event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* keyEvent = static_cast<QMouseEvent*>(event);
        if ( keyEvent->button() == Qt::RightButton )
        {
            this->setObstacles(cell);
            return true;
        } else
        {
            return QObject::eventFilter(obj, event);
        }
    }
    return false;
}
