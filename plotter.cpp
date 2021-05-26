#include "plotter.h"
#include "ui_plotter.h"

Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plotter),
    customPlot(new QCustomPlot(this))
{
  ui->setupUi(this);
  ui->layout->addWidget(customPlot);
}
