#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include "protobufsyncserver.hpp"
#include <iostream>
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
    class config * server_configuration;
    class log * train_logs;
    ProtobufSyncServer * server;


    startup_severity_channel_logger_mt& lg = comm_logger_c1::get();

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
    BOOST_LOG_SEV(lg, notification) << "Program TrainStartUp started";
    BOOST_LOG_SEV(lg, notification) << "Boost.Logging library initialized !";

    //then read information from xml configuration file and configure networking
    try
    {
        server_configuration = new(class config);
        if (server_configuration->result != NO_ERROR){throw (-1);}
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "Reading config file failed !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

    config_signal_management();

    BOOST_LOG_SEV(lg, critical) << "try to create ProtobufSyncServer !!!";

    //ProtobufSyncServer server(server_configuration);
    server = new ProtobufSyncServer(server_configuration);
    server->Start();

    std::thread thread1(codeThread1,10);
    std::thread thread2(codeThread2,10);

    BOOST_LOG_SEV(lg, notification) << "main, Thread1 and Thread2 now execute concurrently...";

    // synchronize threads:
    thread1.join();               // pauses until first finishes
    thread2.join();               // pauses until second finishes

    BOOST_LOG_SEV(lg, notification) << "Thread1 and Thread2 completed.";

    delete(server);
    delete(server_configuration);
    delete(train_logs);
    return NO_ERROR;
}
