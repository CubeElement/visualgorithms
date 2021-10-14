#include "grid.h"
#include "ui_grid.h"
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
#include <queue>
#include <algorithm>

Grid::Grid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Grid)
    , GRID_SIZE(15)
    , DELAY_MS(8)
{
    ui->setupUi(this);
    this->m_createGrid(GRID_SIZE);
    QObject::connect( this, &Grid::stopAnimation,
                      this, &Grid::killLoop);
    QLabel* tip = new QLabel("Left Mouse: set route / clear grid, Right Mouse: set obstacle", this);
    tip->setAlignment(Qt::AlignCenter);
    ui->statusBar->insertPermanentWidget(0, tip, 1);
}

Grid::~Grid()
{
    delete ui;
}

void Grid::m_createGrid(int size_n)
{
    QWidget* grid = new QWidget();
    ui->mainlayout->addWidget(grid);

    this->resize( 30*size_n+1.35*size_n, 30*size_n+25+1.35*size_n);

    int counter = 0;
    for ( int i = 0; i < size_n; i++ )
    {
        for ( int j = 0; j < size_n; j++ )
        {
            QPushButton* element = new QPushButton(grid);
            this->grid_elements.push_back(element);
            element->setGeometry(j*30, i*30, 29, 29);
            this->setCellColor(element, "bg");
//            element->setText(QString::number(counter));
            counter++;
            QObject::connect(element, &QPushButton::pressed,
                             this, &Grid::leftMouseButton);
            element->installEventFilter(this);
        }
    }
}

int Grid::m_getElementPos(QPushButton* elem)
{
    for ( int i = 0; i < this->grid_elements.size(); ++i )
        if ( this->grid_elements[i] == elem )
            return i;
    return -1;
}

void Grid::leftMouseButton()
{
    QPushButton* button = qobject_cast<QPushButton*> (sender());
    setRouteDestinations(button);
}

void Grid::rightMouseButton(QPushButton* cell)
{
    if ( !killLoopFlag ) { emit stopAnimation(); clearGrid(grid_elements); }
    this->setCellColor(cell, "blocked");
    this->blockedCells.push_back(m_getElementPos(cell));
}

void Grid::setRouteDestinations(QPushButton* cell)
{
    int cellPos = m_getElementPos(cell);
    if ( route.empty() )
    {
        this->route.push_back(cellPos);
        this->setCellColor(cell, "start");
    } else if ( route.size() == 1 )
    {
        this->route.push_back(cellPos);
        this->setCellColor(cell, "end");

        std::vector <int>::iterator it = this->route.begin();
        int start = *it;
        int end = *std::next(it);
        shortestPath(start, end);
    } else if ( route.size() == 2 )
    {
        emit stopAnimation();
        this->clearGrid(this->grid_elements);
    }
}

void Grid::drawShortestPath(std::vector<int> prev, int end, int start)
{
    if ( prev[end] != 0 )
    {
        int cell = end;
        std::list<int> reversed;
        reversed.push_front(cell);
        while ( cell != start )
        {
            cell = prev[cell];
            reversed.push_front(cell);
        }
        for ( int cell : reversed )
        {
            delay(DELAY_MS);
            this->setCellColor(this->grid_elements[cell], "path");
        }
    }
}

void Grid::clearGrid (std::vector<QPushButton*> cells)
{
    this->route.clear();
    this->blockedCells.clear();
    for (auto cell : cells)
    {
        this->setCellColor(cell, "bg");
    }
}

void Grid::delay(int msec)
{
    QEventLoop loop;
    QTimer timer;
    timer.connect(&timer, &QTimer::timeout,
                  &loop, &QEventLoop::quit);
    timer.start(msec);
    loop.exec();
}

std::vector <int> Grid::neighborsList(int cell, int const GRID_SIZE)
{
    std::vector <int> adjacent_cells;

    int coor_x = (cell+1 <= GRID_SIZE ?  cell : cell % GRID_SIZE);
    int coor_y = cell / GRID_SIZE;

    if ( coor_y - 1 >= 0 ) adjacent_cells.push_back(cell - GRID_SIZE);
    if ( coor_x + 1 < GRID_SIZE ) adjacent_cells.push_back( cell + 1 );
    if ( coor_y + 1 < GRID_SIZE ) adjacent_cells.push_back( cell + GRID_SIZE );
    if ( coor_x - 1 >= 0 ) adjacent_cells.push_back( cell - 1 );

    return adjacent_cells;
}

bool Grid::eventFilter(QObject* obj, QEvent *event)
{
    QPushButton* cell = qobject_cast<QPushButton*>(obj);
    if ( event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* keyEvent = static_cast<QMouseEvent*>(event);
        if ( keyEvent->button() == Qt::RightButton )
        {
            this->rightMouseButton(cell);
            return true;
        } else
        {
            return QObject::eventFilter(obj, event);
        }
    }
    return false;
}

void Grid::setCellColor(QPushButton *cell, QString type)
{
    QStringList types;
    types << "bg" << "start" << "end" << "blocked" << "path" << "searched"
          << "current";
    switch( types.indexOf(type) )
    {
    case 0:  // bg
        cell->setStyleSheet("background-color: rgba(201, 195, 167, 0.4); "
                            "border: none;");
        break;
    case 1:  // start
        cell->setStyleSheet("background-color: rgba(67, 168, 73, 0.9); "
                            "border: none;");
        break;
    case 2:  // end
        cell->setStyleSheet("background-color: rgba(67, 168, 73, 0.9); "
                            "border: none;");
        break;
    case 3:  // blocked
        cell->setStyleSheet("background-color: rgba(75, 75, 75, 0.9); "
                            "border: none;");
        break;
    case 4:  // path
        cell->setStyleSheet("background-color: rgba(240, 65, 43, 0.9); "
                            "border: none;");
        break;
    case 5:  // searched
        cell->setStyleSheet("background-color: rgba(255, 176, 221, 0.9); "
                            "border: none;");
        break;
    case 6:  // current
        cell->setStyleSheet("background-color: rgba(186, 176, 225, 0.9); "
                            "border: none;");
        break;
    }
}
