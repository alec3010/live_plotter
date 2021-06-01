#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "qcustomplot.h"
#include <QSizePolicy>
#include <QPen>
#include <QColor>
#include <QMap>

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
Q_OBJECT
private:
    Ui::Plotter *ui;
    QCustomPlot* customPlot;
    QTimer dataTimer;
    QStatusBar* statusBar;
    QTreeWidget* dataFieldTreeWidget;
    QVBoxLayout* vlayout;
    QMap<QString, int> graphIndicesMap;

public:
    explicit Plotter(QWidget *parent = nullptr);

private:
    void addPlot(QString name);

signals:

private slots:
    void realtimeDataSlot();
    void on_visibilityChanged(QTreeWidgetItem* item, int column);

};

#endif // PLOTTER_H

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
