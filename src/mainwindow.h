#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gridpath.h"

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

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

private:
    Gridpath* mGridpathModule = nullptr;
};

#endif // MAINWINDOW_H