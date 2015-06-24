#include "traincontrols.h"

class TrainCommunicationsServer const * traincomm;
class config const * trainconf;
static startup_severity_channel_logger_mt * logger;

TrainControls::TrainControls(QWidget *parent, config const * trainGUI_configuration, TrainCommunicationsServer const * t, const TrainCommunicationClient *c) :
    QMainWindow(parent),
    ui(new Ui::TrainControls)
{
    ui->setupUi(this);
    traincomm = t;
    trainconf = trainGUI_configuration;
    startup_severity_channel_logger_mt& lg = trainGUI_logger::get();
    logger = &lg;

    connect(this, &TrainControls::myclose, t, &TrainCommunicationsServer::onCloseTrainGUI);
    connect(this, &TrainControls::myclose, c, &TrainCommunicationClient::onCloseTrainGUI);
    connect(ui->Automatic, &QRadioButton::clicked, c, &TrainCommunicationClient::onChangeModeToManual);
    connect(ui->Manual, &QRadioButton::clicked, c, &TrainCommunicationClient::onChangeModeToAutomatic);
}

TrainControls::~TrainControls()
{
    delete ui;
}



void TrainControls::on_Manual_clicked()
{
    BOOST_LOG_SEV(*logger, notification) << "GUI manual mode button clicked !";
}

void TrainControls::on_Automatic_clicked(bool checked)
{
    BOOST_LOG_SEV(*logger, notification) << "GUI automatic mode button clicked !";
}

void TrainControls::closeEvent(QCloseEvent * evt)
{
    BOOST_LOG_SEV(*logger, notification) << "GUI close control clicked !";
    emit myclose();
    evt->accept();
}



void TrainControls::on_PBStopTrainSw_clicked()
{
    //First check if Train software is running
    std::string command = "pgrep -x Train";
    std::string ls = GetStdoutFromCommand(command, trainconf->linuxSysCallBufferSize_);
    //pgrep return empty if process not found

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
    std::string command = "pgrep -x Train";
    std::string ls = GetStdoutFromCommand(command, trainconf->linuxSysCallBufferSize_);
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
