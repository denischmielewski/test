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
const std::string GLOBAL_DEV_CONFIG_XML_FILE = "/home/train/config/global/dev_config.xml";

extern uint16_t g_commSessionMutexLockTimeoutMilliseconds;

//retrieve BOOSTlogger
startup_severity_channel_logger_mt& lg = startup_logger_c1::get();

config::config()
{
    //ctor
    boost::property_tree::ptree pt1, pt2;
    std::ifstream configFile, configFile2;
    std::string ipAddress;

    configFile.open(GLOBAL_STARTUP_CONFIG_XML_FILE.c_str());
    if(configFile.fail())
    {
        BOOST_LOG_SEV(lg, critical) << "PROBLEM opening config file /home/train/config/global/user_startup_config.xml !!!";
        return;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "found config file /home/train/config/global/user_startup_config.xml !";
        configFile.close();
        try{boost::property_tree::read_xml( GLOBAL_STARTUP_CONFIG_XML_FILE, pt1 );}
        catch(const boost::property_tree::ptree_error& ex)
        {
            BOOST_LOG_SEV(lg, critical) << "ERROR in config file /home/train/config/global/user_startup_config.xml !";
            BOOST_LOG_SEV(lg, critical) << ex.what();
            return;
        }
        BOOST_LOG_SEV(lg, notification) << "PARSED OK config file /home/train/config/global/user_startup_config.xml !";
    }

    configFile2.open(GLOBAL_DEV_CONFIG_XML_FILE.c_str());
    if(configFile2.fail())
    {
        BOOST_LOG_SEV(lg, critical) << "PROBLEM opening config file /home/train/config/global/dev_config.xml !!!";
        return;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "found config file /home/train/config/global/dev_config.xml !";
        configFile2.close();
        try{boost::property_tree::read_xml( GLOBAL_DEV_CONFIG_XML_FILE, pt2 );}
        catch(const boost::property_tree::ptree_error& ex)
        {
            BOOST_LOG_SEV(lg, critical) << "ERROR in config file /home/train/config/global/dev_config.xml !";
            BOOST_LOG_SEV(lg, critical) << ex.what();
            return;
        }
        BOOST_LOG_SEV(lg, notification) << "PARSED OK config file /home/train/config/global/dev_config.xml !";
    }

    //global config file found. Retrieve all config parameters for this element (train or server or operator PC...)
    try
    {
        std::string node = "TRAIN_CONFIG_DEV.LINUX_SYSTEM_CALL_BUFFER_SIZE";
        linuxSysCallBufferSize_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "Size of buffer for linux system calls : " << linuxSysCallBufferSize_;

        node = "TRAIN_CONFIG_DEV.COMM_SESSION_MUTEX_LOCK_TIMEOUT";
        commSessionMutexLockTimeoutMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        g_commSessionMutexLockTimeoutMilliseconds = commSessionMutexLockTimeoutMilliseconds_;
        BOOST_LOG_SEV(lg, notification) << "timeout on communication session mutex lock : " << commSessionMutexLockTimeoutMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.OPERATION_SESSION_MUTEX_LOCK_TIMEOUT";
        operationSessionMutexLockTimeoutMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "timeout on operation session mutex lock : " << operationSessionMutexLockTimeoutMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.COMMUNICATION_THREAD_SLEEP_DURATION";
        communicationThreadsSleepDurationMilliseconds_= std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "communication Threads sleep duration : " << communicationThreadsSleepDurationMilliseconds_ << " milliseconds";

        char * hostname = nullptr;
        hostname = new char[linuxSysCallBufferSize_];   //deleted at the end of the block
        gethostname(hostname, linuxSysCallBufferSize_);

        BOOST_LOG_SEV(lg, notification) << "Computer hostname found : " << hostname;
        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_PORT.IP";
        ipaddressmask = pt1.get<std::string>(node);
        std::size_t pos = ipaddressmask.find("/");      // position of "/" in string
        ipaddress = ipaddressmask.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "IP address & subnet to configure : " << ipaddress << " " << ipaddressmask;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_PORT.ADAPTER";
        adapter = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Network adapter to use : " << adapter;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_PORT.PORT";
        port = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Communication port to use : " << port;

        node = "TRAIN_STARTUP_CONFIG.LOG.LOG_COLLECTOR_FOLDER";
        boostLogCollectorFolder_ = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Log collector folder : " << boostLogCollectorFolder_;

        node = "TRAIN_STARTUP_CONFIG.LOG.LOG_COLLECTOR_FOLDER_MAX_SIZE_IN_MBYTES";
        boostLogCollectorFolderMaxSizeInMbytes_ = stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "log collector folder max size : " << boostLogCollectorFolderMaxSizeInMbytes_ << " MBytes";

        node = "TRAIN_STARTUP_CONFIG.LOG.DRIVE_MIN_FREE_SPACE_IN_MBYTES";
        boostLogDriveMinimumFreeSizeInMegabytes_ = stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "log collector folder drive minimum size : " << boostLogDriveMinimumFreeSizeInMegabytes_  << " MBytes";

        node = "TRAIN_STARTUP_CONFIG.LOG.LOG_AUTO_FLUSH";
        if(pt1.get<std::string>(node) == "YES") boostLogAutoFlush_ = true;
        else boostLogAutoFlush_ = false;
        BOOST_LOG_SEV(lg, notification) << "autoflush log ? : " << boostLogAutoFlush_;

        node = "TRAIN_STARTUP_CONFIG.LOG.MAX_LOG_FILE_SIZE_IN_KBYTES";
        boostLogFileMaxSizeInKBytes_ = stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "log file max size : " << boostLogFileMaxSizeInKBytes_ << " KBytes";

        node = "TRAIN_STARTUP_CONFIG.LOG.LOG_LEVEL";
        boostLogLevel_ = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "log level ? : " << boostLogLevel_;

        delete [] hostname;

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
