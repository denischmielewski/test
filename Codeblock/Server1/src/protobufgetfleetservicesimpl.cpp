#include <protobufgetfleetservicesimpl.hpp>
#include <unordered_map>
#include "TrainSession.hpp"
#include <chrono>


using namespace std;
using namespace google::protobuf;

GetFleetImpl::GetFleetImpl(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions )
{
    softwareConfig_ = config;
    trainsSessions_ = trainsSessions;
}

void GetFleetImpl::GetFleet(  RpcController *       controller,
                                                    const GetFleetCommand * request,
                                                    GetFleetResponse *      response,
                                                    Closure *               done)
{

    startup_severity_channel_logger_mt& lg = server_comm_logger::get();

    BOOST_LOG_SEV(lg, message) << "GetFleetCommand received from " << request->ipaddress();

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    BOOST_LOG_SEV(lg, message) << "start filling GetFleetResponse";
    int i = 0;
    for ( auto it = trainsSessions_->begin(); it != trainsSessions_->end(); ++it )
    {
        TrainOperationSession & trainoperationsession = (it->second).GetTrainOperationSessionRef();
        if(trainoperationsession.IsThisSessionATrain()) {i++;}
    }
    BOOST_LOG_SEV(lg, message) << "number of trains sessions : " << i;
    i = 0;
    for ( auto it = trainsSessions_->begin(); it != trainsSessions_->end(); ++it )
    {
        TrainOperationSession & trainoperationsession = (it->second).GetTrainOperationSessionRef();
        if(trainoperationsession.IsThisSessionATrain())
        {
            BOOST_LOG_SEV(lg, message) << "Train IP address :" << it->first;
            TrainData * td = response->add_traindatalist();
            td->set_ipaddress(it->first);
            if(trainoperationsession.TryLockOperationSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
            {
                td->set_kpposition(trainoperationsession.GetKpPosition());
                td->set_direction(trainoperationsession.GetDirection());
                td->set_mode(trainoperationsession.GetMode());
                std::string smode;
                switch(td->mode())
                {
                    case NONE: smode = "NONE";break;
                    case AUTOMATIC: smode = "AUTOMATIC";break;
                    case SEMIAUTOMATIC: smode = "SEMIAUTOMATIC";break;
                    case MANUAL: smode = "MANUAL";break;
                    default: smode = "NO_MODE_DATA";break;
                }
                td->set_movement(trainoperationsession.GetCurrentSegmentMoveStatus());
                std::string smove;
                switch(td->movement())
                {
                    case STOPPED: smove = "STOPPED";break;
                    case ACCELERATION: smove = "ACCELERATION";break;
                    case CRUISE: smove = "CRUISE";break;
                    case BRAKING: smove = "BRAKING";break;
                    case APPROCHING: smove = "APPROCHING";break;
                    case ARRIVED: smove = "ARRIVED";break;
                    default: smove = "NO_MOVE_DATA";break;
                }
                td->set_path(trainoperationsession.GetPath());
                std::chrono::milliseconds timeFromlastPositionMessageReception = \
                        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - \
                                trainoperationsession.GetLastTimeTrainPositionReceived());
                if(timeFromlastPositionMessageReception.count() >= softwareConfig_->TrainPositionDataValidationPeriodMilliseconds_) td->set_commstatusok(false);
                else  td->set_commstatusok(true);
                std::string cs;
                if(td->commstatusok() == true) cs = "Comm Status OK"; else cs = "Comm Status NOK";
                BOOST_LOG_SEV(lg, message) << "Response for GetFleetCommand : " <<  cs << " " \
                    << td->kpposition() << " direction = " << td->direction() << " " \
                    << smode << " " << smove << " path = " << td->path();
                trainoperationsession.UnlockOperationSessionMutex();
            }
            else
            {
                BOOST_LOG_SEV(lg, warning) << "Train Operation Session Lock failed !!!";
            }
        }
    }

    done->Run();

    std::string ipaddress = request->ipaddress();

    TrainSession & trainSession = (*trainsSessions_)[ipaddress];

    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();

    if(traincommsession.TryLockCommSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
    {
        traincommsession.SetSessionActive();
        traincommsession.SetIpAddress(ipaddress);
        time_t timeraw = rcfSession.getConnectedAtTime();
        if(timeraw != traincommsession.GetSessionConnectionTime() && traincommsession.GetSessionRemoteCallCount() != 0)  traincommsession.IncConnectionLossCount();
        traincommsession.SetSessionConnectionTime(timeraw);
        traincommsession.SetSessionConnectionDuration(pprotoSession->getConnectionDuration());
        traincommsession.SetSessionRemoteCallCount(pprotoSession->getRemoteCallCount());
        traincommsession.SetSessionTotalBytesReceived(pprotoSession->getTotalBytesReceived() );
        traincommsession.SetSessionTotalBytesSent(pprotoSession->getTotalBytesSent());
        traincommsession.UnlockCommSessionMutex();
    }
    else
    {
        BOOST_LOG_SEV(lg, warning) << "Train Communication Session Lock failed !!!";
    }
}

