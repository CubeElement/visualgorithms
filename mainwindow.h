#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <list>
#include <QPushButton>
#include <QObject>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void leftMouseButton();
    void rightMouseButton(QPushButton*);
    void killLoop() { killLoopFlag = true; }

private:
    bool killLoopFlag;

signals:
    void stopAnimation();

protected:
    bool eventFilter(QObject*, QEvent*) override;

public:
    int const GRID_SIZE;
    int const DELAY_MS;
    void createGrid(int GRID_SIZE);
    std::vector <QPushButton*> grid_elements;
    std::list <int> blockedCells;
    std::list <int> route;
    void setRouteDestinations(QPushButton* cell);
    std::vector <int> findPathBFS(int start, int end);
    std::list <int> neighborsList(int cell, int GRID_SIZE);
    void drawShortestPath(std::vector <int> previous_list, int end, int start);
    void clearGrid(std::vector<QPushButton*> grid_elements);
    void delay(int msec);
    void setCellColor(QPushButton* cell, QString type);
};

#endif // MAINWINDOW_H
