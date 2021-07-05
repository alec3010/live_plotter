#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plotter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    plotter(new Plotter(this))

{
    ui_->setupUi(this);
    ui_->layout->addWidget(plotter);

}

MainWindow::~MainWindow()
{
    delete ui_;
}



