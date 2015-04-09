#include "traincontrols.h"
#include "ui_traincontrols.h"
#include <QMessageBox>
#include "utils.hpp"
#include <QProcess>

class TrainCommunicationsServer const * traincomm;
class config const * trainconf;
static startup_severity_channel_logger_mt * logger;

TrainControls::TrainControls(QWidget *parent, config const * trainGUI_configuration, TrainCommunicationsServer const * t) :
    QMainWindow(parent),
    ui(new Ui::TrainControls)
{
    ui->setupUi(this);
    traincomm = t;
    trainconf = trainGUI_configuration;
    startup_severity_channel_logger_mt& lg = trainGUI_logger::get();
    logger = &lg;
    bool connection;


//    t = new TrainCommunicationsServer(trainGUI_configuration);
//    t->start();

    //connection = connect(ui->Automatic, SIGNAL(clicked()), &t, SLOT(sendMode()));
    connection = connect(ui->Automatic, &QRadioButton::clicked, t, &TrainCommunicationsServer::sendModeAutomatic);
    connection = connect(ui->Manual, &QRadioButton::clicked, t, &TrainCommunicationsServer::sendModeManual);
/*  for Debug
    if(connection == true)
        QMessageBox::information(0, "..", "connect succeeded !",0,0);
    else
        QMessageBox::information(0, "..", "connect failed !",0,0);
*/
    connection = connect(this, &TrainControls::myclose, t, &TrainCommunicationsServer::onCloseTrainGUI);
/*  for debug
    if(connection == true)
        QMessageBox::information(0, "..", "connect succeeded !",0,0);
    else
        QMessageBox::information(0, "..", "connect failed !",0,0);
*/
}

TrainControls::~TrainControls()
{
//    t->wait();
//    delete (t);
    delete ui;
}



void TrainControls::on_Manual_clicked()
{
    //QMessageBox::information(0, "..", "I have been pushed 0001 !",0,0);
}

void TrainControls::on_Automatic_clicked(bool checked)
{
    if(checked == true)
    {
        //QMessageBox::information(0, "..", "automatic button true !",0,0);
    }
    else
    {
        //QMessageBox::information(0, "..", "automatic button false !",0,0);
    }
}

void TrainControls::closeEvent(QCloseEvent * evt)
{
    //QMessageBox::information(0, "..", "close event ...",0,0);
    emit myclose();
    evt->accept();
}



void TrainControls::on_PBStopTrainSw_clicked()
{
    //First check if Train software is running
    string command = "pgrep -x Train";
    string ls = GetStdoutFromCommand(command, trainconf->linuxSysCallBufferSize_);
    //preg return empty if process not found

    if(ls.empty())
    {
        BOOST_LOG_SEV(*logger, notification) << "PB StopTrainSw pushed but Sw not running !";
    }
    else
    {
        BOOST_LOG_SEV(*logger, notification) << "PB StopTrainSw pushed, closing Sw ...";
        ls = GetStdoutFromCommand("pkill -x Train", trainconf->linuxSysCallBufferSize_);
        this->thread()->msleep(trainconf->linuxSysCallSleepDurationMilliseconds_);
        ls = GetStdoutFromCommand("pgrep -x Train", trainconf->linuxSysCallBufferSize_);
        if(ls.empty() != true)
        {
            BOOST_LOG_SEV(*logger, critical) << "IMPOSSIBLE TO STOP Train main Sw !!!";
            QMessageBox::critical(0, "Software FAILURE !", "IMPOSSIBLE TO STOP Train main Sw !!!",0,0);
        }
    }
}

void TrainControls::on_PBStartTrainSw_clicked()
{
    //First check if Train software is running
    string command = "pgrep -x Train";
    string ls = GetStdoutFromCommand(command, trainconf->linuxSysCallBufferSize_);
    //preg return empty if process not found
    if(ls.empty() == false)
    {
        //train Sw already running
        BOOST_LOG_SEV(*logger, notification) << "PushButton StartTrainSw pushed but Sw already running !";
    }
    else
    {
        //train Sw is not running !
        BOOST_LOG_SEV(*logger, notification) << "Pushbutton StartTrainSw pushed, starting Sw In another process...";
        QProcess *process = new QProcess(this);
        QString file = "/home/train/programs/real/Train";
        process->startDetached(file);
        this->thread()->msleep(trainconf->linuxSysCallSleepDurationMilliseconds_);
        ls = GetStdoutFromCommand("pgrep -x Train", trainconf->linuxSysCallBufferSize_);
        if(ls.empty() == true)
        {
            BOOST_LOG_SEV(*logger, critical) << "IMPOSSIBLE TO START Train main Sw !!!";
            QMessageBox::critical(0, "Software FAILURE !", "IMPOSSIBLE TO START Train main Sw !!!",0,0);
        }
    }
}
