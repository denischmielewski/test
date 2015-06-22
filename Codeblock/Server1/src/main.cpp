#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include "protobufsyncserver.hpp"
#include "TrainSession.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include <unordered_map>

#define VERSION     "0.0.1.3"
#define DATE        "20150404"

volatile int g_signal_received = 0;
std::unordered_map<std::string, TrainSession>    g_trains;
extern uint16_t g_commSessionMutexLockTimeoutMilliseconds;

void sighandler( int sig, siginfo_t * siginfo, void * context)
{
    startup_severity_channel_logger_mt& lg = main_logger::get();
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
    class log * server_logs_start, * server_logs_after_configread;
    class config * server_configuration;
    ProtobufSyncServer * server;

    startup_severity_channel_logger_mt& lg = main_logger::get();

    //First setup logs capability with raw parameters. We want to be able to inspect configuration in case of problem !
    try
    {
        server_logs_start = new class log;
        try
        {
            server_configuration = new config;
            if (server_configuration->result != NO_ERROR){throw (-1);}
            else
            {
                if (server_configuration->configureMainIPPortMask_() == NO_ERROR)
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
        server_logs_start->RemoveStartupSink();
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
        server_logs_after_configread = new class log(server_configuration);
    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system after config failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }

    //now we can log
    BOOST_LOG_SEV(lg, notification) << "Program Server started ! Version : " << VERSION << " date : " << __DATE__ << ":" << __TIME__;
    BOOST_LOG_SEV(lg, notification) << "Boost.Logging library initialized !";

    config_signal_management();

    BOOST_LOG_SEV(lg, notification) << "try to create ProtobufSyncServer !!!";
    try
    {
            server = new ProtobufSyncServer(server_configuration, &g_trains);
            server->Start();
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "problem with protocolsyncserver !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

    BOOST_LOG_SEV(lg, notification) << "main and all threads now execute concurrently...";

    // synchronize threads:
    server->Join();

    BOOST_LOG_SEV(lg, notification) << "All threads completed." << std::endl;

    server_configuration->removeMainIPPortMask_();
    delete(server);
    delete(server_configuration);

    //Log Summary of communication sessions with trains
    BOOST_LOG_SEV(lg, notification) << "train communication sessions summary :" << std::endl;
    int i = 0;
    for ( auto it = g_trains.begin(); it != g_trains.end(); ++it ){i++;};
    BOOST_LOG_SEV(lg, notification) << "number of sessions :" << i;
    i = 0;
    for ( auto it = g_trains.begin(); it != g_trains.end(); ++it )
    {
        i++;
        BOOST_LOG_SEV(lg, notification) << "========================== SESSION : " << i;
        BOOST_LOG_SEV(lg, notification) << "Train IP address :" << it->first;
        //TrainSession trainsession = it->second;
        //TrainCommSession & traincommsession = trainsession.GetTrainCommSessionRef();
        TrainCommSession & traincommsession = (it->second).GetTrainCommSessionRef();
        if(traincommsession.TryLockCommSessionMutexFor(server_configuration->commSessionMutexLockTimeoutMilliseconds_))
        {
            time_t timeraw = traincommsession.GetSessionConnectionTime();
            char buff[20];
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&timeraw));
            std::string s(buff);
            BOOST_LOG_SEV(lg, notification) << "Session connection at : " <<  s;
            BOOST_LOG_SEV(lg, notification) << "Session connection duration : " << traincommsession.GetSessionConnectionDuration() << " seconds.";
            BOOST_LOG_SEV(lg, notification) << "Session remote calls count : " << traincommsession.GetSessionRemoteCallCount();
            BOOST_LOG_SEV(lg, notification) << "Session total bytes received : " << traincommsession.GetSessionTotalBytesReceived();
            BOOST_LOG_SEV(lg, notification) << "Session total bytes sent : " << traincommsession.GetSessionTotalBytesSent();
            BOOST_LOG_SEV(lg, notification) << "Session connection lost count : " << traincommsession.GetSessionConnectionLossCount();
            traincommsession.UnlockCommSessionMutex();
        }
        else
        {
            BOOST_LOG_SEV(lg, warning) << "Train Communication Session Lock failed !!!";
        }
    }

    BOOST_LOG_SEV(lg, notification) << "EVERYTHING TERMINATED PROPERLY !!!";
    delete(server_logs_start);
    delete(server_logs_after_configread);

    //return NO_ERROR;
    return NO_ERROR;
}
