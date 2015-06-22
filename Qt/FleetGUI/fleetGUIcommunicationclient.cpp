#include "fleetGUIcommunicationclient.hpp"

using namespace std;
using namespace google::protobuf;

static int closeGUI = false;
static startup_severity_channel_logger_mt * logger;

FleetGUICommunicationClient::FleetGUICommunicationClient(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    clientconf = conf;
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    trainsSessions_ = trainsSessions;
    logger = &lg;
}

FleetGUICommunicationClient::~FleetGUICommunicationClient()
{
    BOOST_LOG_SEV(*logger, notification) << "enter DESTRUCTOR FleetGUICommunicationClient class";
    BOOST_LOG_SEV(*logger, notification) << "leave DESTRUCTOR FleetGUICommunicationClient class";
}

void FleetGUICommunicationClient::FleetGUICommunicationClient::run(void)
{
    RCF::RcfProtoChannel * chan = nullptr;
    GetFleetService::Stub * stub = nullptr;

    try
    {
        //RCF::RcfProtoChannel channel( RCF::TcpEndpoint(clientconf->main_ipaddress_, std::stoi(clientconf->main_listener_port_)));
        chan = new RCF::RcfProtoChannel( RCF::TcpEndpoint(clientconf->server1_ipaddress_, std::stoi(clientconf->server1_listener_port_)));
        BOOST_LOG_SEV(*logger, notification) << "Message to Server1 will be sent to : " << clientconf->server1_ipaddress_ << " on port : " << clientconf->server1_listener_port_;
        // connect timeout in ms.
       chan->setConnectTimeoutMs(clientconf->TCPIP_Connection_Timeout_);
       // remote call timeout in ms.
       chan->setRemoteCallTimeoutMs(clientconf->TCPIP_Reply_Timeout_);

       //SetOperationModeService::Stub stub(&channel);
       stub = new GetFleetService::Stub(chan);
       getFleetServiceStub_ = stub;
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, critical) << "problem during RCF Proto client initialization. RCF::Exception: " << e.getErrorString() << std::endl;
        return;
    }

    QTimer * timerForClientThreadNotification = new QTimer;
    connect(timerForClientThreadNotification, &QTimer::timeout, this, &FleetGUICommunicationClient::onTimerForClientThreadNotificationShot);
    timerForClientThreadNotification->start(clientconf->ThreadsLogNotificationFrequencyMilliseconds_);

    QTimer * timerForClientToServer1GetFleet = new QTimer;
    connect(timerForClientThreadNotification, &QTimer::timeout, this, &FleetGUICommunicationClient::onTimerForClientToServer1GetFleetShot);
    timerForClientThreadNotification->start(1000);

    BOOST_LOG_SEV(*logger, notification) << "FleetGUICommunicationsClientThread event loop will start";
    exec();
    BOOST_LOG_SEV(*logger, notification) << "FleetGUICommunicationsClientThread event loop terminated";

    delete chan;
    delete stub;
}

void FleetGUICommunicationClient::onTimerForClientThreadNotificationShot(void)
{
    BOOST_LOG_SEV(*logger, notification) << "hello from FleetGUI comm client thread";
}

void FleetGUICommunicationClient::onTimerForClientToServer1GetFleetShot(void)
{
    try
    {
        getFleetCommand_.set_ipaddress(clientconf->main_ipaddress_);
        BOOST_LOG_SEV(*logger, notification) << "Send GetFleet message to Server1";
        getFleetServiceStub_->GetFleet(NULL, &getFleetCommand_, &getFleetResponse_, NULL);
        BOOST_LOG_SEV(*logger, notification)    << " Received getFleetResponse from Server1 : number of traindata = "<< getFleetResponse_.traindatalist_size();
        for (int i = 0; i < getFleetResponse_.traindatalist_size();i++)
        {
            TrainData td = getFleetResponse_.traindatalist(i);
            std::string smode;
            switch(td.mode())
            {
                case NONE: smode = "NONE";break;
                case AUTOMATIC: smode = "AUTOMATIC";break;
                case SEMIAUTOMATIC: smode = "SEMIAUTOMATIC";break;
                case MANUAL: smode = "MANUAL";break;
                default: smode = "NO_MODE_DATA";break;
            }
            BOOST_LOG_SEV(*logger, notification) << smode;
            std::string smove;
            switch(td.movement())
            {
                case STOPPED: smove = "STOPPED";break;
                case ACCELERATION: smove = "ACCELERATION";break;
                case CRUISE: smove = "CRUISE";break;
                case BRAKING: smove = "BRAKING";break;
                case APPROCHING: smove = "APPROCHING";break;
                case ARRIVED: smove = "ARRIVED";break;
                default: smove = "NO_MOVE_DATA";break;
            }
            BOOST_LOG_SEV(*logger, notification)    << " Train #" << i << " : " << td.ipaddress() << " " << td.kpposition() << " " << smode \
                                                    << " " << smove << " direction " << td.direction() << " " << td.path();
        }

    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, warning) << "problem during synchronous call to main software. RCF exception " << e.getErrorString() << std::endl;
    }
}

void FleetGUICommunicationClient::onCloseFleetGUI()
{
    //QMessageBox::information(0, "..", "onCloseFleetGUIGUI ... !",0,0);
    closeGUI = true;
    //BOOST_LOG_SEV(*logger, notification) << "Close window button event received in FleetGUICommunicationsServer thread !";
    QMessageBox *mbox = new QMessageBox;
    std::string s = "please wait " + std::to_string(clientconf->ThreadsExitTimeoutMilliseconds_/1000) + " seconds !";
    mbox->setIcon(QMessageBox::Information);
    mbox->setWindowTitle(QString::fromStdString(s));
    mbox->setText("program is closing !");
    QTimer::singleShot(clientconf->ThreadsExitTimeoutMilliseconds_, Qt::PreciseTimer, mbox, SLOT(close()));
    mbox->show();

    //this->thread()->msleep(serverconf->ThreadsExitTimeoutMilliseconds_);
    this->exit();
    if(this->wait(clientconf->ThreadsExitTimeoutMilliseconds_) == false) BOOST_LOG_SEV(*logger, warning) << "FleetGUICommunicationsServer Thread did not finished in allocated time !";
}
