#include "plotter.h"
#include "ui_plotter.h"
#include <iostream>

Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plotter),
    customPlot(new QCustomPlot(this)),
    statusBar(new QStatusBar(this)),
    dataFieldTreeWidget(new QTreeWidget(this)),
    vlayout(new QVBoxLayout())

{
    ui->setupUi(this);
    dataFieldTreeWidget->setColumnCount(4);
    dataFieldTreeWidget->setHeaderLabels({"Data Field", "x", "x'", "x''"});
    //dataFieldTreeWidget->setFixedWidth(300);
    dataFieldTreeWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    dataFieldTreeWidget->setColumnWidth(0, 100);
    dataFieldTreeWidget->setColumnWidth(1, 20);
    dataFieldTreeWidget->setColumnWidth(2, 20);
    dataFieldTreeWidget->setColumnWidth(3, 20);

    QStringList example;
       example << "Sine"
             << "Cosine";
    for (auto name : example)
    {
        addPlot(name);

    }


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.5, 1.5);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // connect visibility checkboxes to plotter
    connect(dataFieldTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)),this, SLOT(on_visibilityChanged(QTreeWidgetItem*, int)));




    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(); // Interval 0 means to refresh as fast as possible



    ui->hlayout->addWidget(dataFieldTreeWidget);


    vlayout->addWidget(customPlot, 95);
    vlayout->addWidget(statusBar, 5);
    ui->hlayout->addLayout(vlayout);

}

void Plotter::addPlot(QString name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(dataFieldTreeWidget, {name});
    item->setCheckState(1,Qt::Checked);
    item->setCheckState(2,Qt::Unchecked);
    item->setCheckState(3,Qt::Unchecked);

    graphIndicesMap[name] = customPlot->graphCount();
    customPlot->addGraph();
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot->graph()->setPen(graphPen);
    customPlot->graph()->setName(name);
}

void Plotter::on_visibilityChanged(QTreeWidgetItem* item, int column)
{
    if(column == 1)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot->graph(graphIndicesMap[item->text(0)])->setVisible(checked);
    }
    else if(column == 2)
    {
        //do nothing for now. later first derivative
    }
    else if(column == 3)
    {
        //do nothing for now. later second derivative
    }
    else
    {
        std::cout << "columns do not match up" << std::endl;
    }

}


void Plotter::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
      customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

      // rescale value (vertical) axis to fit the current data:
      customPlot->graph(0)->rescaleValueAxis();
      customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    //customPlot->xAxis->setRange(key, 8,  Qt::AlignRight);
    customPlot->xAxis->setRange(0, key+1);
    customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;

    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(customPlot->graph(0)->data()->size()+customPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}


