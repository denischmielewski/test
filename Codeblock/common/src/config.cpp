#include "errors.hpp"
#include "config.hpp"
#include "utils.hpp"
#include <iostream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <string>
#include <unistd.h>     //for gethostname() call
#include "log.hpp"

#define MAX_BUFFER_LENGTH 1024

using namespace std;
const std::string GLOBAL_STARTUP_CONFIG_XML_FILE = "/home/train/config/global/user_startup_config.xml";

//retrieve BOOSTlogger
startup_severity_channel_logger_mt& lg = startup_logger_c1::get();

config::config()
{
    //ctor
    boost::property_tree::ptree pt;
    std::ifstream configFile;
    char hostname[MAX_BUFFER_LENGTH];
    std::string ipAddress;

    configFile.open(GLOBAL_STARTUP_CONFIG_XML_FILE.c_str());
    if(configFile.fail())
    {
        BOOST_LOG_SEV(lg, critical) << "NOT FOUND config file /hometrain/config/global/user_startup_config.xml !!!";
        return;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "found config file /hometrain/config/global/user_startup_config.xml !";
        configFile.close();
        try{boost::property_tree::read_xml( GLOBAL_STARTUP_CONFIG_XML_FILE, pt );}
        catch(const boost::property_tree::ptree_error& ex)
        {
            BOOST_LOG_SEV(lg, critical) << "ERROR in config file /hometrain/config/global/user_startup_config.xml !";
            BOOST_LOG_SEV(lg, critical) << ex.what();
            return;
        }
        BOOST_LOG_SEV(lg, notification) << "PARSED OK config file /hometrain/config/global/user_startup_config.xml !";
    }

    //global config file found. Retrive IP address for this element (train or server or operator PC...)
    try
    {
        gethostname(hostname, MAX_BUFFER_LENGTH);

        BOOST_LOG_SEV(lg, notification) << "Computer hostname found : " << hostname;
        string node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_PORT.IP";
        ipaddressmask = pt.get<std::string>(node);
        std::size_t pos = ipaddressmask.find("/");      // position of "/" in string
        ipaddress = ipaddressmask.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "IP address & subnet to configure : " << ipaddress << " " << ipaddressmask;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_PORT.ADAPTER";
        adapter = pt.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Network adapter to use : " << adapter;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_PORT.PORT";
        port = pt.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Communication port to use : " << port;

    }
    catch(const boost::property_tree::ptree_error& ex)
    {
        BOOST_LOG_SEV(lg, critical) << "ERROR while retrieving Network config parameter !";
        BOOST_LOG_SEV(lg, critical) << ex.what();
        return;
    }

    //Configure IP address on specified adapter
    string command = "sudo -S ip addr add " + ipaddressmask + " dev " + adapter;
    string ls = GetStdoutFromCommand(command, MAX_BUFFER_LENGTH);
    //Verify if IP address has been properly configured
    command = "ip addr list | sed -n \"/"+ adapter + "/,/" + adapter +  "/p\" | grep inet";
    //ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");
    ls = GetStdoutFromCommand(command, MAX_BUFFER_LENGTH);

    if(ls.find(ipaddress,0) == string::npos)
    {
        BOOST_LOG_SEV(lg, critical) << "address not configured : " << ipaddress;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "address configured properly : " << ipaddress;
    }
}

config::~config()
{
    //dtor
    //remove IP address on specified adapter
    if(result == NO_ERROR)
    {
        string command = "sudo -S ip addr del " + ipaddress + " dev " + adapter;
        string ls = GetStdoutFromCommand(command, MAX_BUFFER_LENGTH);
        //Verify if IP address has been properly configured
        command = "ip addr list | sed -n \"/"+ adapter + "/,/" + adapter +  "/p\" | grep inet";
        //ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");
        ls = GetStdoutFromCommand(command, MAX_BUFFER_LENGTH);

        if(ls.find(ipaddress,0) == string::npos)
        {
            BOOST_LOG_SEV(lg, notification) << "IP address removed properly : " << ipaddress;
        }
        else
        {
            BOOST_LOG_SEV(lg, critical) << "IP address not removed properly : " << ipaddress;
        }
    }
    else
    {
        BOOST_LOG_SEV(lg, critical) << "Error when terminating the program due to configuration issue !!! Search for errors in Logs !!!";
    }
}
