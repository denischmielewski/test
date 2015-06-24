#include <protobufservicesimpl.hpp>
#include <unordered_map>
#include "TrainSession.hpp"

using namespace std;
using namespace google::protobuf;

extern std::unordered_map<std::string, TrainSession>    g_trains;
uint16_t g_commSessionMutexLockTimeoutMilliseconds = 111;

void PositionInformationImpl::PositionInformation(  RpcController *                             controller,
                                                    const PositionInformationTransmit *         request,
                                                    PositionInformationReceive *                response,
                                                    Closure *                                   done)
{
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();

    //make log user-friendly
    std::string smode;
    switch(request->mode())
    {
        case NONE: smode = "NONE";break;
        case AUTOMATIC: smode = "AUTOMATIC";break;
        case SEMIAUTOMATIC: smode = "SEMIAUTOMATIC";break;
        case MANUAL: smode = "MANUAL";break;
    }
    std::string smove;
    switch(request->movement())
    {
        case STOPPED: smove = "STOPPED";break;
        case ACCELERATION: smove = "ACCELERATION";break;
        case CRUISE: smove = "CRUISE";break;
        case BRAKING: smove = "BRAKING";break;
        case APPROCHING: smove = "APPROCHING";break;
        case ARRIVED: smove = "ARRIVED";break;
    }
    BOOST_LOG_SEV(lg, message) << "position received from train : " << request->trainid() << " " << request->kpposition() << " " \
                                    << smode << " " << smove << " direction " << request->direction() << " " \
                                    << request->path();

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    response->set_servername("server1");
    done->Run();

    //Retrieve session info and store them in global g_trains unordered_map
    //g_trains is keyed by train IP addresses
    std::string ipaddressmask = rcfSession.getClientAddress().string();
    std::size_t pos = ipaddressmask.find(":");      // position of "/" in string
    std::string ipaddress = ipaddressmask.substr (0,pos);

    TrainSession & trainSession = g_trains[request->trainid()];

    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();

    if(traincommsession.TryLockCommSessionMutexFor(g_commSessionMutexLockTimeoutMilliseconds))
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

    TrainOperationSession & trainoperationsession = trainSession.GetTrainOperationSessionRef();

    if(trainoperationsession.TryLockOperationSessionMutexFor(g_commSessionMutexLockTimeoutMilliseconds))
    {
        trainoperationsession.SetOperationSessionAsATrain();
        trainoperationsession.SetKpPosition(request->kpposition());
        trainoperationsession.SetDirection(request->direction());
        if(request->mode() == AUTOMATIC) {trainoperationsession.SetModeAutomatic();}
        else if (request->mode() == MANUAL) {trainoperationsession.SetModemanual();}
        else if (request->mode() == SEMIAUTOMATIC) {trainoperationsession.SetModeSemiAutomatic();}
        else
        {
            BOOST_LOG_SEV(lg, critical) << "No Valid Mode in Position Message from train " << request->trainid();
        }
        trainoperationsession.SetCurrentSegmentMoveStatus(request->movement());
        trainoperationsession.SetPath(request->path());
        trainoperationsession.UnlockOperationSessionMutex();
        trainoperationsession.SetLastTimeTrainPositionReceived(std::chrono::high_resolution_clock::now());
    }
    else
    {
        BOOST_LOG_SEV(lg, warning) << "Train Communication Session Lock failed !!!";
    }
}
