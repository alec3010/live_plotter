#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plotter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Plotter* plotter = new Plotter(this);

    ui->layout->addWidget(plotter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
