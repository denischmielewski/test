#include "protobufsyncserver.hpp"
#include "protobufservicesimpl.hpp"
#include "protobufgetfleetservicesimpl.hpp"

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;

void ProtobufSyncServerThreads(void);

ProtobufSyncServer::ProtobufSyncServer() : ProtobufSyncServerThread()
{
}

ProtobufSyncServer::ProtobufSyncServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    serverconf_ = conf;
    trainsSessions_ = trainsSessions;
}

ProtobufSyncServer::~ProtobufSyncServer()
{
    if(ProtobufSyncServerThread.joinable()) ProtobufSyncServerThread.join();
}

void ProtobufSyncServer::ProtobufSyncServerThreadsCode(void)   //RCF and protobuf will start other threads hence the thread(s)
{
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();
    std::chrono::milliseconds duration(serverconf_->communicationThreadsSleepDurationMilliseconds_);

    try
    {
        RCF::init();
        RCF::enableLogging( RCF::LogToFile("/home/train/programs/real/rcfproto.log"), 4, "");
        BOOST_LOG_SEV(lg, threads) << "RCF init !";
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", std::stoi(serverconf_->server1_listener_port_)));
        BOOST_LOG_SEV(lg, threads) << "Protobuf server created ! listen on 0.0.0.0, port " << std::stoi(serverconf_->server1_listener_port_);
        PositionInformationImpl positionInformationImpl;
        server.bindService(positionInformationImpl);
        GetFleetImpl getFleetImpl(serverconf_, trainsSessions_);
        server.bindService(getFleetImpl);
        BOOST_LOG_SEV(lg, threads) << "RCF proto server declared and all services bound !";

        server.start();
        BOOST_LOG_SEV(lg, notification) << "RCF proto server started !";

        std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds logFrequency(serverconf_->ThreadsLogNotificationFrequencyMilliseconds_);

        while(!g_signal_received)
        {
            //log frequency as per configuration so no pollution
            if(logFrequency.count() >= serverconf_->ThreadsLogNotificationFrequencyMilliseconds_)
            {
                t0 = std::chrono::high_resolution_clock::now();
                BOOST_LOG_SEV(lg, threads) << "hello from protobufsyncserver thread";
            }
            std::this_thread::sleep_for(duration);
            t1 = std::chrono::high_resolution_clock::now();
            logFrequency = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        }
        if(g_signal_received) BOOST_LOG_SEV(lg, threads) << "Signal received, terminating ProtobufSyncServerThreads";
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(lg, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}

void ProtobufSyncServer::Start(void)
{
    ProtobufSyncServerThread = std::thread(&ProtobufSyncServer::ProtobufSyncServerThreadsCode,this);
}

void ProtobufSyncServer::Join(void)
{
    ProtobufSyncServerThread.join();
}


