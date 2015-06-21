#include "fleetGUIcommunicationsserver.h"

using namespace std;
using namespace google::protobuf;

static int closeGUI = false;
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
    //dtor
    //if(FleetGUICommunicationsServerThread.joinable()) FleetGUICommunicationsServerThread.join();
    BOOST_LOG_SEV(*logger, notification) << "enter DESTRUCTOR FleetGUICommunicationsServer class";
    BOOST_LOG_SEV(*logger, notification) << "leave DESTRUCTOR FleetGUICommunicationsServer class";
}

void FleetGUICommunicationsServer::run(void)
{

    try
    {
        // RCFProto has been initialized in main().
        // Create server.
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", std::stoi(serverconf->gui_listener_port_)));
        BOOST_LOG_SEV(*logger, notification) << "FleetGUI Protobuf server created ! listen on 0.0.0.0, port " << std::stoi(serverconf->gui_listener_port_);
        // Bind Protobuf services.
/*        PositionInformationImpl positionInformationImpl;
        positionInformationImpl.SetSoftwareConfigPointer(serverconf);
        positionInformationImpl.SetTrainSessionpointer(trainsSessions_);
        server.bindService(positionInformationImpl);
        connect(&positionInformationImpl, SIGNAL(PositionReceivedFromTrain(std::string)), this, SLOT(onPositionReceivedFromTrain(std::string)));
        SetOperationModeImpl setOperationModeInformationImpl(serverconf, trainsSessions_);
        server.bindService(setOperationModeInformationImpl);
*/
        BOOST_LOG_SEV(*logger, notification) << "FleetGUI RCF proto server declared and service bind !";

        // Start the server.
        server.start();
        BOOST_LOG_SEV(*logger, notification) << "FleetGUI RCF proto server started !";

        QTimer * timer = new QTimer;
        connect(timer, &QTimer::timeout, this, &FleetGUICommunicationsServer::onThreadTimerShot);
        timer->start(serverconf->ThreadsLogNotificationFrequencyMilliseconds_);
        exec();
        BOOST_LOG_SEV(*logger, notification) << "FleetGUICommunicationsServerThread event loop terminated";
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}

void FleetGUICommunicationsServer::onCloseFleetGUI()
{
    //QMessageBox::information(0, "..", "onCloseFleetGUIGUI ... !",0,0);
    closeGUI = true;
    //BOOST_LOG_SEV(*logger, notification) << "Close window button event received in FleetGUICommunicationsServer thread !";
    QMessageBox *mbox = new QMessageBox;
    std::string s = "please wait " + std::to_string(serverconf->ThreadsExitTimeoutMilliseconds_/1000) + " seconds !";
    mbox->setIcon(QMessageBox::Information);
    mbox->setWindowTitle(QString::fromStdString(s));
    mbox->setText("program is closing !");
    QTimer::singleShot(serverconf->ThreadsExitTimeoutMilliseconds_, Qt::PreciseTimer, mbox, SLOT(close()));
    mbox->show();

    //this->thread()->msleep(serverconf->ThreadsExitTimeoutMilliseconds_);
    this->exit();
    if(this->wait(serverconf->ThreadsExitTimeoutMilliseconds_) == false) BOOST_LOG_SEV(*logger, warning) << "FleetGUICommunicationsServer Thread did not finished in allocated time !";
}

void FleetGUICommunicationsServer::onThreadTimerShot(void)
{
    BOOST_LOG_SEV(*logger, notification) << "hello from FleetGUI comm server thread";
}

