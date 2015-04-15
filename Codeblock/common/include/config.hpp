#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "errors.hpp"
#include <string>

class config
{
    public:
        config();
        virtual ~config();
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
        uint16_t boostLogCollectorFolderMaxSizeInMbytes_ = 111; //111 Mbytes by default
        uint16_t boostLogDriveMinimumFreeSizeInMegabytes_ = 111;
        bool boostLogAutoFlush_ = true;
        uint16_t boostLogFileMaxSizeInKBytes_ = 111;
        std::string boostLogLevel_ = "";
        std::string server1_ipaddress_ = "";
        std::string server2_ipaddress_ = "";
        std::string server3_ipaddress_ = "";
        unsigned int linuxSysCallBufferSize_ = 8192;                    //8192 bytes by default
        uint16_t commSessionMutexLockTimeoutMilliseconds_ = 111;        //111 ms by default
        uint16_t operationSessionMutexLockTimeoutMilliseconds_ = 111;   //111 ms by default
        uint16_t communicationThreadsSleepDurationMilliseconds_ = 111;  //111 ms by default
        uint32_t linuxSysCallSleepDurationMilliseconds_ = 555;          //555 ms by default
        uint32_t ThreadsLogNotificationFrequencyMilliseconds_ = 1111;               //1111 ms by default
        uint32_t ThreadsExitTimeoutMilliseconds_ = 5555;                //5555 ms by default
        int16_t configureMainIPPortMask_(void);
        int16_t configureGUIIPPortMask_(void);
        int16_t removeMainIPPortMask_(void);
        int16_t removeGUIIPPortMask_(void);
    protected:
    private:
};

#endif // CONFIG_HPP
