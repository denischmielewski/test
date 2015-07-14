#include "fleetGUIcommunicationclient.hpp"

using namespace std;
using namespace google::protobuf;

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
}

void FleetGUICommunicationClient::FleetGUICommunicationClient::run(void)
{
    RCF::RcfProtoChannel * chan = nullptr;
    GetFleetService::Stub * stub = nullptr;

    try
    {
        chan = new RCF::RcfProtoChannel( RCF::TcpEndpoint(clientconf->server1_ipaddress_, std::stoi(clientconf->server1_listener_port_)));
        BOOST_LOG_SEV(*logger, threads) << "Message to Server1 will be sent to : " << clientconf->server1_ipaddress_ << " on port : " << clientconf->server1_listener_port_;
       chan->setConnectTimeoutMs(clientconf->TCPIP_Connection_Timeout_);
       chan->setRemoteCallTimeoutMs(clientconf->TCPIP_Reply_Timeout_);

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
    timerForClientThreadNotification->start(clientconf->FleetGUIToServer1MessagesFrequencyMilliseconds_);

    BOOST_LOG_SEV(*logger, threads) << "FleetGUICommunicationsClientThread event loop will start";
    exec();
    BOOST_LOG_SEV(*logger, threads) << "FleetGUICommunicationsClientThread event loop terminated";

    delete chan;
    delete stub;
}

void FleetGUICommunicationClient::onTimerForClientThreadNotificationShot(void)
{
    BOOST_LOG_SEV(*logger, threads) << "hello from FleetGUI comm client thread";
}

void FleetGUICommunicationClient::onTimerForClientToServer1GetFleetShot(void)
{
    try
    {
        getFleetCommand_.set_ipaddress(clientconf->main_ipaddress_);
        BOOST_LOG_SEV(*logger, message) << "Send GetFleet message to Server1 : size = " << getFleetCommand_.ByteSize() << " cached size = " << getFleetCommand_.GetCachedSize();
        getFleetServiceStub_->GetFleet(NULL, &getFleetCommand_, &getFleetResponse_, NULL);
        BOOST_LOG_SEV(*logger, message)    << "Received getFleetResponse from Server1 : size = " << getFleetResponse_.ByteSize() \
                                           << " number of traindata = "<< getFleetResponse_.traindatalist_size() << " cached size = " << getFleetResponse_.GetCachedSize();
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
            std::string cs;
            if(td.commstatusok() == true) cs = "Comm Status OK"; else cs = "Comm Status NOK";
            BOOST_LOG_SEV(*logger, message)    << "Train #" << i+1 << " " << cs << " " \
                                                    << " : " << td.ipaddress() << " " << td.kpposition() << " " << smode \
                                                    << " " << smove << " direction " << td.direction() << " " << td.path();

            TrainSession & trainSession = (*trainsSessions_)[td.ipaddress()];
            TrainOperationSession & trainoperationsession = trainSession.GetTrainOperationSessionRef();

            if(trainoperationsession.TryLockOperationSessionMutexFor(clientconf->commSessionMutexLockTimeoutMilliseconds_))
            {
                trainoperationsession.SetOperationSessionAsATrain();
                trainoperationsession.SetKpPosition(td.kpposition());
                trainoperationsession.SetDirection(td.direction());
                if(td.mode() == AUTOMATIC) {trainoperationsession.SetModeAutomatic();}
                else if (td.mode() == MANUAL) {trainoperationsession.SetModemanual();}
                else if (td.mode() == SEMIAUTOMATIC) {trainoperationsession.SetModeSemiAutomatic();}
                else
                {
                    BOOST_LOG_SEV(*logger, critical) << "No Valid Mode in Position Message from train " << td.ipaddress();
                }
                trainoperationsession.SetCurrentSegmentMoveStatus(td.movement());
                trainoperationsession.SetPath(td.path());
                if(td.commstatusok() == true) trainoperationsession.SetLastTimeTrainPositionReceived(std::chrono::high_resolution_clock::now());
                trainoperationsession.UnlockOperationSessionMutex();
            }
            else
            {
                BOOST_LOG_SEV(*logger, warning) << "Train Communication Session Lock failed !!!";
            }
        }
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, warning) << "problem during synchronous call to main software. RCF exception " << e.getErrorString() << std::endl;
    }
}

void FleetGUICommunicationClient::onCloseFleetGUI()
{
    BOOST_LOG_SEV(*logger, threads) << "Terminating comm client thread";
    this->exit();
}
