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
#include "trainprotobufsynchronousserver.hpp"
#include "protobufsynchronousclientfortraingui.hpp"
#include "protobufsynchronousclientforserver1.hpp"
#include <RCFProto.hpp>

#define VERSION     "0.0.1.2"

volatile int g_signal_received = 0;

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
    class log * train_logs_start, * train_logs_after_configread;
    class config * train_configuration;
    ProtobufSynchronousClientForServer1 * client1;
    ProtobufSynchronousClientForTrainGUI * client2;
    TrainProtobufSynchronousServer * trainProtobufSynchronousServer;
    std::unordered_map<std::string, TrainSession>    trainsSessions;

    startup_severity_channel_logger_mt& lg = main_logger::get();

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
        delete(train_logs_start);

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

    // Initialize RCFProto.
    try
    {
        RCF::init();
        BOOST_LOG_SEV(lg, notification) << "RCF init !";
    }
    catch(const RCF::Exception & e)
    {
        BOOST_LOG_SEV(lg, critical) << "problem during RCF Initialization : " << e.getErrorString() << std::endl;
        BOOST_LOG_SEV(lg, critical) << "RCF::Exception: " << e.getErrorString() << std::endl;
        return ERROR_RCF_INITIALIZATION;
    }

    BOOST_LOG_SEV(lg, notification) << "try to create TrainProtobufSynchronousServer !!!";
    try
    {
            trainProtobufSynchronousServer = new TrainProtobufSynchronousServer(train_configuration, &trainsSessions);
            trainProtobufSynchronousServer->Start();
            BOOST_LOG_SEV(lg, notification) << "train Protobuf Synchronous Server properly initialized !!!";

    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "PROBLEM creating train Protobuf Synchronous Server !!!";
        return ERROR_WITH_PROTOCOL_BUFFER_SERVER;
    }


    BOOST_LOG_SEV(lg, notification) << "try to create Protobuf Synchronous Server1 client !!!";
    try
    {
            client1 = new ProtobufSynchronousClientForServer1(train_configuration);
            client1->Start();
            BOOST_LOG_SEV(lg, notification) << "train Protobuf Synchronous Server1 client properly initialized !!!";
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "PROBLEM creating Protobuf Synchronous Server1 client !!!";
        return ERROR_WITH_PROTOCOL_BUFFER_CLIENT;
    }


    BOOST_LOG_SEV(lg, notification) << "try to create Protobuf Synchronous GUI client !!!";
    try
    {
            client2 = new ProtobufSynchronousClientForTrainGUI(train_configuration);
            client2->Start();
            BOOST_LOG_SEV(lg, notification) << "train Protobuf Synchronous GUI client properly initialized !!!";
    }
    catch(int e)
    {
        BOOST_LOG_SEV(lg, critical) << "PROBLEM creating Protobuf Synchronous GUI client !!!";
        return ERROR_CONFIG_FILE_HANDLING;
    }

    //all threads are joined in the destructors below
    delete(client1);
    delete(client2);
    delete(trainProtobufSynchronousServer);
    BOOST_LOG_SEV(lg, notification) << "All threads completed.";
    train_configuration->removeMainIPPortMask_();

    //Log Summary of communication sessions with trains
    BOOST_LOG_SEV(lg, notification) << "train communication sessions summary :" << std::endl;
    int i = 0;
    for ( auto it = trainsSessions.begin(); it != trainsSessions.end(); ++it ){i++;};
    BOOST_LOG_SEV(lg, notification) << "number of sessions :" << i;
    for ( auto it = trainsSessions.begin(); it != trainsSessions.end(); ++it )
    {
        BOOST_LOG_SEV(lg, notification) << "Train IP address :" << it->first;
        TrainSession trainsession = it->second;
        TrainCommSession & traincommsession = trainsession.GetTrainCommSessionRef();
        if(traincommsession.TryLockCommSessionMutexFor(train_configuration->commSessionMutexLockTimeoutMilliseconds_))
        {
            time_t timeraw = traincommsession.GetSessionConnectionTime();
#warning TODO (dev#1#15-03-27): the following line add a carriage return in log file.
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

    delete(train_configuration);
    delete(train_logs_after_configread);

    BOOST_LOG_SEV(lg, notification) << "EVERYTHING TERMINATED PROPERLY !!!";

    return NO_ERROR;
}
