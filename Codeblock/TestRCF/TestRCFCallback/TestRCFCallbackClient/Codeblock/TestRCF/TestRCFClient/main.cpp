#include <iostream>

#include <RCF/RCF.hpp>


RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
    RCF_METHOD_V1(void, Print, const std::string &)
RCF_END(I_HelloWorld)

class HelloWorldImplClient
{
public:
    void Print(const std::string & s)
    {
        std::cout << "I_HelloWorld service: " << s << std::endl;
    }
};


int main()
{
    RCF::RcfInitDeinit rcfInit;

    // Client needs a RcfServer to accept callback connections.
    RCF::RcfServer callbackServer(( RCF::TcpEndpoint("127.0.0.1",50000) ));
    HelloWorldImplClient helloWorld;
    callbackServer.bind<I_HelloWorld>(helloWorld);
    callbackServer.start();

    // Establish client connection to server.
    RcfClient<I_HelloWorld> gClient;


    int i = 1;
    while (i)
    {
        sleep(1);
        try
        {
            RcfClient<I_HelloWorld> client( RCF::TcpEndpoint("192.168.0.11",50001) );

            gClient = client;
            // 2 second timeout when establishing network connection.
            client.getClientStub().setConnectTimeoutMs(2000);
            // 5 second timeout when waiting for remote call response from the server.
            client.getClientStub().setRemoteCallTimeoutMs(5*1000);

            client.Print("Hello World");
            i=0;
        }
        catch(const RCF::Exception & e)
        {
            std::cout << "Error: " << e.getErrorString() << std::endl;
            std::cout << i << std::endl;
            i++;
        }
    }

    // Create the callback connection.
    RCF::createCallbackConnection(gClient, callbackServer);

    // Print on the server
    for(int i=0;i<30;i++)
    {
        gClient.Print("Hello World Client");
        sleep(1);
        std::cout << i << std::endl;
    }

    ///*
    std::cout << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    //*/
    return 0;
}
