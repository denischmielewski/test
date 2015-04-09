#include "traincommunicationsserver.h"

extern uint16_t g_commSessionMutexLockTimeoutMilliseconds;
int closeGUI = false;
static startup_severity_channel_logger_mt * logger;


TrainCommunicationsServer::TrainCommunicationsServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    serverconf = conf;
    startup_severity_channel_logger_mt& lg = trainGUI_logger::get();
    trainsSessions_ = trainsSessions;
    logger = &lg;
}

TrainCommunicationsServer::~TrainCommunicationsServer()
{
    //dtor
    //if(TrainCommunicationsServerThread.joinable()) TrainCommunicationsServerThread.join();
}

void TrainCommunicationsServer::run(void)
{

    try
    {
        // Initialize RCFProto.
        RCF::init();
        BOOST_LOG_SEV(*logger, notification) << "TrainGUI RCF init !";
        // Create server.
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", std::stoi(serverconf->gui_listener_port_)));
        BOOST_LOG_SEV(*logger, notification) << "trainGUI Protobuf server created ! listen on 0.0.0.0, port " << std::stoi(serverconf->gui_listener_port_);
        // Bind Protobuf services.
        PositionInformationImpl positionInformationImpl;
        server.bindService(positionInformationImpl);
        SetOperationModeImpl setOperationModeInformationImpl(serverconf, trainsSessions_);
        server.bindService(setOperationModeInformationImpl);

        BOOST_LOG_SEV(*logger, notification) << "TrainGUI RCF proto server declared and service bind !";

        // Start the server.
        server.start();
        BOOST_LOG_SEV(*logger, notification) << "TrainGUI RCF proto server started !";

        QTimer * timer = new QTimer;
        connect(timer, &QTimer::timeout, this, &TrainCommunicationsServer::onThreadTimerShot);
        timer->start(serverconf->ThreadsLogNotificationFrequencyMilliseconds_);
/*        while(!closeGUI)
        {
            this->thread()->msleep(serverconf->communicationThreadsSleepDurationMilliseconds_);
            BOOST_LOG_SEV(*logger, notification) << "remaining timer time" << timer->remainingTime();
        }
        if(closeGUI) BOOST_LOG_SEV(*logger, notification) << "Closing GUI, terminating TrainCommunicationsServerThreads";
*/
        exec();
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}

void TrainCommunicationsServer::sendModeAutomatic()
{
    //QMessageBox::information(0, "..", "pushed from automatic button !",0,0);
    BOOST_LOG_SEV(*logger, notification) << "Automatic mode button pushed !";
}

void TrainCommunicationsServer::sendModeManual()
{
    //QMessageBox::information(0, "..", "pushed from manual button !",0,0);
    BOOST_LOG_SEV(*logger, notification) << "Manual mode button pushed !";
}

void TrainCommunicationsServer::onCloseTrainGUI()
{
    closeGUI = true;
    //QMessageBox::information(0, "..", "in communication ... !",0,0);
    BOOST_LOG_SEV(*logger, notification) << "Close window button pushed !";
    this->exit();
    if(this->wait(serverconf->ThreadsExitTimeoutMilliseconds_) == false)    BOOST_LOG_SEV(*logger, warning) << "TrainCommunicationsServer Thread did not finished in allocated time !";;
}

void TrainCommunicationsServer::onThreadTimerShot(void)
{
    BOOST_LOG_SEV(*logger, notification) << "hello from trainGUI server thread";
}
