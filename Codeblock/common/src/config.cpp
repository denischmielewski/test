#include "config.hpp"
#include "errors.hpp"
#include "utils.hpp"
#include <iostream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <string>
#include <unistd.h>     //for gethostname() call
#include "log.hpp"
#include "TrainOperationSession.hpp"

// TODO (dev#1#15-03-27): to be removed and use value from xml config file
//#define MAX_BUFFER_LENGTH 1024

using namespace std;
const std::string GLOBAL_STARTUP_CONFIG_XML_FILE = "/home/train/config/global/user_startup_config.xml";
const std::string GLOBAL_DEV_CONFIG_XML_FILE = "/home/train/config/global/dev_config.xml";
const std::string DEFAULT_DATA_CONFIG_XML_FILE = "/home/train/config/local/defaultdata.xml";

//retrieve BOOSTlogger
startup_severity_channel_logger_mt& lg = startup_logger::get();

config::config()
{
    //ctor
    boost::property_tree::ptree pt1, pt2, pt3;
    std::ifstream configFile, configFile2, configFile3;
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

    configFile3.open(DEFAULT_DATA_CONFIG_XML_FILE.c_str());
    if(configFile3.fail())
    {
        BOOST_LOG_SEV(lg, critical) << "PROBLEM opening config file /home/train/config/local/defaultdata.xml !!!";
        return;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "found config file /home/train/config/local/defaultdata.xml !";
        configFile3.close();
        try{boost::property_tree::read_xml(DEFAULT_DATA_CONFIG_XML_FILE, pt3 );}
        catch(const boost::property_tree::ptree_error& ex)
        {
            BOOST_LOG_SEV(lg, critical) << "ERROR in config file /home/train/config/local/defaultdata.xml !";
            BOOST_LOG_SEV(lg, critical) << ex.what();
            return;
        }
        BOOST_LOG_SEV(lg, notification) << "PARSED OK config file /home/train/config/local/defaultdata.xml !";
    }

    //global config files found. Retrieve all config parameters for this element (train or server or operator PC...)
    try
    {
        std::string node = "TRAIN_CONFIG_DEV.LINUX_SYSTEM_CALL_BUFFER_SIZE";
        linuxSysCallBufferSize_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "Size of buffer for linux system calls : " << linuxSysCallBufferSize_;

        node = "TRAIN_CONFIG_DEV.COMM_SESSION_MUTEX_LOCK_TIMEOUT";
        commSessionMutexLockTimeoutMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "timeout on communication session mutex lock : " << commSessionMutexLockTimeoutMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.OPERATION_SESSION_MUTEX_LOCK_TIMEOUT";
        operationSessionMutexLockTimeoutMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "timeout on operation session mutex lock : " << operationSessionMutexLockTimeoutMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.COMMUNICATION_THREAD_SLEEP_DURATION";
        communicationThreadsSleepDurationMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "communication Threads sleep duration : " << communicationThreadsSleepDurationMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.LINUX_SYS_CALL_SLEEP_DURATION";
        linuxSysCallSleepDurationMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "Linux system call sleep duration : " << linuxSysCallSleepDurationMilliseconds_ << " microseconds";

        node = "TRAIN_CONFIG_DEV.THREADS_LOGS_NOTIFICATION_FREQUENCY";
        ThreadsLogNotificationFrequencyMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "Threads log notification frequency : " << ThreadsLogNotificationFrequencyMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.THREADS_EXIT_TIMEOUT";
        ThreadsExitTimeoutMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "Threads exit timeout value : " << ThreadsExitTimeoutMilliseconds_ << " milliseconds";

        node = "TRAIN_CONFIG_DEV.LOAD_TRAIN_OP_DATA_FROM_DEFAULTDATA_XML_FILE";
        std::string s1 = pt2.get<std::string>(node);
        if(s1 == "YES")
        {
            loadTrainOperationDataFromDefaultDataXmlFile_ = true;
            BOOST_LOG_SEV(lg, critical) << "train operation Data WILL be loaded from defaultdata.xml file !!!";
        }
        else
        {
            loadTrainOperationDataFromDefaultDataXmlFile_ = false;
            BOOST_LOG_SEV(lg, notification) << "train operation Data WILL NOT be loaded from defaultdata.xml file !!!";
        }

        node = "TRAIN_CONFIG_DEV.MOVEMENT_THREAD_BEAT";
        movementThreadBeatMilliseconds_ = std::stoul(pt2.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "Movement Thread Beat period : " << movementThreadBeatMilliseconds_ << " milliseconds";

        char * hostname = nullptr;
        hostname = new char[linuxSysCallBufferSize_];   //deleted at the end of the block
        gethostname(hostname, linuxSysCallBufferSize_);
        hostname_ = std::string(hostname);
        BOOST_LOG_SEV(lg, notification) << "Computer hostname found : " << hostname_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_SUBNET.MAIN_IP";
        main_ipaddressmask_ = pt1.get<std::string>(node);
        std::size_t pos = main_ipaddressmask_.find("/");      // position of "/" in string
        main_ipaddress_ = main_ipaddressmask_.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "IP address & subnet to configure for main software : " << main_ipaddress_ << " " << main_ipaddressmask_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_SUBNET.GUI_IP";
        gui_ipaddressmask_ = pt1.get<std::string>(node);
        pos = gui_ipaddressmask_.find("/");      // position of "/" in string
        gui_ipaddress_ = gui_ipaddressmask_.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "IP address & subnet to configure for GUI software : " << gui_ipaddress_ << " " << gui_ipaddressmask_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG." + string(hostname) + ".IP_ADDRESSES_AND_SUBNET.ADAPTER";
        adapter_ = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Network adapter to use : " << adapter_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG.MAIN_LISTENER_PORT";
        main_listener_port_ = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Communication port to use for main Sw : " << main_listener_port_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG.GUI_LISTENER_PORT";
        gui_listener_port_ = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Communication port to use for GUI Sw : " << gui_listener_port_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG.SERVER1_LISTENER_PORT";
        server1_listener_port_ = pt1.get<std::string>(node);
        BOOST_LOG_SEV(lg, notification) << "Communication port to use for Server1 : " << server1_listener_port_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG.SERVER1.IP_ADDRESSES_AND_SUBNET.MAIN_IP";
        std::string ipaddressmask = pt1.get<std::string>(node);
        pos = ipaddressmask.find("/");      // position of "/" in string
        server1_ipaddress_ = ipaddressmask.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "server1 Main sw IP address : " << server1_ipaddress_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG.SERVER2.IP_ADDRESSES_AND_SUBNET.MAIN_IP";
        ipaddressmask = pt1.get<std::string>(node);
        pos = ipaddressmask.find("/");      // position of "/" in string
        server2_ipaddress_ = ipaddressmask.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "server2 Main sw IP address : " << server2_ipaddress_;

        node = "TRAIN_STARTUP_CONFIG.TRAINS_IP_CONFIG.SERVER3.IP_ADDRESSES_AND_SUBNET.MAIN_IP";
        ipaddressmask = pt1.get<std::string>(node);
        pos = ipaddressmask.find("/");      // position of "/" in string
        server3_ipaddress_ = ipaddressmask.substr (0,pos);
        BOOST_LOG_SEV(lg, notification) << "server3 Main sw IP address : " << server3_ipaddress_;

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

        node = "TRAIN_STARTUP_CONFIG.TCPIP.TCPIP_CONNECTION_TIMEOUT";
        TCPIP_Connection_Timeout_ = std::stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "TCPIP Connection timeout value : " << TCPIP_Connection_Timeout_ << " milliseconds";

        node = "TRAIN_STARTUP_CONFIG.TCPIP.TCPIP_REPLY_TIMEOUT";
        TCPIP_Reply_Timeout_ = std::stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "TCPIP Reply timeout value : " << TCPIP_Reply_Timeout_ << " milliseconds";

        node = "TRAIN_STARTUP_CONFIG.SYNCHRONOUS_MESSAGES.TRAIN_TO_SERVER1_MESSAGES_FREQUENCY";
        TrainToServer1MessagesFrequency_ = std::stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "train to Server1 messages frequency : " << TrainToServer1MessagesFrequency_ << " milliseconds";

        node = "TRAIN_STARTUP_CONFIG.SYNCHRONOUS_MESSAGES.TRAIN_TO_TRAINGUI_MESSAGES_FREQUENCY";
        TrainToTrainGUIMessagesFrequency_ = std::stoul(pt1.get<std::string>(node), nullptr, 10);
        BOOST_LOG_SEV(lg, notification) << "train to TrainGUI messages frequency : " << TrainToTrainGUIMessagesFrequency_ << " milliseconds";

        if(loadTrainOperationDataFromDefaultDataXmlFile_)
        {
            node = "DEFAULT.MODE";
            std::string s2 = pt3.get<std::string>(node);
            if(s2 == "AUTOMATIC") defaultMode_ = AUTOMATIC;
            else if(s2 == "SEMIAUTOMATIC") defaultMode_ = SEMIAUTOMATIC;
            else if(s2 == "MANUAL") defaultMode_ = MANUAL;
            else defaultMode_ = NONE;
            BOOST_LOG_SEV(lg, critical) << "Default mode loaded from defaultdata.xml file : " << s2;

            node = "DEFAULT.PATH";
            defaultPath_ = pt3.get<std::string>(node);
            BOOST_LOG_SEV(lg, critical) << "Default path : " << defaultPath_;

            node = "DEFAULT.DIRECTION";
            defaultDirection_ = stoi(pt3.get<std::string>(node), nullptr, 10);
            BOOST_LOG_SEV(lg, critical) << "Default direction : " << defaultDirection_;

            node = "DEFAULT.KPPOSITION";
            defaultKpPosition_ = stof(pt3.get<std::string>(node), nullptr);
            BOOST_LOG_SEV(lg, critical) << "Default KpPosition : " << defaultKpPosition_;
        }
        delete [] hostname;
    }
    catch(const boost::property_tree::ptree_error& ex)
    {
        BOOST_LOG_SEV(lg, critical) << "ERROR while retrieving config parameter !";
        BOOST_LOG_SEV(lg, critical) << ex.what();
        return;
    }
}

config::~config()
{
    //dtor
}

int16_t config::configureMainIPPortMask_(void)
{
    //Configure main IP address on specified adapter
    string command = "sudo -S ip addr add " + main_ipaddressmask_ + " dev " + adapter_;
    string ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);
    //Verify if IP address has been properly configured
    command = "ip addr list | sed -n \"/"+ adapter_ + "/,/" + adapter_ +  "/p\" | grep inet";
    //ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");
    ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);

    if(ls.find(main_ipaddress_,0) == string::npos)
    {
        BOOST_LOG_SEV(lg, critical) << "Train main IP address not configured : " << main_ipaddress_;
        return ERROR_MAIN_IP_CONFIGURATION;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "Train main IP address configured properly : " << main_ipaddress_;
        return NO_ERROR;
    }
}

int16_t config::configureGUIIPPortMask_(void)
{
    //Configure GUI IP address on specified adapter
    string command = "sudo -S ip addr add " + gui_ipaddressmask_ + " dev " + adapter_;
    string ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);
    //Verify if IP address has been properly configured
    command = "ip addr list | sed -n \"/"+ adapter_ + "/,/" + adapter_ +  "/p\" | grep inet";
    //ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");
    ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);

    if(ls.find(gui_ipaddress_,0) == string::npos)
    {
        BOOST_LOG_SEV(lg, critical) << "Train GUI IP address not configured : " << gui_ipaddress_;
        return ERROR_GUI_IP_CONFIGURATION;
    }
    else
    {
        BOOST_LOG_SEV(lg, notification) << "Train GUI IP address configured properly : " << gui_ipaddress_;
        return NO_ERROR;
    }
}

int16_t config::removeMainIPPortMask_(void)
{
    //remove main IP address on specified adapter
    string command = "sudo -S ip addr del " + main_ipaddress_ + " dev " + adapter_;
    string ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);
    //Verify if IP address has been properly configured
    command = "ip addr list | sed -n \"/"+ adapter_ + "/,/" + adapter_ +  "/p\" | grep inet";
    //ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");
    ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);

    if(ls.find(main_ipaddress_,0) == string::npos)
    {
        BOOST_LOG_SEV(lg, notification) << "Train main IP address removed properly : " << main_ipaddress_;
        return NO_ERROR;
    }
    else
    {
        BOOST_LOG_SEV(lg, critical) << "Train main IP address not removed properly : " << main_ipaddress_;
        return ERROR_MAIN_IP_CONFIGURATION;
    }
}

int16_t config::removeGUIIPPortMask_(void)
{
//remove main IP address on specified adapter
    string command = "sudo -S ip addr del " + gui_ipaddress_ + " dev " + adapter_;
    string ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);
    //Verify if IP address has been properly configured
    command = "ip addr list | sed -n \"/"+ adapter_ + "/,/" + adapter_ +  "/p\" | grep inet";
    //ls = GetStdoutFromCommand("ip addr list | sed -n \"/eth0/,/eth0/p\" | grep inet");
    ls = GetStdoutFromCommand(command, linuxSysCallBufferSize_);

    if(ls.find(main_ipaddress_,0) == string::npos)
    {
        BOOST_LOG_SEV(lg, notification) << "Train GUI IP address removed properly : " << gui_ipaddress_;
        return NO_ERROR;
    }
    else
    {
        BOOST_LOG_SEV(lg, critical) << "train GUI IP address not removed properly : " << gui_ipaddress_;
        return ERROR_MAIN_IP_CONFIGURATION;
    }
}
