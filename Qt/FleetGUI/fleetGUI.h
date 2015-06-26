#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QProcess>
#include <string>
#include <QGraphicsScene>

#include "fleetGUIcommunicationsserver.h"
#include "fleetGUIcommunicationclient.hpp"
#include "config.hpp"
#include "ui_fleetGUI.h"
#include "utils.hpp"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0,
                        config const * fleetGUI_configuration = 0,
                        FleetGUICommunicationsServer const * t = 0,
                        FleetGUICommunicationClient const * c = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    Ui::MainWindow *ui;
    QGraphicsScene * myScene;

signals:
    void myclose(void);
};

#endif // MAINWINDOW_H
