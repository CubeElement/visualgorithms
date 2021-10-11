#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gridpath.h"
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
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mGridpathModule = new Gridpath(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    mGridpathModule->show();
}
