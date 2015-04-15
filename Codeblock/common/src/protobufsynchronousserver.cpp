#include "protobufsynchronousserver.hpp"

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;

ProtobufSynchronousServer::ProtobufSynchronousServer() : ProtobufSynchronousServerThread()
{
    //ctor
}

ProtobufSynchronousServer::ProtobufSynchronousServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    serverconf = conf;
    sessions_ = trainsSessions;
}

ProtobufSynchronousServer::~ProtobufSynchronousServer()
{
    //dtor
    if(ProtobufSynchronousServerThread.joinable()) ProtobufSynchronousServerThread.join();
}

void ProtobufSynchronousServer::Start(void)
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousServerThread = std::thread(&ProtobufSynchronousServer::ProtobufSynchronousServerThreadsCode,this);
}

void ProtobufSynchronousServer::Join(void)
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousServerThread.join();
}

void ProtobufSynchronousServer::ProtobufSynchronousServerThreadsCode(void)   //RCF and protobuf will start other threads hence the thread(s)
{
    startup_severity_channel_logger_mt& lg = server_logger::get();
    std::chrono::milliseconds duration(serverconf->ThreadsLogNotificationFrequencyMilliseconds_);
    RCF::RcfProtoServer * rcfProtoServer;


    try
    {
        // Initialize RCFProto.
        RCF::init();
        BOOST_LOG_SEV(lg, notification) << "RCF init !";
        // Create server.
        try
        {
            rcfProtoServer = new RCF::RcfProtoServer( RCF::TcpEndpoint("0.0.0.0", GetRCFProtoServerListeningPort()));
            BOOST_LOG_SEV(lg, notification) << "Protobuf server created ! listen on 0.0.0.0, port " << GetRCFProtoServerListeningPort();
        }
        catch(google::protobuf::FatalException fe)
        {
            BOOST_LOG_SEV(lg, critical) << "UNSUCCESSFULL bind for PositionInformationServiceImpl!!!" << fe.what();
        }



        // Bind Protobuf service.
        if(BindProtobufServices(/*serverconf, sessions_*/*rcfProtoServer) == NO_ERROR)
        {
            BOOST_LOG_SEV(lg, notification) << "RCF proto server declared and service bind !";
        }
        else
        {
            BOOST_LOG_SEV(lg, critical) << "NOT POSSIBLE to IMPLEMENT OR BIND PROTOBUF SERVICES !!!!";
        }

        // Start the server.
        rcfProtoServer->start();
        BOOST_LOG_SEV(lg, notification) << "RCF proto server started !";

        while(!g_signal_received)
        {
            std::this_thread::sleep_for(duration);
            BOOST_LOG_SEV(lg, notification) << "hello from protobufsynchronousserver thread";
        }
        if(g_signal_received)
        {
            BOOST_LOG_SEV(lg, notification) << "Signal received, terminating ProtobufSynchronousServerThreads";
            //delete rcfProtoServer;
        }
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(lg, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}
