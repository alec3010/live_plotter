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
    dataFieldTreeWidget->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    dataFieldTreeWidget->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    dataFieldTreeWidget->header()->setSectionResizeMode(3, QHeaderView::Fixed);

    //dataFieldTreeWidget->header()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
    dataFieldTreeWidget->header()->setStretchLastSection(false);


    dataFieldTreeWidget->setColumnWidth(0, 100);
    dataFieldTreeWidget->setColumnWidth(1, 40);
    dataFieldTreeWidget->setColumnWidth(2, 40);
    dataFieldTreeWidget->setColumnWidth(3, 40);
    //dataFieldTreeWidget->setMinimumWidth(200);
    //dataFieldTreeWidget->setMaximumWidth(400);
    dataFieldTreeWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    dataFieldTreeWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    dataFieldTreeWidget->setSelectionMode(QAbstractItemView::NoSelection);


    QStringList example;
       example << "Sine" << "Cosine";
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
    dataTimer.start(20); // Interval 0 means to refresh as fast as possible



    ui->hlayout->addWidget(dataFieldTreeWidget);


    vlayout->addWidget(customPlot, 95);
    vlayout->addWidget(statusBar, 5);
    ui->hlayout->addLayout(vlayout);


}

void Plotter::addPlot(QString name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(dataFieldTreeWidget, {name});
    INIT_PREFERENCES(pref);
    graphPreferenceMap[name] = pref;

    graphPreferenceMap[name].valueIndex = customPlot->graphCount();
    customPlot->addGraph();
    QPen graphPen;
    QColor valueColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(valueColor);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot->graph()->setPen(graphPen);
    customPlot->graph()->setName(name);

    graphPreferenceMap[name].firstOrderIndex = customPlot->graphCount();
    customPlot->addGraph();
    //QPen graphPenFirstOrder;
    QColor firstDerivColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(firstDerivColor);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot->graph()->setPen(graphPen);
    customPlot->graph()->setName(name + "_1");
    customPlot->graph()->setVisible(false);

    graphPreferenceMap[name].scndOrderIndex = customPlot->graphCount();
    customPlot->addGraph();
    //QPen graphPenScndOrder;
    QColor scndDerivColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(scndDerivColor);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot->graph()->setPen(graphPen);
    customPlot->graph()->setName(name + "_2");
    customPlot->graph()->setVisible(false);


    item->setCheckState(1,Qt::Checked);
    item->setBackgroundColor(1, valueColor);
    item->setCheckState(2,Qt::Unchecked);
    item->setBackgroundColor(2, firstDerivColor);
    item->setCheckState(3,Qt::Unchecked);
    item->setBackgroundColor(3, scndDerivColor);

    dataFieldTreeWidget->adjustSize();


}


void Plotter::on_visibilityChanged(QTreeWidgetItem* item, int column)
{
    if(column == 1)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot->graph(graphPreferenceMap[item->text(0)].valueIndex)->setVisible(checked);
        graphPreferenceMap[item->text(0)].showVal=checked;
    }
    else if(column == 2)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot->graph(graphPreferenceMap[item->text(0)].firstOrderIndex)->setVisible(checked);
        graphPreferenceMap[item->text(0)].showFirstOrderDerivative=checked;
    }
    else if(column == 3)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot->graph(graphPreferenceMap[item->text(0)].scndOrderIndex)->setVisible(checked);
        graphPreferenceMap[item->text(0)].showScndOrderDerivative=checked;
    }
    else
    {
        std::cout << "columns do not match up" << std::endl;
    }

}

double Plotter::approximateFirstOrderDerivative(QSharedPointer< QCPGraphDataContainer > data)
{
    const QCPGraphData* lastDataIt = (data->constEnd() - 1);
    const QCPGraphData* thirdToLastDataIt = (data->constEnd() - 3);
    double lastValue = lastDataIt->mainValue();
    double scndToLastValue = thirdToLastDataIt->mainValue();
    double keyDiff = lastDataIt->mainKey() - thirdToLastDataIt->mainKey();




//    double firstKey =  (data->constBegin() + 0)->mainKey();
//    double secondKey = (data->constBegin() + 1)->mainKey();
//    double thirdKey = (data->constBegin() + 2)->mainKey();
//    double fourthKey = (data->constBegin() + 3)->mainKey();
//    double firstKeyDiff = secondKey - firstKey;



//    if(loopCounter< 20)
//    {
//        std::cout << "last Key: " + std::to_string(lastDataIt->mainKey()) << std::endl;
//        std::cout << "second to last Key: " + std::to_string(scndToLastDataIt->mainKey()) << std::endl;
//        std::cout << " key Diff: " + std::to_string(keyDiff) << std::endl;
//        std::cout << "----------------------------------------------" <<std::endl;
//    }

    return (lastValue - scndToLastValue)/keyDiff;


}


double Plotter::approximateSecondOrderDerivative(QSharedPointer< QCPGraphDataContainer > data)
{
    const QCPGraphData* lastDataIt = (data->constEnd() - 1);
    const QCPGraphData* secondToLastDataIt = (data->constEnd() - 2);
    const QCPGraphData* thirdToLastDataIt = (data->constEnd() -3);

    double lastValue = lastDataIt->mainValue();
    double secondToLastValue = secondToLastDataIt->mainValue();
    double thirdToLastValue = thirdToLastDataIt->mainValue();

    double lastKeyDiff = lastDataIt->mainKey() - thirdToLastDataIt->mainKey();
    //double secondToLastKeyDiff = secondToLastDataIt->mainKey() - thirdToLastDataIt->mainKey();

    return (lastValue - 2*secondToLastValue + thirdToLastValue)/(lastKeyDiff*lastKeyDiff);

}




void Plotter::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;



    QMap<QString, Preferences>::const_iterator it = graphPreferenceMap.constBegin();
    while(it!= graphPreferenceMap.constEnd())
    {

        if (it.key() == "Sine")
        {
            //std::cout << customPlot->graph(it.value().valueIndex)->name().toStdString() << std::endl;
            customPlot->graph(it.value().valueIndex)->addData(key, qSin(key));//+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
            if(loopCounter == 20)
            {
                if(it.value().showFirstOrderDerivative)
                {
                    double firstOrderValue = approximateFirstOrderDerivative(customPlot->graph(it.value().valueIndex)->data());

                    customPlot->graph(it.value().firstOrderIndex)->addData(previousKey, firstOrderValue);


                }
                if(it.value().showScndOrderDerivative)
                {
                    double secondOrderValue = approximateFirstOrderDerivative(customPlot->graph(it.value().firstOrderIndex)->data());

                    customPlot->graph(it.value().scndOrderIndex)->addData(previousKey, secondOrderValue);

                }




            }


        }
        else if (it.key() == "Cosine")
        {
            //std::cout << customPlot->graph(it.value().valueIndex)->name().toStdString()<< std::endl;

            customPlot->graph(it.value().valueIndex)->addData(key, qCos(key));//+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

            if(loopCounter == 20)
            {


                double firstOrderValue = approximateFirstOrderDerivative(customPlot->graph(it.value().valueIndex)->data());
                customPlot->graph(it.value().firstOrderIndex)->addData(previousKey, firstOrderValue);

                double secondOrderValue = approximateFirstOrderDerivative(customPlot->graph(it.value().firstOrderIndex)->data());
                customPlot->graph(it.value().scndOrderIndex)->addData(previousKey, secondOrderValue);
            }

/*
            if(it.value().showVal)
            {

            }
            if(it.value().showFirstOrderDerivative)
            {

            }
            if(it.value().showScndOrderDerivative)
            {
                //create method for second order derivative and insert
            }*/
        }

        ++it;


    }


    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {



      // rescale value (vertical) axis to fit the current data:

      customPlot->graph(0)->rescaleValueAxis(true);
      customPlot->graph(1)->rescaleValueAxis(true);
      customPlot->graph(2)->rescaleValueAxis(true);
      customPlot->graph(3)->rescaleValueAxis(true);
      customPlot->graph(4)->rescaleValueAxis(true);
      customPlot->graph(5)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    customPlot->xAxis->setRange(key, 8,  Qt::AlignRight);
    //customPlot->xAxis->setRange(0, key+1);
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

    if(loopCounter < 20)
        loopCounter++;

    previousKey = key;


}


