#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include <iostream>
#include "protobufsyncclient.hpp"
#include "protobufsyncGUIclient.hpp"
#include <thread>
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include "TrainSession.hpp"

#define VERSION     "0.0.1.2"

volatile int g_signal_received = 0;

void sighandler( int sig, siginfo_t * siginfo, void * context)
{
    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();
    if(sig == SIGINT) BOOST_LOG_SEV(lg, notification) << "SIGHANDLER: SIGINT (CTRL^C) received. terminate all threads...";
    if(sig == SIGTERM) BOOST_LOG_SEV(lg, notification) << "SIGHANDLER: SIGTERM (kill) received. terminate all threads...";
    g_signal_received = 1;
}

void config_signal_management(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof (struct sigaction));
    action.sa_sigaction = sighandler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

int main()
{
#warning TODO (dev#5#15-03-27): warning: variable ‘train_logs_after_configread’ set but not used [-Wunused-but-set-variable]|
    class log * train_logs_start, * train_logs_after_configread;
    class config * train_configuration;
    ProtobufSyncClient * client;
    ProtobufSyncGUIClient * client2;
    std::unordered_map<std::string, TrainSession>    trainsSessions;

    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();

    //First setup logs capability with raw parameters. We want to be able to inspect configuration in case of problem !
    try
    {
        train_logs_start = new class log;
        try
        {
            train_configuration = new config;
            if (train_configuration->result != NO_ERROR){throw (-1);}
            else
            {
                if (train_configuration->configureMainIPPortMask_() == NO_ERROR)
                    BOOST_LOG_SEV(lg, notification) << "main IP configuration terminated properly !!!";
                else
                {
                    BOOST_LOG_SEV(lg, critical) << "PROBLEM main IP configuration !!!";
                    return ERROR_MAIN_IP_CONFIGURATION;
                }
            }
        }
        catch(int e)
        {
            BOOST_LOG_SEV(lg, critical) << "Reading config file failed !!!";
            return ERROR_CONFIG_FILE_HANDLING;
        }
        train_logs_start->RemoveStartupSink();
        //delete(train_logs_start);

    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }

    //now we can configure logging as specified in xml configuration files
    try
    {
        train_logs_after_configread = new class log(train_configuration);
    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system after config failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }

    //now we can log
    BOOST_LOG_SEV(lg, notification) << "Program Train started ! Version : " << VERSION << " date : " << __DATE__ << ":" << __TIME__;;
    BOOST_LOG_SEV(lg, notification) << "Boost.Logging library initialized !";

    config_signal_management();

    BOOST_LOG_SEV(lg, notification) << "try to create ProtobufSyncClient !!!";
    try
    {
            client = new ProtobufSyncClient(train_configuration);
            client->Start();
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "problem with ProtobufSyncClient !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }
    BOOST_LOG_SEV(lg, notification) << "try to create ProtobufSyncGUIClient !!!";
    try
    {
            client2 = new ProtobufSyncGUIClient(train_configuration);
            client2->Start();
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "problem with ProtobufSyncGUIClient !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

//    if(client->ProtobufSyncClientThread.joinable()) client->Join();
    client->Join();
    client2->Join();

    BOOST_LOG_SEV(lg, notification) << "All threads completed.";

    delete(client);
    delete(client2);
    train_configuration->removeMainIPPortMask_();
    delete(train_configuration);
    delete(train_logs_start);
    delete(train_logs_after_configread);

    BOOST_LOG_SEV(lg, notification) << "EVERYTHING TERMNATED PROPERLY !!!";

    return NO_ERROR;
}
