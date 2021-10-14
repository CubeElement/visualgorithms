#ifndef GRIDPATH_H
#define GRIDPATH_H

#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <list>
#include <QPushButton>
#include <QObject>
#include <vector>

namespace Ui { class Grid; }

class Grid : public QMainWindow
{
    Q_OBJECT

public:
    explicit Grid(QWidget *parent = nullptr);
    ~Grid();

private:
    Ui::Grid *ui;

public slots:
    void leftMouseButton();
    void rightMouseButton(QPushButton*);
    void killLoop() { killLoopFlag = true; }

signals:
    void stopAnimation();

protected:
    bool eventFilter(QObject*, QEvent*) override;
    int const GRID_SIZE;
    int const DELAY_MS;
    bool killLoopFlag;
    void m_createGrid(int GRID_SIZE);
    int m_getElementPos(QPushButton* elem);
    std::vector <QPushButton*> grid_elements;
    std::vector <int> blockedCells;
    std::vector <int> route;

public:
    void setRouteDestinations(QPushButton* cell);
    void drawShortestPath(std::vector <int> previous_list, int end, int start);
    void clearGrid(std::vector<QPushButton*> grid_elements);
    void delay(int msec);
    void setCellColor(QPushButton* cell, QString type);
    virtual std::vector <int> shortestPath(int start, int end) = 0;
    std::vector <int> neighborsList(int cell, int GRID_SIZE);

};

#endif // GRIDPATH_H
