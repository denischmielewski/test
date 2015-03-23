#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include <iostream>
#include "protobufsyncclient.hpp"
#include <thread>
#include <chrono>
#include <signal.h>

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
    class log * train_logs;
    class config * train_config;
    ProtobufSyncClient * client;

    startup_severity_channel_logger_mt& lg = startup_logger_c1::get();

    //First setup logs capability
    try
    {
        train_logs = new(class log);
    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }
    //now we can log
    BOOST_LOG_SEV(lg, notification) << "Program Train started";
    BOOST_LOG_SEV(lg, notification) << "Boost.Logging library initialized !";

    //then read information from xml configuration file and configure networking
    try
    {
        train_config = new(class config);
        if (train_config->result != NO_ERROR){throw (-1);};
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "Reading config file failed !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

    config_signal_management();

    BOOST_LOG_SEV(lg, notification) << "try to create ProtobufSyncClient !!!";
    try
    {
            client = new ProtobufSyncClient(train_config);
            client->Start();
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "problem with protocolsyncserver !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

//    if(client->ProtobufSyncClientThread.joinable()) client->Join();
    client->Join();

    BOOST_LOG_SEV(lg, notification) << "All threads completed.";

    delete(client);
    delete(train_config);
    delete(train_logs);

    BOOST_LOG_SEV(lg, notification) << "EVERYTHING TERMNATED PROPERLY !!!";

    return NO_ERROR;
}
