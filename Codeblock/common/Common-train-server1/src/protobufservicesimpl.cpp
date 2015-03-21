#include <protobufservicesimpl.hpp>

using namespace std;
using namespace google::protobuf;

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

    // Send response back to the client.
    done->Run();

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

}

