#include <protobufservicesimpl.hpp>
#include <unordered_map>
#include "TrainSession.hpp"

using namespace std;
using namespace google::protobuf;

extern std::unordered_map<std::string, TrainSession>    g_trains;
uint16_t g_commSessionMutexLockTimeoutMilliseconds = 111;



// PositionInformation() method implementation.
void PositionInformationImpl::PositionInformation(  RpcController *                             controller,
                                                    const PositionInformationTransmit *         request,
                                                    PositionInformationReceive *                response,
                                                    Closure *                                   done)
{
    startup_severity_channel_logger_mt& lg = server_logger::get();

    BOOST_LOG_SEV(lg, notification) << "position received from train !";

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    // Fill in the response.
    response->set_servername("Position received OK !");
    // Send response back to the client.
    done->Run();

    //Retrieve session info and store them in global g_trains unordered_map
    //g_trains is keyed by train IP addresses
    std::string ipaddressmask = rcfSession.getClientAddress().string();
    std::size_t pos = ipaddressmask.find(":");      // position of "/" in string
    std::string ipaddress = ipaddressmask.substr (0,pos);

    TrainSession & trainSession = g_trains[ipaddress];

    //retrieve a ref to train comm session
    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();

    if(traincommsession.TryLockCommSessionMutexFor(g_commSessionMutexLockTimeoutMilliseconds))
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
