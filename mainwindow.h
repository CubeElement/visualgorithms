#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <list>
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
    void pressedButton();

public:
    int const GRID_SIZE;
    void createGrid(int GRID_SIZE);
    std::vector <int> findPath(int start, int end, std::list <int>);
    std::list <int> neighbors(int cell, int GRID_SIZE);
    void printShortestPath(std::vector <int> previous_list);
};



#endif // MAINWINDOW_H
