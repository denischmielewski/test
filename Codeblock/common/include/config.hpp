#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "errors.hpp"
#include <string>

class config
{
    public:
        config();
        virtual ~config();
        std::string adapter = "";
        std::string ipaddress = "";
        std::string port = "";
        std::string ipaddressmask = "";
        int result = NO_ERROR;
        std::string boostLogCollectorFolder_ = "";
        uint16_t boostLogCollectorFolderMaxSizeInMbytes_ = 111; //111 Mbytes by default
        uint16_t boostLogDriveMinimumFreeSizeInMegabytes_ = 111;
        bool boostLogAutoFlush_ = true;
        uint16_t boostLogFileMaxSizeInKBytes_ = 111;
        std::string boostLogLevel_ = "";
        unsigned int linuxSysCallBufferSize_ = 8192;    //8192 bytes by default
        uint16_t commSessionMutexLockTimeoutMilliseconds_ = 111; //111 ms by default
        uint16_t operationSessionMutexLockTimeoutMilliseconds_ = 111; //111 ms by default
        uint16_t communicationThreadsSleepDurationMilliseconds_ = 111; //111 ms by default
    protected:
    private:
};

#endif // CONFIG_HPP
