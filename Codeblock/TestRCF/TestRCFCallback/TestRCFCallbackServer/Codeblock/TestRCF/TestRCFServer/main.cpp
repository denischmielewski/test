#include <RCF/RCF.hpp>

#include <iostream>


RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
    RCF_METHOD_V1(void, Print, const std::string &)
RCF_END(I_HelloWorld)

int j=0;

class HelloWorldImpl
{
public:
    void Print(const std::string & s)
    {
        std::cout << "I_HelloWorld service: " << s << std::endl;
    }
};

typedef boost::shared_ptr< RcfClient<I_HelloWorld> >    HelloWorldPtr;
HelloWorldPtr                                           gCallbackClientPtr;


void onCallbackConnectionCreated(
    RCF::RcfSessionPtr sessionPtr,
    RCF::ClientTransportAutoPtr transportAutoPtr)
{
    j++;
    HelloWorldPtr helloWorldPtr( new RcfClient<I_HelloWorld>(transportAutoPtr) );
    gCallbackClientPtr= helloWorldPtr;
}

int main()
{
    RCF::RcfInitDeinit rcfInit;

    int i = 0;
    HelloWorldImpl helloWorld;
    RCF::RcfServer server( RCF::TcpEndpoint(50001) );
    server.bind<I_HelloWorld>(helloWorld);
    server.setOnCallbackConnectionCreated(boost::bind(&onCallbackConnectionCreated, _1, _2) );
    server.start();

    // Server-side - wait for client so callback connection can be created.
    while(j==0)
    {
        i++;
        std::cout << i << std::endl;
        RCF::sleepMs(1000);
    }

    // Print on client using Callback
    while(i < 30)
    {
        i++;
        std::cout << i << std::endl;
        gCallbackClientPtr->Print("Hello World Callback");
        RCF::sleepMs(1000);
    }

    std::cout << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    //deallocate memory
    gCallbackClientPtr.reset();
    return 0;
}
