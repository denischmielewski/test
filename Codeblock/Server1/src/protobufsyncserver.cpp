#include "protobufsyncserver.hpp"
#include "protobufservicesimpl.hpp"

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;



Session::Session()
{
}

Session::~Session()
{
    //dtor
}

void ProtobufSyncServerThreads(void);

ProtobufSyncServer::ProtobufSyncServer() : ProtobufSyncServerThread()
{
    //ctor
}

ProtobufSyncServer::ProtobufSyncServer(config const * conf)
{
    serverconf = conf;
}

ProtobufSyncServer::~ProtobufSyncServer()
{
    //dtor
    if(ProtobufSyncServerThread.joinable()) ProtobufSyncServerThread.join();
}

void ProtobufSyncServer::ProtobufSyncServerThreadsCode(void)   //RCF and protobuf will start other threads hence the thread(s)
{
    startup_severity_channel_logger_mt& lg = server_logger::get();
    std::chrono::seconds duration(1);

    this->session_.sessionactive_=true;

    try
    {
        // Initialize RCFProto.
        RCF::init();
        RCF::enableLogging( RCF::LogToFile("/home/train/programs/real/rcfproto.log"), 4, "");
        BOOST_LOG_SEV(lg, notification) << "RCF init !";
        // Create server.
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", std::stoi(serverconf->server1_listener_port_)));
        BOOST_LOG_SEV(lg, notification) << "Protobuf server created ! listen on 0.0.0.0, port " << std::stoi(serverconf->server1_listener_port_);
        // Bind Protobuf service.
        PositionInformationImpl positionInformationImpl;
        server.bindService(positionInformationImpl);
        BOOST_LOG_SEV(lg, notification) << "RCF proto server declared and service bind !";

        // Start the server.
        server.start();
        BOOST_LOG_SEV(lg, notification) << "RCF proto server started !";

        while(!g_signal_received)
        {
            std::this_thread::sleep_for(duration);
            BOOST_LOG_SEV(lg, notification) << "hello from protobufsyncserver thread";
        }
        if(g_signal_received) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating ProtobufSyncServerThreads";

    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(lg, critical) << "RCF::Exception: " << e.getErrorString();
        return;
    }
}

void ProtobufSyncServer::Start(void)
{
    // This will start the thread. Notice move semantics!
    ProtobufSyncServerThread = std::thread(&ProtobufSyncServer::ProtobufSyncServerThreadsCode,this);
}

void ProtobufSyncServer::Join(void)
{
    // This will start the thread. Notice move semantics!
    ProtobufSyncServerThread.join();
}


