#include <protobufpositioninformationserviceimpl.hpp>
#include <unordered_map>

using namespace std;
using namespace google::protobuf;

//extern std::unordered_map<std::string, TrainSession>    g_trains;
//uint16_t g_commSessionMutexLockTimeoutMilliseconds = 111;
/*
PositionInformationImpl::PositionInformationImpl(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions )
{
    softwareConfig_ = config;
    trainsSessions_ = trainsSessions;
}
*/

PositionInformationImpl::PositionInformationImpl()
{

}

// PositionInformation() method implementation.
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
    BOOST_LOG_SEV(lg, notification)     << "position received from " << ipaddress \
                                        << " TrainID = " << request->trainid() \
                                        << " Position = " << request->position() \
                                        << " Status = " << request->status();

    // Fill in the response.
    response->set_servername("Position received OK !");
    // Send response back to the client.
    done->Run();

    //Retrieve session info and store them in unordered_map
    //g_trains is keyed by train IP addresses
    TrainSession & trainSession = (*trainsSessions_)[ipaddress];

    //retrieve a ref to train comm session
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

    emit PositionReceivedFromTrain(QString::fromStdString(request->position()));
}

void PositionInformationImpl::SetSoftwareConfigPointer(config const * conf)
{
    softwareConfig_ = conf;
}

void PositionInformationImpl::SetTrainSessionpointer(std::unordered_map<std::string, TrainSession> * trainsSessions)
{
    trainsSessions_ = trainsSessions;
}
