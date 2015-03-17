#include <iostream>

#include <RCF/RCF.hpp>


RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
    RCF_METHOD_V1(void, Print, const std::string &)
RCF_END(I_HelloWorld)

int main()
{
    RCF::RcfInitDeinit rcfInit;
    int i=0;
    std::cout << "Calling the I_HelloWorld Print() method." << std::endl;
    RcfClient<I_HelloWorld> client( RCF::TcpEndpoint(50001) );
    for(i=0;i<1000;i++)
    {
        client.Print("Hello World");
        usleep(1000);
        std::cout << i << std::endl;
    }
    return 0;
}
