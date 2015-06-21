#include "fleetGUI.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, config const * fleetGUI_configuration, FleetGUICommunicationsServer const * t, const FleetGUICommunicationClient *c) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    bool connection;

    ui->setupUi(this);
    connection = connect(this, &MainWindow::myclose, t, &FleetGUICommunicationsServer::onCloseFleetGUI);
    connection = connect(this, &MainWindow::myclose, c, &FleetGUICommunicationClient::onCloseFleetGUI);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * evt)
{
    emit myclose();
    evt->accept();
}
