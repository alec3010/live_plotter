#include "plotter.h"

#include "ui_plotter.h"
#include <qpushbutton.h>

Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plotter)
{
    ui->setupUi(this);
    connect(this->ui->addPlotLineButton, SIGNAL(clicked()), this, SLOT(addPlotWidget()));

}

Plotter::~Plotter()
{
    delete ui;
}

void Plotter::addPlotWidget()
{
    Plot* newPlot = new Plot(this);
    ui->vLayout->addWidget(newPlot);
    plots_.push_back(newPlot);

}
