#include "protobufsyncserver.hpp"

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;

void ProtobufSyncServerThreads(void);

ProtobufSyncServer::ProtobufSyncServer() : ProtobufSyncServerThread()
{
    //ctor
}

ProtobufSyncServer::ProtobufSyncServer(config * &conf)
{
    serverconf = conf;
}

ProtobufSyncServer::~ProtobufSyncServer()
{
    //dtor
    if(ProtobufSyncServerThread.joinable()) ProtobufSyncServerThread.join();
}

// PositionInformationService implementation.
class PositionInformationImpl : public PositionInformationService
{
public:

    // PositionInformation() method implementation.
    void PositionInformation(
        RpcController *                             controller,
        const PositionInformationTransmit *         request,
        PositionInformationReceive *                response,
        Closure *                                   done)
    {
        startup_severity_channel_logger_mt& lg = protobufsyncserver_logger_c1::get();

        BOOST_LOG_SEV(lg, notification) << "message received from train !";

        RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
        RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
        RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

        // Fill in the response.
        response->set_servername("server");

        // Send response back to the client.
        done->Run();
/*
        bool b = rcfSession.getEnableCompression();
        cout << "compression enable ?: " << b << endl;
        const RCF::RemoteAddress & ipaddress = rcfSession.getClientAddress();
        std::cout << "remote address: " << ipaddress.string() << std::endl;
        time_t timeraw = rcfSession.getConnectedAtTime();
        std::cout << "connected at time: " << ctime(&timeraw);
        RCF::SessionState & sessionState = rcfSession.getSessionState();
        b = sessionState.isConnected();
        cout << "Session active ?: " << b << endl;
        std::cout << "request user data: " << rcfSession.getRequestUserData() << std::endl;
        std::cout << "connection duration: " << pprotoSession->getConnectionDuration() << std::endl;
        std::cout << "remote call count: " << pprotoSession->getRemoteCallCount() << std::endl;
        std::cout << "total bytes received: " << pprotoSession->getTotalBytesReceived() << std::endl;
        std::cout << "total bytes sent: " << pprotoSession->getTotalBytesSent() << std::endl;
        std::cout << "transport protocol : " << pprotoSession->getTransportProtocol() << std::endl;
        std::cout << "transport type : " << pprotoSession->getTransportType() << std::endl;
        std::cout << "session cancelled ? " << pprotoSession->IsCanceled() << std::endl;
*/
    }
};

void ProtobufSyncServer::ProtobufSyncServerThreadsCode(void)   //RCF and protobuf will start other threads hence the thread(s)
{
    startup_severity_channel_logger_mt& lg = protobufsyncserver_logger_c1::get();
    std::chrono::seconds duration(1);

    try
    {
        // Initialize RCFProto.
        RCF::init();
        RCF::enableLogging( RCF::LogToFile("/home/train/programs/real/rcfproto.log"), 4, "");
        BOOST_LOG_SEV(lg, notification) << "RCF init !";
        // Create server.
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", 50001) );
        BOOST_LOG_SEV(lg, notification) << "Protobuf server created !";
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


