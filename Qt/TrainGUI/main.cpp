#include "traincontrols.h"

#include <QApplication>
#include <QObject>
#include <QThread>

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <RCFProto.hpp>

#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include "traincommunicationsserver.h"
#include "traincommunicationclient.hpp"
#include "TrainSession.hpp"

#define VERSION     "0.0.1.3"

// FIXME this variable is declared only to compile. It is not used in the GUI context.
// To be fixed in codeblock train and server sw, for example by removing the thread code from TrainOperationSession.cpp and put it in
// another file specific to train.
volatile int g_signal_received = 0;

int main(int argc, char *argv[])
{
    class log * trainGUI_logs_start, * trainGUI_logs_after_configread;
    class config * trainGUI_configuration;
    startup_severity_channel_logger_mt * logger;
    TrainCommunicationsServer * t;
    std::unordered_map<std::string, TrainSession>    trainsSessions;

    QApplication a(argc, argv);

    //First setup logs capability with raw parameters. We want to be able to inspect configuration in case of problem !
    try
    {
        trainGUI_logs_start = new class log;
        startup_severity_channel_logger_mt& lg = main_logger::get();
        logger = &lg;
        try
        {
            trainGUI_configuration = new config;
            if (trainGUI_configuration->result != NO_ERROR){throw (-1);}
            else
            {
                if (trainGUI_configuration->configureGUIIPPortMask_() == NO_ERROR)
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
        trainGUI_logs_start->RemoveStartupSink();
        delete(trainGUI_logs_start);
    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }

    //now we can configure logging as specified in xml configuration files
    try
    {
        trainGUI_logs_after_configread = new class log(trainGUI_configuration);
    }
    catch(const std::exception& e)
    {
        std::cout << "Initialization of log system after config failed !!!" << e.what() << std::endl;
        return ERROR_LOG_COULD_NOT_BE_INITIALIZED;
    }

    //now we can log
    // Initialize RCFProto.

    RCF::init();
    BOOST_LOG_SEV(*logger, notification) << "TrainGUI RCF init !";

    TrainCommunicationsServer TrainCommunicationsServerThread(trainGUI_configuration, &trainsSessions);
    TrainCommunicationsServerThread.start();
    TrainCommunicationClient TrainCommunicationClientThread(trainGUI_configuration, &trainsSessions);
    TrainCommunicationClientThread.start();

    TrainControls w(0, trainGUI_configuration, &TrainCommunicationsServerThread, &TrainCommunicationClientThread);
    BOOST_LOG_SEV(*logger, notification) << "Program trainGUI started ! Version : " << VERSION << " date : " << __DATE__ << ":" << __TIME__;
    BOOST_LOG_SEV(*logger, notification) << "Everything configured, communication thread running ... !";

    w.show();

    int ret = a.exec();

    TrainCommunicationsServerThread.wait();
    TrainCommunicationClientThread.wait();

    trainGUI_configuration->removeGUIIPPortMask_();

    //Log Summary of communication sessions with trains
    BOOST_LOG_SEV(*logger, notification) << "train communication sessions summary :" << std::endl;
    int i = 0;
    for ( auto it = trainsSessions.begin(); it != trainsSessions.end(); ++it ){i++;};
    BOOST_LOG_SEV(*logger, notification) << "number of sessions :" << i;

    for ( auto it = trainsSessions.begin(); it != trainsSessions.end(); ++it )
    {
        BOOST_LOG_SEV(*logger, notification) << "Train IP address :" << it->first;
        TrainSession trainsession = it->second;
        TrainCommSession & traincommsession = trainsession.GetTrainCommSessionRef();
        if(traincommsession.TryLockCommSessionMutexFor(trainGUI_configuration->commSessionMutexLockTimeoutMilliseconds_))
        {
            time_t timeraw = traincommsession.GetSessionConnectionTime();
#warning TODO (dev#1#15-03-27): the following line add a carriage return in log file.
            BOOST_LOG_SEV(*logger, notification) << "Session connection at : " << ctime(&timeraw);
            BOOST_LOG_SEV(*logger, notification) << "Session connection duration : " << traincommsession.GetSessionConnectionDuration();
            BOOST_LOG_SEV(*logger, notification) << "Session remote calls count : " << traincommsession.GetSessionRemoteCallCount();
            BOOST_LOG_SEV(*logger, notification) << "Session total bytes received : " << traincommsession.GetSessionTotalBytesReceived();
            BOOST_LOG_SEV(*logger, notification) << "Session total bytes sent : " << traincommsession.GetSessionTotalBytesSent();
            BOOST_LOG_SEV(*logger, notification) << "Session connection lost count : " << traincommsession.GetSessionConnectionLossCount();
            traincommsession.UnlockCommSessionMutex();
            BOOST_LOG_SEV(*logger, notification) << "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
        }
        else
        {
            BOOST_LOG_SEV(*logger, warning) << "Train Communication Session Lock failed !!!";
        }
        BOOST_LOG_SEV(*logger, notification) << "222222222222222222222222222222222222222222222222222222222222222222222222222222222";
    }

    delete(trainGUI_configuration);
    BOOST_LOG_SEV(*logger, notification) << "333333333333333333333333333333333333333333333333333333333333333333333333333333333";
    delete(trainGUI_logs_after_configread);
    BOOST_LOG_SEV(*logger, notification) << "444444444444444444444444444444444444444444444444444444444444444444444444444444444";

    BOOST_LOG_SEV(*logger, notification) << "EVERYTHING TERMINATED PROPERLY !!! GUI return code = " << ret;

    return NO_ERROR;
}
