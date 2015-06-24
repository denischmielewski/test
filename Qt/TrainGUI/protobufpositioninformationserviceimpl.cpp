#include <protobufpositioninformationserviceimpl.hpp>
#include <unordered_map>

using namespace std;
using namespace google::protobuf;

PositionInformationImpl::PositionInformationImpl()
{

}

void PositionInformationImpl::PositionInformation(  RpcController *                             controller,
                                                    const PositionInformationTransmit *         request,
                                                    PositionInformationReceive *                response,
                                                    Closure *                                   done)
{
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    std::string ipaddressmask = rcfSession.getClientAddress().string();
    std::size_t pos = ipaddressmask.find(":");
    std::string ipaddress = ipaddressmask.substr (0,pos);
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

    response->set_servername("Position received OK !");
    // Send response back to the client.
    done->Run();

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

    emit PositionReceivedFromTrain(111.111/*request->position()*/);
}

void PositionInformationImpl::SetSoftwareConfigPointer(config const * conf)
{
    softwareConfig_ = conf;
}

void PositionInformationImpl::SetTrainSessionpointer(std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    trainsSessions_ = trainsSessions;
}
