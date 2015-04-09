#ifndef TRAINCONTROLS_H
#define TRAINCONTROLS_H

#include <QMainWindow>
#include <QCloseEvent>
#include "traincommunicationsserver.h"
#include "config.hpp"


namespace Ui {
class TrainControls;
}

class TrainControls : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrainControls(QWidget *parent = 0, config const * trainGUI_configuration = 0, TrainCommunicationsServer const * t = 0);
    ~TrainControls();
    void closeEvent(QCloseEvent *);

private slots:
    void on_Manual_clicked();
    void on_Automatic_clicked(bool checked);



    void on_PBStopTrainSw_clicked();

    void on_PBStartTrainSw_clicked();

private:
    Ui::TrainControls *ui;

signals:
    void myclose(void);
};

#endif // TRAINCONTROLS_H