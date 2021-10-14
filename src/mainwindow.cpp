#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "breadthfs.h"
#include "depthfs.h"
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
    m_BreadthFSGrid = new BreadthFS(this);
    m_BreadthFSGrid->setAttribute(Qt::WA_DeleteOnClose, true);
    m_BreadthFSGrid->show();
}

void MainWindow::on_depthfs_button_clicked()
{
    m_DepthFSGrid = new DepthFS(this);
    m_DepthFSGrid->setAttribute(Qt::WA_DeleteOnClose, true);
    m_DepthFSGrid->show();
}

