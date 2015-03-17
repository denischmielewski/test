#include "errors.hpp"
#include "config.hpp"
#include "log.hpp"
#include <iostream>



int main()
{
    class log * train_logs;
    class config * train_config;
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
    BOOST_LOG_SEV(lg, notification) << "Program TrainStartUp started";
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

    delete(train_config);
    delete(train_logs);
    return NO_ERROR;
}
