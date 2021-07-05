#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "qcustomplot.h"
#include <QSizePolicy>
#include <QPen>
#include <QColor>
#include <QMap>

#include <QSharedPointer>

typedef struct
{ //Each Graph has some information attached to it
    bool showVal; //does the user want to see the values
    int valueIndex; //which index holds the value-Graph
    bool showFirstOrderDerivative; //does the user want to see the first order derivative
    int firstOrderIndex; //which index holds the graph with the first order derivative
    bool showScndOrderDerivative; //does the user want to see the second order derivative
    int scndOrderIndex; // index for second order derivative
} Preferences;

#define INIT_PREFERENCES(P) Preferences P = {.showVal = true, .valueIndex = 0, .showFirstOrderDerivative = false, .firstOrderIndex = 0, .showScndOrderDerivative = false, .scndOrderIndex = 0}


namespace Ui {
class Plot;
}

class Plot : public QWidget
{
Q_OBJECT
private:
    Ui::Plot *ui_;
    QCustomPlot* customPlot_;
    QTimer dataTimer_;
    QStatusBar* statusBar_;
    QTreeWidget* dataFieldTreeWidget_;
    QVBoxLayout* vlayout_;

    QMap<QString, Preferences> graphPreferenceMap_;
    int loopCounter_ = 0;

    double previousKey_;




public:
    explicit Plot(QWidget *parent = nullptr);

private:

    double approximateFirstOrderDerivative(QSharedPointer< QCPGraphDataContainer > data);
    //QSharedPointer <QCPGraphDataContainer> approximateSecondOrderDerivative(QSharedPointer< QCPGraphDataContainer > data);


signals:

private slots:
    void realtimeDataSlot();
    void addDataField(QString name);
    void on_visibilityChanged(QTreeWidgetItem* item, int column);

};

#endif // PLOT_H

/*
class DummyDataSender;
{


public:
    DummyDataSender();

private:
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;
    double data;


    void getCurrent()


}*/
