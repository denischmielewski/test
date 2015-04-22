#include "protobufsynchronousclient.hpp"

using namespace std;
using namespace google::protobuf;

extern volatile int g_signal_received;

void ProtobufSyncClientThreads(void);

ProtobufSynchronousClient::ProtobufSynchronousClient()
{
    //ctor
}

ProtobufSynchronousClient::~ProtobufSynchronousClient()
{
    //dtor
    if(ProtobufSynchronousClientThread.joinable()) ProtobufSynchronousClientThread.join();
}

ProtobufSynchronousClient::ProtobufSynchronousClient(config * conf)
{
    clientconf = conf;
}

void ProtobufSynchronousClient::Start()
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousClientThread = std::thread(&ProtobufSynchronousClient::ProtobufSynchronousClientThreadCode,this);
}

void ProtobufSynchronousClient::Join()
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousClientThread.join();
}

void ProtobufSynchronousClient::ProtobufSynchronousClientThreadCode(void)
{
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    extern volatile int g_signal_received;
    std::chrono::milliseconds duration(clientconf->communicationThreadsSleepDurationMilliseconds_);

    while(!g_signal_received)
    {
        BOOST_LOG_SEV(lg, critical) << "Log from Protobuf Synchronous Client Base class ! SHOULD NOT HAPPEN !!!";
        std::this_thread::sleep_for(duration);
    }

    if(g_signal_received) BOOST_LOG_SEV(lg, notification) << "Log from Protobuf Synchronous Client Base class ! SHOULD NOT HAPPEN !!!";
}
