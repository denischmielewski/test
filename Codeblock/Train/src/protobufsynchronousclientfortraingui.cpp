#include "protobufsynchronousclientfortraingui.hpp"


ProtobufSynchronousClientForTrainGUI::~ProtobufSynchronousClientForTrainGUI()
{
    //dtor
    if(ProtobufSynchronousClientThread.joinable()) ProtobufSynchronousClientThread.join();
}

ProtobufSynchronousClientForTrainGUI::ProtobufSynchronousClientForTrainGUI(config * conf, std::unordered_map<std::string, TrainSession> * const trainsessions)
{
    clientconf = conf;
    trainsSessions_ = trainsessions;
}

void ProtobufSynchronousClientForTrainGUI::Start()
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousClientThread = std::thread(&ProtobufSynchronousClientForTrainGUI::ProtobufSynchronousClientThreadCode,this);
}

void ProtobufSynchronousClientForTrainGUI::Join()
{
    // This will start the thread. Notice move semantics!
    ProtobufSynchronousClientThread.join();
}

void ProtobufSynchronousClientForTrainGUI::ProtobufSynchronousClientThreadCode(void)
{
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    extern volatile int g_signal_received;
    std::chrono::milliseconds sleepMilliseconds(clientconf->communicationThreadsSleepDurationMilliseconds_);

    // Create request object.
    PositionInformationTransmit request;

    // Create response object.
    PositionInformationReceive response;

    RCF::RcfProtoChannel channel( RCF::TcpEndpoint(clientconf->gui_ipaddress_, std::stoi(clientconf->gui_listener_port_)));
    BOOST_LOG_SEV(lg, notification) << "Message to GUI will be sent to : " << clientconf->gui_ipaddress_ << " on port : " << clientconf->gui_listener_port_;

    // connect timeout in ms.
    channel.setConnectTimeoutMs(clientconf->TCPIP_Connection_Timeout_);
    // remote call timeout in ms.
    channel.setRemoteCallTimeoutMs(clientconf->TCPIP_Reply_Timeout_);

    // Create service stub.
    PositionInformationService::Stub stub(&channel);

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds sendingFrequency(clientconf->TrainToTrainGUIMessagesFrequency_);

    auto it = trainsSessions_->find(clientconf->main_ipaddress_);
    TrainOperationSession & trainoperationsession = (it->second).GetTrainOperationSessionRef();

    while(!g_signal_received)
    {

        try
        {
            // Make a synchronous remote call to server according to configured frequency
            if(sendingFrequency.count() >= clientconf->TrainToTrainGUIMessagesFrequency_)
            {
                if(trainoperationsession.TryLockCommSessionMutexFor(clientconf->operationSessionMutexLockTimeoutMilliseconds_))
                {

                    request.set_trainid(clientconf->hostname_);
                    request.set_kpposition(trainoperationsession.GetKpPosition());
                    request.set_mode(trainoperationsession.GetMode());
                    request.set_direction(trainoperationsession.GetDirection());
                    request.set_movement(trainoperationsession.GetCurrentSegmentMoveStatus());
                    request.set_path(trainoperationsession.GetPath());
                    trainoperationsession.UnlockCommSessionMutex();
                }
                else
                {
                    BOOST_LOG_SEV(lg, warning) << "Train Operation Session Lock failed !!!";
                }

                t0 = std::chrono::high_resolution_clock::now();
                //make log user-friendly
                std::string smode;
                switch(trainoperationsession.GetMode())
                {
                    case NONE: smode = "NONE";break;
                    case AUTOMATIC: smode = "AUTOMATIC";break;
                    case SEMIAUTOMATIC: smode = "SEMIAUTOMATIC";break;
                    case MANUAL: smode = "MANUAL";break;
                }
                std::string smove;
                switch(trainoperationsession.GetCurrentSegmentMoveStatus())
                {
                    case STOPPED: smove = "STOPPED";break;
                    case ACCELERATION: smove = "ACCELERATION";break;
                    case CRUISE: smove = "CRUISE";break;
                    case BRAKING: smove = "BRAKING";break;
                    case APPROCHING: smove = "APPROCHING";break;
                    case ARRIVED: smove = "ARRIVED";break;
                }
                BOOST_LOG_SEV(lg, notification) << "Sending message to GUI : trainID = " << request.trainid() << " path = " << request.path() \
                                                << " direction " << request.direction() << " position " << request.kpposition() << " " \
                                                << smode << " "<< smove;
                stub.PositionInformation(NULL, &request, &response, NULL);
                // Process response.
                BOOST_LOG_SEV(lg, notification) << "Received response from GUI : server name = " << response.servername();
            }
        }
        catch(const RCF::Exception & e)
        {
            BOOST_LOG_SEV(lg, warning) << "problem during synchronous call to GUI : " << e.getErrorString() << std::endl;
            BOOST_LOG_SEV(lg, warning) << "RCF::Exception: " << e.getErrorString() << std::endl;
        }
        std::this_thread::sleep_for(sleepMilliseconds);
        t1 = std::chrono::high_resolution_clock::now();
        sendingFrequency = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    }

    if(g_signal_received) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating Protobuf Synchronous Client for Train GUI Thread";
}
