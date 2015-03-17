#include <iostream>
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
// Include protoc-generated header.
#include "TestRCFProto.pb.h"

// TextFormat::PrintToString()
#include <google/protobuf/text_format.h>

using namespace google::protobuf;
using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    // Initialize RCFProto.
    RCF::init();

    // Create request object.
    PositionInformationTransmit request;
    request.set_trainid("TrainXXX");
    request.set_position("Line1");
    request.set_status(444);

    // Create response object.
    PositionInformationReceive response;

    // Create channel.
    RCF::RcfProtoChannel channel( RCF::TcpEndpoint("192.168.1.106", 50001) );

    // connect timeout in ms.
    channel.setConnectTimeoutMs(3000);
    // remote call timeout in ms.
    channel.setRemoteCallTimeoutMs(4000);



    for(int i = 0 ; i < 10 ; i++)
    {
        try
        {
            channel.connect();
        }
        catch(const RCF::Exception & e)
        {
            std::cout << "RCF::Exception: " << e.getErrorString() << std::endl;
            //std::chrono::seconds duration(3);
            //std::this_thread::sleep_for(duration);

            if(i == 9)
            {
                std::cout << "connection retry exceeded !" << std::endl;
                return 1;
            }
        }
    }


    // Create service stub.
    PositionInformationService::Stub stub(&channel);

    // Print out request.
    std::string strRequest;
    TextFormat::PrintToString(request, &strRequest);
    std::cout << "Sending request: " << strRequest << std::endl;



    for(int i = 0 ; i < 5 ; i++)
    {
        try
        {
            // Make a synchronous remote call to server.
            stub.PositionInformation(NULL, &request, &response, NULL);

            // Print out response.
            std::string strResponse;
            TextFormat::PrintToString(response, &strResponse);
            std::cout << "Received response: " << strResponse << std::endl;
            std::cout << strResponse << std::endl;
            std::chrono::seconds duration(1);
            std::this_thread::sleep_for(duration);
        }
        catch(const RCF::Exception & e)
        {
            std::cout << "RCF::Exception: " << e.getErrorString() << std::endl;
            //return 1;
        }
    }

    return 0;
}
