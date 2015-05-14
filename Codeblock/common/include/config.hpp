#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "errors.hpp"
#include <string>

class config    //note: default value for members have no meaning. They will be overridden by values from xml config fiel.
{               //      they help howver debugging if something wrong during configuration load
    public:
        config();
        virtual ~config();
        std::string hostname_ = "";
        std::string adapter_ = "";
        std::string main_ipaddressmask_ = "";
        std::string main_ipaddress_ = "";
        std::string gui_ipaddressmask_ = "";
        std::string gui_ipaddress_ = "";
        std::string main_listener_port_ = "";
        std::string gui_listener_port_ = "";
        std::string server1_listener_port_ = "";
        int result = NO_ERROR;
        std::string boostLogCollectorFolder_ = "";
        uint16_t boostLogCollectorFolderMaxSizeInMbytes_ = 111;
        uint16_t boostLogDriveMinimumFreeSizeInMegabytes_ = 111;
        bool boostLogAutoFlush_ = true;
        uint16_t boostLogFileMaxSizeInKBytes_ = 111;
        std::string boostLogLevel_ = "";
        std::string server1_ipaddress_ = "";
        std::string server2_ipaddress_ = "";
        std::string server3_ipaddress_ = "";
        unsigned int linuxSysCallBufferSize_ = 8192;
        uint16_t commSessionMutexLockTimeoutMilliseconds_ = 111;
        uint16_t operationSessionMutexLockTimeoutMilliseconds_ = 111;
        uint16_t communicationThreadsSleepDurationMilliseconds_ = 111;
        uint32_t linuxSysCallSleepDurationMilliseconds_ = 555;
        uint32_t ThreadsLogNotificationFrequencyMilliseconds_ = 1111;
        uint32_t ThreadsExitTimeoutMilliseconds_ = 5555;
        uint32_t TCPIP_Connection_Timeout_ = 3333;
        uint32_t TCPIP_Reply_Timeout_ = 4444;
        uint32_t TrainToServer1MessagesFrequency_ = 222;
        uint32_t TrainToTrainGUIMessagesFrequency_ = 555;
        int16_t configureMainIPPortMask_(void);
        int16_t configureGUIIPPortMask_(void);
        int16_t removeMainIPPortMask_(void);
        int16_t removeGUIIPPortMask_(void);
        bool loadTrainOperationDataFromDefaultDataXmlFile_ = 0;
        int16_t defaultMode_ = 0;
        std::string defaultPath_ = "";
        int16_t defaultDirection_ = 0;
        float defaultKpPosition_ = 0.0;
        uint16_t movementThreadBeatMilliseconds_ = 22;
    protected:
    private:
};

#endif // CONFIG_HPP
