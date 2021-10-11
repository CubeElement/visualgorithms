#include "gridpath.h"
#include "ui_gridpath.h"
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

Gridpath::Gridpath(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gridpath), GRID_SIZE(15), DELAY_MS(8)
{
    ui->setupUi(this);

    this->createGrid(GRID_SIZE);
    QObject::connect( this, &Gridpath::stopAnimation,
                      this, &Gridpath::killLoop);
    QLabel* tip = new QLabel("Left Mouse: set route / clear grid, Right Mouse: set obstacle", this);
    tip->setAlignment(Qt::AlignCenter);
    ui->statusBar->insertPermanentWidget(0, tip, 1);
}

Gridpath::~Gridpath()
{
    delete ui;
}

void Gridpath::createGrid(int n)
{
    QWidget* grid = new QWidget();
    ui->mainlayout->addWidget(grid);

    this->resize( 30*n+20, 30*n+45);

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
                             this, &Gridpath::leftMouseButton);
            element->installEventFilter(this);
        }
    }
}

void Gridpath::leftMouseButton()
{
    QPushButton* button = qobject_cast<QPushButton*> (sender());
    setRouteDestinations(button);
}

void Gridpath::rightMouseButton(QPushButton* cell)
{
    if ( !killLoopFlag ) { emit stopAnimation(); clearGrid(grid_elements); }
    this->setCellColor(cell, "blocked");
    this->blockedCells.push_back(cell->text().toInt());
}

void Gridpath::setRouteDestinations(QPushButton* cell)
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
        findPathBFS(start, end);
    } else if ( route.size() == 2 )
    {
        emit stopAnimation();
        this->clearGrid(this->grid_elements);
    }
}

std::vector <int> Gridpath::findPathBFS(int start, int end)
{
    std::vector <int> previous(GRID_SIZE*GRID_SIZE);
    std::list <int> queue;
    std::list <int> visited = this->blockedCells;
    queue.push_back(start);
    visited.push_back(start);

    int current = start;
    bool foundEnd = false;

    killLoopFlag = false;

    while ( !queue.empty() && !foundEnd && !killLoopFlag )
    {
        current = queue.front();
        this->setCellColor(this->grid_elements[current], "current");
        delay(DELAY_MS);

        queue.pop_front();
        for ( int neighbor : neighborsList(current, GRID_SIZE) )
        {
            if ( killLoopFlag == true ) { qDebug() << "stop"; break;}
            QApplication::processEvents();
            if ( std::count(visited.begin(), visited.end(), neighbor) == 0)
            {
                queue.push_back(neighbor);
                visited.push_back(neighbor);
                this->setCellColor(this->grid_elements[neighbor], "searched");
                delay(DELAY_MS);
                previous[neighbor] = current;
            }
            if ( neighbor == end ) { foundEnd = true; break; }
        }
    }
    drawShortestPath(previous, end, start);
    return previous;
}

void Gridpath::drawShortestPath(std::vector <int> prev, int end, int start)
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
    } else
    {
        qDebug() << "error message";
    }
}

void Gridpath::clearGrid (std::vector<QPushButton*> cells)
{
    this->route.clear();
    this->blockedCells.clear();
    for (auto cell : cells)
    {
        this->setCellColor(cell, "bg");
    }
}

void Gridpath::delay(int msec)
{
    QEventLoop loop;
    QTimer timer;
    timer.connect(&timer, &QTimer::timeout,
                  &loop, &QEventLoop::quit);
    timer.start(msec);
    loop.exec();
}

std::list <int> Gridpath::neighborsList(int cell, int const GRID_SIZE)
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

bool Gridpath::eventFilter(QObject* obj, QEvent *event)
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

void Gridpath::setCellColor(QPushButton *cell, QString type)
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
