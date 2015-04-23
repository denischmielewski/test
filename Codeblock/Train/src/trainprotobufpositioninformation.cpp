#include "trainprotobufpositioninformation.hpp"

using namespace std;
using namespace google::protobuf;

TrainProtobufPositionInformation::~TrainProtobufPositionInformation()
{
    //dtor
}

void TrainProtobufPositionInformation::SetResponse(PositionInformationReceive * response, Closure * done)
{
    response->set_servername("Position received OK !");
    // Send response back to the client.
    done->Run();
}


void TrainProtobufPositionInformation::UpdateSession(RCF::RcfProtoSession * pprotoSession, RCF::RcfSession & rcfSession)
{
    startup_severity_channel_logger_mt& logger = server_comm_logger::get();

    std::string ipaddressmask = rcfSession.getClientAddress().string();
    std::size_t pos = ipaddressmask.find(":");
    std::string ipaddress = ipaddressmask.substr (0,pos);

    TrainSession & trainSession = (*trainsSessions_)[ipaddress];

    //retrieve a ref to train comm session
    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();

    if(traincommsession.TryLockCommSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
    {
        traincommsession.SetSessionActive();
        traincommsession.SetIpAddress(ipaddress);
        BOOST_LOG_SEV(logger, notification) << "remote address: " << traincommsession.GetIpAddress();
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
        BOOST_LOG_SEV(logger, warning) << "Train Communication Session Lock failed !!!";
    }
}

