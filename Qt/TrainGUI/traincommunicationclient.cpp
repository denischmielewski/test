#include "traincommunicationclient.hpp"

using namespace std;
using namespace google::protobuf;

static startup_severity_channel_logger_mt * logger;

TrainCommunicationClient::TrainCommunicationClient(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    clientconf = conf;
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    trainsSessions_ = trainsSessions;
    logger = &lg;
}

TrainCommunicationClient::~TrainCommunicationClient()
{
}

void TrainCommunicationClient::TrainCommunicationClient::run(void)
{
    RCF::RcfProtoChannel * chan = nullptr;
    SetOperationModeService::Stub * stub = nullptr;

    try
    {
        chan = new RCF::RcfProtoChannel( RCF::TcpEndpoint(clientconf->main_ipaddress_, std::stoi(clientconf->main_listener_port_)));
        BOOST_LOG_SEV(*logger, threads) << "Message to main software will be sent to : " << clientconf->main_ipaddress_ << " on port : " << clientconf->main_listener_port_;
       chan->setConnectTimeoutMs(clientconf->TCPIP_Connection_Timeout_);
       chan->setRemoteCallTimeoutMs(clientconf->TCPIP_Reply_Timeout_);

       stub = new SetOperationModeService::Stub(chan);
       setOperationModeServiceStub_ = stub;
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, critical) << "problem during RCF Proto client initialization. RCF::Exception: " << e.getErrorString() << std::endl;
        return;
    }

    QTimer * timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &TrainCommunicationClient::onThreadTimerShot);
    timer->start(clientconf->ThreadsLogNotificationFrequencyMilliseconds_);
    BOOST_LOG_SEV(*logger, threads) << "TrainCommunicationsClientThread event loop will start";
    exec();
    BOOST_LOG_SEV(*logger, threads) << "TrainCommunicationsClientThread event loop terminated";

    delete chan;
    delete stub;
}

void TrainCommunicationClient::onThreadTimerShot(void)
{
    BOOST_LOG_SEV(*logger, threads) << "hello from trainGUI comm client thread";
}

void TrainCommunicationClient::onCloseTrainGUI()
{
    BOOST_LOG_SEV(*logger, threads) << "Terminating comm client thread";
    this->exit();
}

void TrainCommunicationClient::onChangeModeToManual()
{
    try
    {
        operationModeRequest_.set_mode("Manual");
        BOOST_LOG_SEV(*logger, message) << "Sending New mode message to main software : Manual mode";
        setOperationModeServiceStub_->SetOperationMode(NULL, &operationModeRequest_, &operationModeResponse_, NULL);
        BOOST_LOG_SEV(*logger, message)    << "Received response from main software : new mode = " << operationModeResponse_.newmode() \
                                                << " previous mode = " << operationModeResponse_.previousmode();
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, warning) << "problem during synchronous call to main software. RCF exception " << e.getErrorString() << std::endl;
    }
}

void TrainCommunicationClient::onChangeModeToAutomatic()
{
    try
    {
        operationModeRequest_.set_mode("Automatic");
        BOOST_LOG_SEV(*logger, message) << "Sending New Mode message to main software : Automatic mode";
        setOperationModeServiceStub_->SetOperationMode(NULL, &operationModeRequest_, &operationModeResponse_, NULL);
        BOOST_LOG_SEV(*logger, message)    << "Received response from main software : new mode = " << operationModeResponse_.newmode()
                                                << " previous mode = " << operationModeResponse_.previousmode();
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(*logger, warning) << "problem during synchronous call to main software. RCF exception " << e.getErrorString() << std::endl;
    }
}
