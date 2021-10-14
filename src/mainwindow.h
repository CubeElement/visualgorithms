#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "breadthfs.h"
#include "depthfs.h"
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
    void on_breadthfs_button_clicked();
    void on_depthfs_button_clicked();

private:
    Ui::MainWindow *ui;

private:
    BreadthFS* m_BreadthFSGrid = nullptr;
    DepthFS* m_DepthFSGrid = nullptr;
};

#endif // MAINWINDOW_H
