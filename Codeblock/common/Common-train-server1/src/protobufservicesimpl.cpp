#include <protobufservicesimpl.hpp>
#include <unordered_map>
#include "TrainSession.hpp"
#include "TrainCommSession.hpp"

using namespace std;
using namespace google::protobuf;

extern std::unordered_map<std::string, TrainSession>    g_trains;

// PositionInformation() method implementation.
void PositionInformationImpl::PositionInformation(  RpcController *                             controller,
                                                    const PositionInformationTransmit *         request,
                                                    PositionInformationReceive *                response,
                                                    Closure *                                   done)
{
    startup_severity_channel_logger_mt& lg = protobufsyncserver_logger_c1::get();

    BOOST_LOG_SEV(lg, notification) << "message received from train !";

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();
    // Fill in the response.
    response->set_servername("server");

    //Retrieve session info and store them in global g_trains unordered_map
    //g_trains is keyed by train IP addresses and TCP port
    TrainSession & trainSession = g_trains[rcfSession.getClientAddress().string()];
    //retrieve a ref to train comm session
    TrainCommSession & traincommsession = trainSession.GetTrainCommSessionRef();

    if(traincommsession.TryLockCommSessionMutexFor(100))
    {
        traincommsession.SetIpAddress(rcfSession.getClientAddress().string());
        BOOST_LOG_SEV(lg, notification) << "remote address: " << traincommsession.GetIpAddress();
        time_t timeraw = rcfSession.getConnectedAtTime();
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
/*
    bool b = rcfSession.getEnableCompression();
    cout << "compression enable ?: " << b << endl;
    const RCF::RemoteAddress & ipaddress = rcfSession.getClientAddress();
    BOOST_LOG_SEV(lg, notification) << "remote address: " << ipaddress.string();
    time_t timeraw = rcfSession.getConnectedAtTime();
    std::cout << "connected at time: " << ctime(&timeraw);
    RCF::SessionState & sessionState = rcfSession.getSessionState();
    b = sessionState.isConnected();
    cout << "Session active ?: " << b << endl;
    std::cout << "request user data: " << rcfSession.getRequestUserData() << std::endl;
    std::cout << "connection duration: " << pprotoSession->getConnectionDuration() << std::endl;
    std::cout << "remote call count: " << pprotoSession->getRemoteCallCount() << std::endl;
    std::cout << "total bytes received: " << pprotoSession->getTotalBytesReceived() << std::endl;
    std::cout << "total bytes sent: " << pprotoSession->getTotalBytesSent() << std::endl;
    std::cout << "transport protocol : " << pprotoSession->getTransportProtocol() << std::endl;
    std::cout << "transport type : " << pprotoSession->getTransportType() << std::endl;
    std::cout << "session cancelled ? " << pprotoSession->IsCanceled() << std::endl;
*/
    // Send response back to the client.
    done->Run();
}

