#include "datademonstrator.h"
#include "ui_datademonstrator.h"

DataDemonstrator::DataDemonstrator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataDemonstrator)
{
    ui->setupUi(this);
}

DataDemonstrator::~DataDemonstrator()
{
    delete ui;
}
