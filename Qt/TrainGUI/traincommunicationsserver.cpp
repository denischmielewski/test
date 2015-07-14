#include "traincommunicationsserver.h"

using namespace std;
using namespace google::protobuf;

static startup_severity_channel_logger_mt * logger;


TrainCommunicationsServer::TrainCommunicationsServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    serverconf = conf;
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();
    trainsSessions_ = trainsSessions;
    logger = &lg;
}

TrainCommunicationsServer::~TrainCommunicationsServer()
{
}

void TrainCommunicationsServer::run(void)
{
    try
    {
        // RCFProto has been initialized in main().
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", std::stoi(serverconf->gui_listener_port_)));
        BOOST_LOG_SEV(*logger, threads) << "trainGUI Protobuf server created ! listen on 0.0.0.0, port " << std::stoi(serverconf->gui_listener_port_);
        PositionInformationImpl positionInformationImpl;
        positionInformationImpl.SetSoftwareConfigPointer(serverconf);
        positionInformationImpl.SetTrainSessionpointer(trainsSessions_);
        server.bindService(positionInformationImpl);
        connect(&positionInformationImpl, SIGNAL(PositionReceivedFromTrain(std::string)), this, SLOT(onPositionReceivedFromTrain(std::string)));
        SetOperationModeImpl setOperationModeInformationImpl(serverconf, trainsSessions_);
        server.bindService(setOperationModeInformationImpl);

        BOOST_LOG_SEV(*logger, threads) << "TrainGUI RCF proto server declared and service bind !";

        server.start();
        BOOST_LOG_SEV(*logger, threads) << "TrainGUI RCF proto server started !";

        QTimer * timer = new QTimer;
        connect(timer, &QTimer::timeout, this, &TrainCommunicationsServer::onThreadTimerShot);
        timer->start(serverconf->ThreadsLogNotificationFrequencyMilliseconds_);
        exec();
        BOOST_LOG_SEV(*logger, threads) << "TrainCommunicationsServerThread event loop terminated";
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}
void TrainCommunicationsServer::onCloseTrainGUI()
{
    BOOST_LOG_SEV(*logger, threads) << "Terminating comm server threads";
    this->exit();
}

void TrainCommunicationsServer::onThreadTimerShot(void)
{
    BOOST_LOG_SEV(*logger, threads) << "hello from trainGUI comm server thread";
}

void TrainCommunicationsServer::onPositionReceivedFromTrain(string s)
{
    BOOST_LOG_SEV(*logger, message) << "position received from train = " << s;
}
