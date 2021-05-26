#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
Q_OBJECT
private:
    Ui::Plotter *ui;

public:
    explicit Plotter(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // PLOTTER_H
