#include <iostream>
#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include <unistd.h>

using namespace std;

#define MAX_BUFFER_LENGTH 1024

const std::string XML_PATH1 = "/home/train/config/global/train_config.xml";

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
    boost::property_tree::ptree pt1;
    ifstream configFile;
    char hostname[MAX_BUFFER_LENGTH];
    string ipAddress;


    configFile.open(XML_PATH1.c_str());
    if(configFile.fail())
    {
        std::cout << "global config file not found at " << XML_PATH1.c_str() <<std::endl;
        return(-1);
    }
    else
    {
        configFile.close();
        boost::property_tree::read_xml( XML_PATH1, pt1  );
    }

    if(!gethostname(hostname, MAX_BUFFER_LENGTH))
    {
        std::cout << hostname << std::endl;
        string node = "TRAINS_CONFIG." + string(hostname) + ".IP_ADDESSES_AND_PORT.PMP.IP";
        std::cout << node << std::endl;
        try
        {
            string ipAddr = pt1.get<std::string>(node);
            ipAddress = ipAddr;
        }
        catch(const boost::property_tree::ptree_error& ex)
        {
            std::cout << ex.what() << std::endl;
        }
        std::cout << ipAddress << std::endl;
    }
    else
    {
        std::cout << "impossible to get hostname" << std::endl;
    }

    return 0;
}
