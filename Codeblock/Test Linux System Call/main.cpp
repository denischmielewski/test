#include <iostream>
#include <cstdlib>
#include "string.h"
#include "errno.h"
#include "stdio.h"

using namespace std;

string GetStdoutFromCommand(string cmd)
{
    string data;
    FILE * stream;
    const int max_buffer = 1000;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

int main()
{
    //int i =0;
    cout << "Hello world!" << endl << endl << endl;
/*
    i= system ("cd nonexistingfolder");
    cout << i << endl;
    cout << strerror(errno) << endl;
*/
    //string ls = GetStdoutFromCommand("cd nonexistingfolder");

    //std::cout << "Press Enter to continue" << std::endl;
    //std::cin.get();

    string ls = GetStdoutFromCommand("sudo -S ip addr add 192.168.0.55/24 dev eth0");


    cout << "erreur code : " << errno << endl;
    cout << "erreur code string : " << strerror(errno) << endl;
    cout << "LS: " << ls << endl;

    ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");

    if(ls.find("192.168.0.55",0) == string::npos)
    {
        cout << " address not configured " << endl;
    }
    else
    {
        cout << "address configured properly " << endl;
    }

    ls = GetStdoutFromCommand("sudo ip addr del 192.168.0.55/24 dev eth0");

    if(ls.find("192.168.0.55",0) == string::npos)
    {
        cout << "address removed properly " << endl;
    }
    else
    {
        cout << "address not removed " << endl;
    }

    std::cout << "Press Enter to quit...." << std::endl;
    std::cin.get();

    return 0;
}
