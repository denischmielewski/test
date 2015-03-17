#include <iostream>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

using namespace std;
using namespace google::protobuf;

// PositionInformationService implementation.
class PositionInformationImpl : public PositionInformationService
{
public:

    // PositionInformation() method implementation.
    void PositionInformation(
        RpcController *                             controller,
        const PositionInformationTransmit *         request,
        PositionInformationReceive *                response,
        Closure *                                   done)
    {
        RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
        RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
        RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();
        // request parameters
        cout << request->trainid() << std::endl;
        cout << request->position() << std::endl;
        cout << request->status() << std::endl;
        // Fill in the response.
        response->set_servername("server");

        // Send response back to the client.
        done->Run();

        //rcfSession.setOnDestroyCallback(OnDestroyRCFSessionCallback);
        //rcfSession.OnDestroyCallback = OnDestroyRCFSessionCallback;
        bool b = rcfSession.getEnableCompression();
        cout << "compression enable ?: " << b << endl;
        const RCF::RemoteAddress & ipaddress = rcfSession.getClientAddress();
        std::cout << "remote address: " << ipaddress.string() << std::endl;
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

};

int main()
{
    cout << "Hello world!" << endl;

    try
    {
        // Initialize RCFProto.
        RCF::init();
        cout << "RCF init !" << endl;
        // Create server.
        RCF::RcfProtoServer server( RCF::TcpEndpoint("0.0.0.0", 50001) );

        // Bind Protobuf service.
        PositionInformationImpl positionInformationImpl;
        server.bindService(positionInformationImpl);
        cout << "RCF proto server declared and service bind !" << endl;


        // Start the server.
        server.start();
        cout << "RCF proto server started !" << endl;
        // Wait for clients.
        std::cout << "Press Enter to exit." << std::endl;
        std::cin.get();

    }
    catch(const RCF::Exception & e)
    {
        std::cout << "RCF::Exception: " << e.getErrorString() << std::endl;
        return 1;
    }

    return 0;
}
