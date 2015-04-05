#include "protobufsyncclient.hpp"

// TextFormat::PrintToString()
#include <google/protobuf/text_format.h>

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;

void ProtobufSyncClientThreads(void);

ProtobufSyncClient::ProtobufSyncClient()
{
    //ctor
}

ProtobufSyncClient::~ProtobufSyncClient()
{
    //dtor
    if(ProtobufSyncClientThread.joinable()) ProtobufSyncClientThread.join();
}

ProtobufSyncClient::ProtobufSyncClient(config * conf)
{
    clientconf = conf;
}

void ProtobufSyncClient::Start()
{
    // This will start the thread. Notice move semantics!
    ProtobufSyncClientThread = std::thread(&ProtobufSyncClient::ProtobufSyncClientThreadsCode,this);
}

void ProtobufSyncClient::Join()
{
    // This will start the thread. Notice move semantics!
    ProtobufSyncClientThread.join();
}

void ProtobufSyncClient::ProtobufSyncClientThreadsCode(void)   //RCF and protobuf will start other threads hence the thread(s)
{
    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();
    std::chrono::seconds duration(1);

    // Initialize RCFProto.
    RCF::init();

    // Create request object.
    PositionInformationTransmit request;
    request.set_trainid("TrainXXX");
    request.set_position("Line1");
    request.set_status(444);

    // Create response object.
    PositionInformationReceive response;

    RCF::RcfProtoChannel channel( RCF::TcpEndpoint(clientconf->server1_ipaddress_, std::stoi(clientconf->listener_port_)));
    BOOST_LOG_SEV(lg, notification) << "Mesage will be sent to : " << clientconf->server1_ipaddress_ << "on port : " << clientconf->listener_port_;
    // connect timeout in ms.
    channel.setConnectTimeoutMs(3000);
    // remote call timeout in ms.
    channel.setRemoteCallTimeoutMs(4000);

    // Create service stub.
    PositionInformationService::Stub stub(&channel);

    while(!g_signal_received)
    {
        try
        {
            // Make a synchronous remote call to server.
                // Print out request.
            std::string strRequest;
            TextFormat::PrintToString(request, &strRequest);
            BOOST_LOG_SEV(lg, notification) << "Sending message to server: " << strRequest;
            stub.PositionInformation(NULL, &request, &response, NULL);

            // Print out response.
            std::string strResponse;
            TextFormat::PrintToString(response, &strResponse);
            BOOST_LOG_SEV(lg, notification) << "Received response: " << strResponse;
        }
        catch(const RCF::Exception & e)
        {
            BOOST_LOG_SEV(lg, warning) << "problem during synchronous call to server: " << e.getErrorString() << std::endl;
            BOOST_LOG_SEV(lg, warning) << "RCF::Exception: " << e.getErrorString() << std::endl;
        }
        std::chrono::seconds duration(1);
        std::this_thread::sleep_for(duration);
    }

    if(g_signal_received) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating ProtobufSyncClientThreads";
}
