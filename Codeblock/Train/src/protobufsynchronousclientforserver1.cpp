#include "protobufsynchronousclientforserver1.hpp"

ProtobufSynchronousClientForServer1::~ProtobufSynchronousClientForServer1()
{
    //dtor
    if(ProtobufSynchronousClientThread.joinable()) ProtobufSynchronousClientThread.join();
}

ProtobufSynchronousClientForServer1::ProtobufSynchronousClientForServer1(config * conf)
{
    clientconf = conf;
}

void ProtobufSynchronousClientForServer1::Start()
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousClientThread = std::thread(&ProtobufSynchronousClientForServer1::ProtobufSynchronousClientThreadCode,this);
}

void ProtobufSynchronousClientForServer1::Join()
{
    ProtobufSynchronousClientThread.join();
}

void ProtobufSynchronousClientForServer1::ProtobufSynchronousClientThreadCode(void)
{
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    extern volatile int g_signal_received;
    std::chrono::milliseconds sleepMilliseconds(clientconf->communicationThreadsSleepDurationMilliseconds_);

    // Create request object.
    PositionInformationTransmit request;
    request.set_trainid(clientconf->hostname_);

    // Create response object.
    PositionInformationReceive response;

    RCF::RcfProtoChannel channel( RCF::TcpEndpoint(clientconf->server1_ipaddress_, std::stoi(clientconf->server1_listener_port_)));
    BOOST_LOG_SEV(lg, notification) << "Message to server1 will be sent to : " << clientconf->server1_ipaddress_ << " on port : " << clientconf->server1_listener_port_;

    // connect timeout in ms.
    channel.setConnectTimeoutMs(clientconf->TCPIP_Connection_Timeout_);
    // remote call timeout in ms.
    channel.setRemoteCallTimeoutMs(clientconf->TCPIP_Reply_Timeout_);

    // Create service stub.
    PositionInformationService::Stub stub(&channel);

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds sendingFrequency(clientconf->TrainToServer1MessagesFrequency_);

    while(!g_signal_received)
    {

        try
        {
            // Make a synchronous remote call to server according to configured frequency
            if(sendingFrequency.count() >= clientconf->TrainToServer1MessagesFrequency_)
            {
                t0 = std::chrono::high_resolution_clock::now();
                BOOST_LOG_SEV(lg, notification) << "Sending message to Server1 : trainID = " << request.trainid() << " position = " << "TODO"/*request.position()*/ \
                                                << " status = " << "TODO"/*request.status()*/;
                stub.PositionInformation(NULL, &request, &response, NULL);

                // Process response.
                BOOST_LOG_SEV(lg, notification) << "Received response from Server1 : server name = " << response.servername();
            }
        }
        catch(const RCF::Exception & e)
        {
            BOOST_LOG_SEV(lg, warning) << "problem during synchronous call to server1 : " << e.getErrorString() << std::endl;
            BOOST_LOG_SEV(lg, warning) << "RCF::Exception: " << e.getErrorString() << std::endl;
        }
        std::this_thread::sleep_for(sleepMilliseconds);
        t1 = std::chrono::high_resolution_clock::now();
        sendingFrequency = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    }

    if(g_signal_received) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating Client for Server1 thread";
}

