#ifndef DATADEMONSTRATOR_H
#define DATADEMONSTRATOR_H

#include <QWidget>

namespace Ui {
class DataDemonstrator;
}

class DataDemonstrator : public QWidget
{
    Q_OBJECT

public:
    explicit DataDemonstrator(QWidget *parent = 0);
    ~DataDemonstrator();

private:
    Ui::DataDemonstrator *ui;
};

#endif // DATADEMONSTRATOR_H
