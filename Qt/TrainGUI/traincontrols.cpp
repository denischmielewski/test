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
    guiUpdateTimer = new QTimer;
    //timer->start(clientconf->ThreadsLogNotificationFrequencyMilliseconds_);
    guiUpdateTimer->start(500);

    connect(this, &TrainControls::myclose, t, &TrainCommunicationsServer::onCloseTrainGUI);
    connect(this, &TrainControls::myclose, c, &TrainCommunicationClient::onCloseTrainGUI);
    connect(ui->Automatic, &QRadioButton::clicked, c, &TrainCommunicationClient::onChangeModeToManual);
    connect(ui->Manual, &QRadioButton::clicked, c, &TrainCommunicationClient::onChangeModeToAutomatic);
    connect(guiUpdateTimer, &QTimer::timeout, this, &TrainControls::onGuiUpdateTimerShot);
}

TrainControls::~TrainControls()
{
    guiUpdateTimer->stop();
    delete guiUpdateTimer;
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
    //No need to verify if the software is running. This is controlled by onGuiUpdateTimerShot
    BOOST_LOG_SEV(*logger, notification) << "PB StopTrainSw pushed, closing Sw ...";
    //No need to verify the syscall retrun. If something happened, onGuiUpdateTimerShot will detect it !
    GetStdoutFromCommand("pkill -x Train", trainconf->linuxSysCallBufferSize_);
    //disable both buttons. Everything will be set by onGuiUpdateTimerShot
    ui->PBStartTrainSw->setDisabled(true);
    ui->PBStopTrainSw->setDisabled(true);
}

void TrainControls::on_PBStartTrainSw_clicked()
{
    //No need to verify if the software is running. this is controlled by onGuiUpdateTimerShot
    BOOST_LOG_SEV(*logger, notification) << "Pushbutton StartTrainSw pushed, starting Sw In another process...";
    QProcess *process = new QProcess(this);
    QString file = "/home/train/programs/real/Train";
    process->startDetached(file);
    //disable both buttons. Everything will be set by onGuiUpdateTimerShot
    ui->PBStartTrainSw->setDisabled(true);
    ui->PBStopTrainSw->setDisabled(true);
}

void TrainControls::onGuiUpdateTimerShot(void)
{
    std::string ls = GetStdoutFromCommand("pgrep -x Train", trainconf->linuxSysCallBufferSize_);
    if(ls.empty() == true)
    {
        ui->PBStartTrainSw->setEnabled(true);
        ui->PBStopTrainSw->setDisabled(true);
    }
    else
    {
        ui->PBStartTrainSw->setDisabled(true);
        ui->PBStopTrainSw->setEnabled(true);
    }
}
