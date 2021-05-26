#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plotter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    plotter(new Plotter(this))
{
    ui->setupUi(this);



    ui->layout->addWidget(plotter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
