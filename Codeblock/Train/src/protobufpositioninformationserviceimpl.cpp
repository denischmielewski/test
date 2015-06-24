#include <protobufpositioninformationserviceimpl.hpp>

using namespace std;
using namespace google::protobuf;

static startup_severity_channel_logger_mt * logger;

PositionInformationImpl::PositionInformationImpl(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions )
{
    softwareConfig_ = config;
    trainsSessions_ = trainsSessions;
    startup_severity_channel_logger_mt& lg = server_comm_logger::get();
    logger = &lg;
}

void PositionInformationImpl::PositionInformation(  RpcController *                             controller,
                                                    const PositionInformationTransmit *         request,
                                                    PositionInformationReceive *                response,
                                                    Closure *                                   done)
{
    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    BOOST_LOG_SEV(*logger, message) << "position received from " << rcfSession.getClientAddress().string();

    // Fill in the response.
    response->set_servername("Position received OK !");
    // Send response back to the client.
    done->Run();

    //Retrieve session info and store them in unordered_map
    TrainSession & trainSession = (*trainsSessions_)[request->trainid()];
    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();
    if(traincommsession.TryLockCommSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
    {
        traincommsession.SetSessionActive();
        traincommsession.SetIpAddress(request->trainid());
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
        BOOST_LOG_SEV(*logger, warning) << "Train Communication Session Lock failed !!!";
    }
}
