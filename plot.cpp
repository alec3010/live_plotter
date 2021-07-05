#include "plot.h"
#include "ui_plot.h"
#include <iostream>

Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Plot),
    customPlot_(new QCustomPlot(this)),
    statusBar_(new QStatusBar(this)),
    dataFieldTreeWidget_(new QTreeWidget(this)),
    vlayout_(new QVBoxLayout())

{
    ui_->setupUi(this);
    dataFieldTreeWidget_->setColumnCount(4);
    dataFieldTreeWidget_->setHeaderLabels({"Data Field", "x", "x'", "x''"});
    dataFieldTreeWidget_->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    dataFieldTreeWidget_->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    dataFieldTreeWidget_->header()->setSectionResizeMode(3, QHeaderView::Fixed);

    //dataFieldTreeWidget_->header()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
    dataFieldTreeWidget_->header()->setStretchLastSection(false);


    dataFieldTreeWidget_->setColumnWidth(0, 100);
    dataFieldTreeWidget_->setColumnWidth(1, 40);
    dataFieldTreeWidget_->setColumnWidth(2, 40);
    dataFieldTreeWidget_->setColumnWidth(3, 40);
    //dataFieldTreeWidget_->setMinimumWidth(200);
    //dataFieldTreeWidget_->setMaximumWidth(400);
    dataFieldTreeWidget_->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    dataFieldTreeWidget_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    dataFieldTreeWidget_->setSelectionMode(QAbstractItemView::NoSelection);


    QStringList example;
       example << "Sine" << "Cosine";
    for (auto name : example)
    {
        addDataField(name);

    }


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot_->xAxis->setTicker(timeTicker);
    customPlot_->axisRect()->setupFullAxesBox();
    customPlot_->yAxis->setRange(-1.5, 1.5);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot_->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot_->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot_->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot_->yAxis2, SLOT(setRange(QCPRange)));

    // connect visibility checkboxes to plotter
    connect(dataFieldTreeWidget_, SIGNAL(itemChanged(QTreeWidgetItem*, int)),this, SLOT(on_visibilityChanged(QTreeWidgetItem*, int)));




    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer_, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer_.start(20); // Interval 0 means to refresh as fast as possible



    ui_->hlayout->addWidget(dataFieldTreeWidget_);


    vlayout_->addWidget(customPlot_, 95);
    vlayout_->addWidget(statusBar_, 5);
    ui_->hlayout->addLayout(vlayout_);


}

void Plot::addDataField(QString name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(dataFieldTreeWidget_, {name});


    INIT_PREFERENCES(pref);

    graphPreferenceMap_[name] = pref;

    graphPreferenceMap_[name].valueIndex = customPlot_->graphCount();
    customPlot_->addGraph();
    QPen graphPen;
    QColor valueColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(valueColor);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot_->graph()->setPen(graphPen);
    customPlot_->graph()->setName(name);

    graphPreferenceMap_[name].firstOrderIndex = customPlot_->graphCount();
    customPlot_->addGraph();
    //QPen graphPenFirstOrder;
    QColor firstDerivColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(firstDerivColor);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot_->graph()->setPen(graphPen);
    customPlot_->graph()->setName(name + "_1");
    customPlot_->graph()->setVisible(false);

    graphPreferenceMap_[name].scndOrderIndex = customPlot_->graphCount();
    customPlot_->addGraph();
    //QPen graphPenScndOrder;
    QColor scndDerivColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(scndDerivColor);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    customPlot_->graph()->setPen(graphPen);
    customPlot_->graph()->setName(name + "_2");
    customPlot_->graph()->setVisible(false);


    item->setCheckState(1,Qt::Checked);
    item->setBackgroundColor(1, valueColor);
    item->setCheckState(2,Qt::Unchecked);
    item->setBackgroundColor(2, firstDerivColor);
    item->setCheckState(3,Qt::Unchecked);
    item->setBackgroundColor(3, scndDerivColor);

    dataFieldTreeWidget_->adjustSize();


}


void Plot::on_visibilityChanged(QTreeWidgetItem* item, int column)
{
    if(column == 1)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot_->graph(graphPreferenceMap_[item->text(0)].valueIndex)->setVisible(checked);
        graphPreferenceMap_[item->text(0)].showVal=checked;
    }
    else if(column == 2)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot_->graph(graphPreferenceMap_[item->text(0)].firstOrderIndex)->setVisible(checked);
        graphPreferenceMap_[item->text(0)].showFirstOrderDerivative=checked;
    }
    else if(column == 3)
    {
        bool checked = (item->checkState(column) == Qt::Checked) ? true : false;
        customPlot_->graph(graphPreferenceMap_[item->text(0)].scndOrderIndex)->setVisible(checked);
        graphPreferenceMap_[item->text(0)].showScndOrderDerivative=checked;
    }
    else
    {
        std::cout << "columns do not match up" << std::endl;
    }

}

double Plot::approximateFirstOrderDerivative(QSharedPointer< QCPGraphDataContainer > data)
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



//    if(loopCounter_< 20)
//    {
//        std::cout << "last Key: " + std::to_string(lastDataIt->mainKey()) << std::endl;
//        std::cout << "second to last Key: " + std::to_string(scndToLastDataIt->mainKey()) << std::endl;
//        std::cout << " key Diff: " + std::to_string(keyDiff) << std::endl;
//        std::cout << "----------------------------------------------" <<std::endl;
//    }

    return (lastValue - scndToLastValue)/keyDiff;


}

/*
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

*/


void Plot::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;



    QMap<QString, Preferences>::const_iterator it = graphPreferenceMap_.constBegin();
    while(it!= graphPreferenceMap_.constEnd())
    {

        if (it.key() == "Sine")
        {
            //std::cout << customPlot_->graph(it.value().valueIndex)->name().toStdString() << std::endl;
            customPlot_->graph(it.value().valueIndex)->addData(key, qSin(key));//+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
            if(loopCounter_ == 20)
            {
                if(it.value().showFirstOrderDerivative)
                {
                    double firstOrderValue = approximateFirstOrderDerivative(customPlot_->graph(it.value().valueIndex)->data());

                    customPlot_->graph(it.value().firstOrderIndex)->addData(previousKey_, firstOrderValue);


                }
                if(it.value().showScndOrderDerivative)
                {
                    double secondOrderValue = approximateFirstOrderDerivative(customPlot_->graph(it.value().firstOrderIndex)->data());

                    customPlot_->graph(it.value().scndOrderIndex)->addData(previousKey_, secondOrderValue);

                }




            }


        }
        else if (it.key() == "Cosine")
        {
            //std::cout << customPlot_->graph(it.value().valueIndex)->name().toStdString()<< std::endl;

            customPlot_->graph(it.value().valueIndex)->addData(key, qCos(key));//+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

            if(loopCounter_ == 20)
            {


                double firstOrderValue = approximateFirstOrderDerivative(customPlot_->graph(it.value().valueIndex)->data());
                customPlot_->graph(it.value().firstOrderIndex)->addData(previousKey_, firstOrderValue);

                double secondOrderValue = approximateFirstOrderDerivative(customPlot_->graph(it.value().firstOrderIndex)->data());
                customPlot_->graph(it.value().scndOrderIndex)->addData(previousKey_, secondOrderValue);
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

      customPlot_->graph(0)->rescaleValueAxis(true);
      customPlot_->graph(1)->rescaleValueAxis(true);
      customPlot_->graph(2)->rescaleValueAxis(true);
      customPlot_->graph(3)->rescaleValueAxis(true);
      customPlot_->graph(4)->rescaleValueAxis(true);
      customPlot_->graph(5)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    customPlot_->xAxis->setRange(key, 8,  Qt::AlignRight);
    //customPlot_->xAxis->setRange(0, key+1);
    customPlot_->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;



    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      statusBar_->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(customPlot_->graph(0)->data()->size()+customPlot_->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;

    }


    if(loopCounter_ < 20)
        loopCounter_++;

    previousKey_ = key;


}


