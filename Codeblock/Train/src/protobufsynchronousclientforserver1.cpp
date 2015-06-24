#include "protobufsynchronousclientforserver1.hpp"

ProtobufSynchronousClientForServer1::~ProtobufSynchronousClientForServer1()
{
    if(ProtobufSynchronousClientThread.joinable()) ProtobufSynchronousClientThread.join();
}

ProtobufSynchronousClientForServer1::ProtobufSynchronousClientForServer1(config * const conf, std::unordered_map<std::string, TrainSession> * const sessions)
{
    clientconf = conf;
    trainsSessions_ = sessions;
}

void ProtobufSynchronousClientForServer1::Start()
{
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

    PositionInformationTransmit request;
    request.set_trainid(clientconf->hostname_);
    PositionInformationReceive response;

    RCF::RcfProtoChannel channel( RCF::TcpEndpoint(clientconf->server1_ipaddress_, std::stoi(clientconf->server1_listener_port_)));
    BOOST_LOG_SEV(lg, threads) << "Message to server1 will be sent to : " << clientconf->server1_ipaddress_ << " on port : " << clientconf->server1_listener_port_;

    channel.setConnectTimeoutMs(clientconf->TCPIP_Connection_Timeout_);
    channel.setRemoteCallTimeoutMs(clientconf->TCPIP_Reply_Timeout_);

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

                //build request
                auto it = trainsSessions_->find(clientconf->main_ipaddress_);
                TrainOperationSession & trainoperationsession = (it->second).GetTrainOperationSessionRef();
                if(trainoperationsession.TryLockOperationSessionMutexFor(clientconf->operationSessionMutexLockTimeoutMilliseconds_))
                {

                    request.set_trainid(clientconf->main_ipaddress_);
                    request.set_kpposition(trainoperationsession.GetKpPosition());
                    request.set_mode(trainoperationsession.GetMode());
                    request.set_direction(trainoperationsession.GetDirection());
                    request.set_movement(trainoperationsession.GetCurrentSegmentMoveStatus());
                    request.set_path(trainoperationsession.GetPath());
                    trainoperationsession.UnlockOperationSessionMutex();
                }
                else
                {
                    BOOST_LOG_SEV(lg, warning) << "Train Operation Session Lock failed !!!";
                }

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
                BOOST_LOG_SEV(lg, message) << "Sending message to Server1 : trainID = " << request.trainid() << " path = " << request.path() \
                                                << " direction " << request.direction() << " position " << request.kpposition() << " "\
                                                << smode << " " << smove;
                stub.PositionInformation(NULL, &request, &response, NULL);

                // Process response.
                BOOST_LOG_SEV(lg, message) << "Received response from Server1 : server name = " << response.servername();
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

    if(g_signal_received) BOOST_LOG_SEV(lg, threads) << "Signal received, terminating Client for Server1 thread";
}

