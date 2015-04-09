#include "traincontrols.h"
#include <QApplication>
#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>
#include "traincommunicationsserver.h"
#include "TrainSession.hpp"

#define VERSION     "0.0.1.2"

std::unordered_map<std::string, TrainSession>    g_trains;

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

    t = new TrainCommunicationsServer(trainGUI_configuration, &trainsSessions);
    t->start();
    TrainControls w(0, trainGUI_configuration, t);
    BOOST_LOG_SEV(*logger, notification) << "Program trainGUI started ! Version : " << VERSION << " date : " << __DATE__ << ":" << __TIME__;
    BOOST_LOG_SEV(*logger, notification) << "Everything configured, communication thread running ... !";


/*    //Log Summary of communication sessions with trains
    BOOST_LOG_SEV(lg, notification) << "train communication sessions summary :" << std::endl;
    int i = 0;
    for ( auto it = g_trains.begin(); it != g_trains.end(); ++it ){i++;};
    BOOST_LOG_SEV(lg, notification) << "number of sessions :" << i;
    for ( auto it = g_trains.begin(); it != g_trains.end(); ++it )
    {
        BOOST_LOG_SEV(lg, notification) << "Train IP address :" << it->first;
        TrainSession trainsession = it->second;
        TrainCommSession & traincommsession = trainsession.GetTrainCommSessionRef();
        if(traincommsession.TryLockCommSessionMutexFor(server_configuration->commSessionMutexLockTimeoutMilliseconds_))
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
*/

    w.show();

    int ret = a.exec();

    t->wait();
    trainGUI_configuration->removeGUIIPPortMask_();
    delete(t);
    delete(trainGUI_configuration);
    delete(trainGUI_logs_after_configread);
    BOOST_LOG_SEV(*logger, notification) << "EVERYTHING TERMINATED PROPERLY !!! GUI return code = " << ret;

    return NO_ERROR;
}
