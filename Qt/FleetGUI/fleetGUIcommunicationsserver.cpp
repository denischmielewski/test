#include "fleetGUIcommunicationsserver.h"

using namespace std;
using namespace google::protobuf;

static startup_severity_channel_logger_mt * logger;


FleetGUICommunicationsServer::FleetGUICommunicationsServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    serverconf = conf;
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();
    trainsSessions_ = trainsSessions;
    logger = &lg;
}

FleetGUICommunicationsServer::~FleetGUICommunicationsServer()
{
}

void FleetGUICommunicationsServer::run(void)
{
    try
    {
        // RCFProto has been initialized in main().
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", std::stoi(serverconf->gui_listener_port_)));
        BOOST_LOG_SEV(*logger, threads) << "FleetGUI Protobuf server created ! listen on 0.0.0.0, port " << std::stoi(serverconf->gui_listener_port_);
        // Bind Protobuf services.
        //nothing to bind for now, will come later
        BOOST_LOG_SEV(*logger, threads) << "FleetGUI RCF proto server declared and service bind !";
        server.start();
        BOOST_LOG_SEV(*logger, threads) << "FleetGUI RCF proto server started !";

        QTimer * timer = new QTimer;
        connect(timer, &QTimer::timeout, this, &FleetGUICommunicationsServer::onThreadTimerShot);
        timer->start(serverconf->ThreadsLogNotificationFrequencyMilliseconds_);
        exec();
        BOOST_LOG_SEV(*logger, threads) << "FleetGUICommunicationsServerThread event loop terminated";
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}

void FleetGUICommunicationsServer::onCloseFleetGUI()
{
    BOOST_LOG_SEV(*logger, threads) << "Terminating comm server threads";
    this->exit();
}

void FleetGUICommunicationsServer::onThreadTimerShot(void)
{
    BOOST_LOG_SEV(*logger, threads) << "hello from FleetGUI comm server thread";
}

