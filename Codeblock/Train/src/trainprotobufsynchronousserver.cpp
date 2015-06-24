#include "trainprotobufsynchronousserver.hpp"

using namespace google::protobuf;

TrainProtobufSynchronousServer::TrainProtobufSynchronousServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    serverconf = conf;
    sessions_ = trainsSessions;
}

TrainProtobufSynchronousServer::~TrainProtobufSynchronousServer()
{
    if(TrainProtobufSynchronousServerThread.joinable()) TrainProtobufSynchronousServerThread.join();
}

void TrainProtobufSynchronousServer::Start(void)
{
    TrainProtobufSynchronousServerThread = std::thread(&TrainProtobufSynchronousServer::TrainProtobufSynchronousServerThreadCode,this);
}

void TrainProtobufSynchronousServer::Join(void)
{
    TrainProtobufSynchronousServerThread.join();
}

int TrainProtobufSynchronousServer::GetRCFProtoServerListeningPort(void)
{
    return std::stoi(serverconf->main_listener_port_);
}

void TrainProtobufSynchronousServer::TrainProtobufSynchronousServerThreadCode(void)
{
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();
    std::chrono::milliseconds duration(serverconf->communicationThreadsSleepDurationMilliseconds_);
    RCF::RcfProtoServer * rcfProtoServer;
    extern volatile int g_signal_received;

    try
    {
        rcfProtoServer = new RCF::RcfProtoServer( RCF::TcpEndpoint("0.0.0.0", GetRCFProtoServerListeningPort()));
        BOOST_LOG_SEV(lg, threads) << "Protobuf server created ! listen on 0.0.0.0, port " << GetRCFProtoServerListeningPort();
    }
    catch(google::protobuf::FatalException fe)
    {
        BOOST_LOG_SEV(lg, critical) << "UNSUCCESSFULL RCFProtoServer creation !!!" << fe.what();
        return;
    }

    // Bind Protobuf services.
    TrainProtobufPositionInformation trainProtobufPositionInformation(serverconf, sessions_);
    rcfProtoServer->bindService(trainProtobufPositionInformation);
    BOOST_LOG_SEV(lg, threads) << "Bind PositionInformationServiceImpl Successfull !!!";
    TrainProtobufSetTrainModeCommand trainProtobufSetTrainModeCommand(serverconf, sessions_);
    rcfProtoServer->bindService(trainProtobufSetTrainModeCommand);
    BOOST_LOG_SEV(lg, threads) << "Bind SetTrainModeCommandServiceImpl Successfull !!!";

    // Start the server.
    rcfProtoServer->start();
    BOOST_LOG_SEV(lg, threads) << "RCF proto server started !";

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds logFrequency(serverconf->ThreadsLogNotificationFrequencyMilliseconds_);

    while(!g_signal_received)
    {
        //log frequency as per configuration so no pollution
        if(logFrequency.count() >= serverconf->ThreadsLogNotificationFrequencyMilliseconds_)
        {
            t0 = std::chrono::high_resolution_clock::now();
            BOOST_LOG_SEV(lg, threads) << "hello from protobufsynchronousserver thread";
        }
        std::this_thread::sleep_for(duration);
        t1 = std::chrono::high_resolution_clock::now();
        logFrequency = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    }
    if(g_signal_received)
    {
        BOOST_LOG_SEV(lg, notification) << "Signal received, terminating ProtobufSynchronousServerThreads";
    }
}
