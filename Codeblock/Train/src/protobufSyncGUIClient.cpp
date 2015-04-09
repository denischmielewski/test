#include "protobufsyncGUIclient.hpp"

// TextFormat::PrintToString()
#include <google/protobuf/text_format.h>

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;

void ProtobufSyncGUIClientThreads(void);

ProtobufSyncGUIClient::ProtobufSyncGUIClient()
{
    //ctor
}

ProtobufSyncGUIClient::~ProtobufSyncGUIClient()
{
    //dtor
    if(ProtobufSyncGUIClientThread.joinable()) ProtobufSyncGUIClientThread.join();
}

ProtobufSyncGUIClient::ProtobufSyncGUIClient(config * conf)
{
    clientconf = conf;
}

void ProtobufSyncGUIClient::Start()
{
    // This will start the thread. Notice move semantics!
    ProtobufSyncGUIClientThread = std::thread(&ProtobufSyncGUIClient::ProtobufSyncGUIClientThreadsCode,this);
}

void ProtobufSyncGUIClient::Join()
{
    // This will start the thread. Notice move semantics!
    ProtobufSyncGUIClientThread.join();
}

void ProtobufSyncGUIClient::ProtobufSyncGUIClientThreadsCode(void)   //RCF and protobuf will start other threads hence the thread(s)
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

    RCF::RcfProtoChannel channel( RCF::TcpEndpoint(clientconf->server1_ipaddress_, std::stoi(clientconf->gui_listener_port_)));
    BOOST_LOG_SEV(lg, notification) << "Message to GUI will be sent to : " << clientconf->gui_ipaddress_ << " on port : " << clientconf->gui_listener_port_;
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
            BOOST_LOG_SEV(lg, notification) << "Sending message to GUI : " << strRequest;
            stub.PositionInformation(NULL, &request, &response, NULL);

            // Print out response.
            std::string strResponse;
            TextFormat::PrintToString(response, &strResponse);
            BOOST_LOG_SEV(lg, notification) << "Received response from GUI: " << strResponse;
        }
        catch(const RCF::Exception & e)
        {
            BOOST_LOG_SEV(lg, warning) << "problem during synchronous call to GUI : " << e.getErrorString() << std::endl;
            BOOST_LOG_SEV(lg, warning) << "RCF::Exception: " << e.getErrorString() << std::endl;
        }
        std::chrono::seconds duration(1);
        std::this_thread::sleep_for(duration);
    }

    if(g_signal_received) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating ProtobufSyncGUIClientThreads";
}
