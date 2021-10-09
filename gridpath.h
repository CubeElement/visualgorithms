#ifndef GRIDPATH_H
#define GRIDPATH_H

#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <list>
#include <QPushButton>
#include <QObject>
#include <vector>

namespace Ui {
class Gridpath;
}

class Gridpath : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gridpath(QWidget *parent = nullptr);
    ~Gridpath();

private:
    Ui::Gridpath *ui;

public slots:
    void leftMouseButton();
    void rightMouseButton(QPushButton*);
    void killLoop() { killLoopFlag = true; }

signals:
    void stopAnimation();

private:
    int const GRID_SIZE;
    int const DELAY_MS;
    bool killLoopFlag;
    void createGrid(int GRID_SIZE);
    std::vector <int> findPathBFS(int start, int end);
    std::list <int> neighborsList(int cell, int GRID_SIZE);
    std::vector <QPushButton*> grid_elements;
    std::list <int> blockedCells;
    std::list <int> route;

protected:
    bool eventFilter(QObject*, QEvent*) override;

public:
    void setRouteDestinations(QPushButton* cell);
    void drawShortestPath(std::vector <int> previous_list, int end, int start);
    void clearGrid(std::vector<QPushButton*> grid_elements);
    void delay(int msec);
    void setCellColor(QPushButton* cell, QString type);

};

#endif // GRIDPATH_H
