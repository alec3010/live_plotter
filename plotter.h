#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include "plot.h"

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = 0);
    ~Plotter();

private:
    Ui::Plotter *ui;
    std::vector<Plot*> plots_;

private slots:
    void addPlotWidget();
};

#endif // PLOTTER_H
