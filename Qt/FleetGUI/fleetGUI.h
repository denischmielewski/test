#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QProcess>
#include <string>

#include "fleetGUIcommunicationsserver.h"
#include "fleetGUIcommunicationclient.hpp"
#include "config.hpp"
#include "ui_mainwindow.h"
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

private:
    Ui::MainWindow *ui;

signals:
    void myclose(void);
};

#endif // MAINWINDOW_H
