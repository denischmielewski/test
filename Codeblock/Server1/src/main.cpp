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

#define VERSION     "0.0.1.2"

volatile int g_signal_received = 0;
std::unordered_map<std::string, TrainSession>    g_trains;

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

void codeThread1(int x)
{
    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();
    std::chrono::seconds duration(1);
    int i = 0;
    while(!g_signal_received && i < x)
    {
        std::this_thread::sleep_for(duration);
        BOOST_LOG_SEV(lg, notification) << "thread 1";
        i++;
    }
    if(g_signal_received && i != x) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating thread 1";
}

void codeThread2(int x)
{
    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();
    std::chrono::seconds duration(1);
    int i = 0;
    while(!g_signal_received && i < x)
    {
        std::this_thread::sleep_for(duration);
        BOOST_LOG_SEV(lg, notification) << "thread 2";
        i++;
    }
    if(g_signal_received && i != x) BOOST_LOG_SEV(lg, notification) << "Signal received, terminating thread 2";
}

int main()
{

    class log * train_logs;
    class config * server_configuration;
    ProtobufSyncServer * server;

    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();

    //First setup logs capability
    try
    {
        train_logs = new class log;
    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }
    //now we can log
    BOOST_LOG_SEV(lg, notification) << "Program Server started ! Version : " << VERSION << " date : " << __DATE__ << ":" << __TIME__;
    BOOST_LOG_SEV(lg, notification) << "Boost.Logging library initialized !";

    //then read information from xml configuration file and configure networking

    try
    {
        server_configuration = new config;
        if (server_configuration->result != NO_ERROR){throw (-1);}
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "Reading config file failed !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

    config_signal_management();

    BOOST_LOG_SEV(lg, notification) << "try to create ProtobufSyncServer !!!";
    try
    {
            server = new ProtobufSyncServer(server_configuration);
            server->Start();
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "problem with protocolsyncserver !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

    std::thread thread1(codeThread1,10);
    std::thread thread2(codeThread2,10);

    BOOST_LOG_SEV(lg, notification) << "main and all threads now execute concurrently...";

    // synchronize threads:
    if(thread1.joinable()) thread1.join();               // pauses until first finishes
    if(thread2.joinable()) thread2.join();               // pauses until second finishes
    server->Join();

    BOOST_LOG_SEV(lg, notification) << "All threads completed." << std::endl;

    delete(server);
    delete(server_configuration);
    delete(train_logs);

    //Log Summary of communication sessions with trains
    BOOST_LOG_SEV(lg, notification) << "train communication sessions summary :" << std::endl;
    int i = 0;
    for ( auto it = g_trains.begin(); it != g_trains.end(); ++it ){i++;};
    BOOST_LOG_SEV(lg, notification) << "number of sessions :" << i;
    for ( auto it = g_trains.begin(); it != g_trains.end(); ++it )
    {
        BOOST_LOG_SEV(lg, notification) << "Train IP address :" << it->first;
        TrainSession trainsession = it->second;
        TrainCommSession & traincommsession = trainsession.GetTrainCommSessionRef();
        if(traincommsession.TryLockCommSessionMutexFor(100))
        {
            time_t timeraw = traincommsession.GetSessionConnectionTime();
            BOOST_LOG_SEV(lg, notification) << "Session connection at : " << ctime(&timeraw);
            BOOST_LOG_SEV(lg, notification) << "Session connection duration : " << traincommsession.GetSessionConnectionDuration();
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
    //return NO_ERROR;
    return NO_ERROR;
}
