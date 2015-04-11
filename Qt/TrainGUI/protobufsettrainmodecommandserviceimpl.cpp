#include <protobufsettrainmodecommandserviceimpl.hpp>
#include <unordered_map>
#include "TrainSession.hpp"

using namespace std;
using namespace google::protobuf;

SetOperationModeImpl::SetOperationModeImpl(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions )
{
    softwareConfig_ = config;
    trainsSessions_ = trainsSessions;
}

// PositionInformation() method implementation.
void SetOperationModeImpl::SetOperationMode(  RpcController *                             controller,
                                                    const SetOperationModeCommand *         request,
                                                    SetOperationModeResponse *                response,
                                                    Closure *                                   done)
{

    startup_severity_channel_logger_mt& lg = comm_logger::get();

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    BOOST_LOG_SEV(lg, notification) << "position received from " << rcfSession.getClientAddress().string();

    // Fill in the response.
    response->set_previousmode("Manual");
    response->set_newmode("Automatic");

    // Send response back to the client.
    done->Run();

    //Retrieve session info and store them in global trainsSessions unordered_map
    //trainsSessions is keyed by train IP addresses
    std::string ipaddressmask = rcfSession.getClientAddress().string();
    std::size_t pos = ipaddressmask.find(":");      // position of "/" in string
    std::string ipaddress = ipaddressmask.substr (0,pos);

    TrainSession & trainSession = (*trainsSessions_)[ipaddress];

    //retrieve a ref to train comm session
    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();

    if(traincommsession.TryLockCommSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
    {
        traincommsession.SetSessionActive();
        traincommsession.SetIpAddress(ipaddress);
        BOOST_LOG_SEV(lg, notification) << "remote address: " << traincommsession.GetIpAddress();
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

