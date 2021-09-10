#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QDebug>
#include <iostream>
#include <QObject>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->createGrid(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createGrid(int n)
{
    QWidget* grid = new QWidget();
    ui->mainlayout->addWidget(grid);

    this->resize( 30*n+20, 30*n+20);

    int counter = 0;
    for ( int i = 0; i < n; i++ )
    {
        for ( int j = 0; j < n; j++ )
        {
            QPushButton* element = new QPushButton(grid);
            element->setGeometry(i*30, j*30, 29, 29);
            element->setStyleSheet("QPushButton {"
                                   "border-style: solid;"
                                   "border-width: 1px;"
                                   "border-color: black;"
                                   "background-color: rgba(46, 204, 113, 0.4); border: none;"
                                   "}");
            element->setText(QString::number(counter));
            counter++;
            QObject::connect(element, &QPushButton::pressed,
                             this, &MainWindow::pressedButton);

        }
    }
}

void MainWindow::pressedButton()
{
    QPushButton *button = qobject_cast<QPushButton *> (sender());
    button->setStyleSheet("QPushButton {"
                                   "border-style: solid;"
                                   "border-width: 1px;"
                                   "border-color: black;"
                                   "background-color: rgba(80, 50, 113, 0.4); border: none;"
                                   "}");
}
