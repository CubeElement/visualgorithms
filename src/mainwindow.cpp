#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "breadthfs.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_breadthfs_button_clicked()
{
    mGridpathModule = new BreadthFS(this);
    mGridpathModule->setAttribute(Qt::WA_DeleteOnClose, true);
    mGridpathModule->show();
}
