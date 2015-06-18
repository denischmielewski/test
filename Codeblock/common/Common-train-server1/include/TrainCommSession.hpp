#ifndef TRAINCOMMSESSION_HPP
#define TRAINCOMMSESSION_HPP

#include <string>
#include <mutex>
#include <chrono>
#include <RCFProto.hpp>
#include "log.hpp"

#define COMM_SESSION_ACTIVE 1
#define COMM_SESSION_INACTIVE 0

class TrainCommSession
{
    public:
        TrainCommSession();
        ~TrainCommSession();
        void SetIpAddress(const std::string);
        void SetSessionActive(void);
        void SetSessionInactive(void);
        void SetSessionConnectionDuration(size_t);
        void SetSessionRemoteCallCount(size_t);
        void SetSessionTotalBytesReceived(uint64_t);
        void SetSessionTotalBytesSent(uint64_t);
        void SetSessionConnectionTime(time_t);
        const std::string & GetIpAddress(void);
        bool GetSessionState(void);
        size_t GetSessionConnectionDuration(void);
        size_t GetSessionRemoteCallCount(void);
        uint64_t GetSessionTotalBytesReceived(void);
        uint64_t GetSessionTotalBytesSent(void);
        size_t GetSessionConnectionLossCount(void);
        time_t GetSessionConnectionTime(void);
        bool TryLockCommSessionMutexFor(size_t milliseconds);
        void UnlockCommSessionMutex(void);
        void IncConnectionLossCount(void);
    protected:
    private:
        std::string     commSessionIPAddress = "Not set !!!";
        bool    commSessionState = COMM_SESSION_INACTIVE;
        size_t  commSessionConnectionDuration = 0;
        size_t  commSessionRemoteCallCount = 0;
        uint64_t    commSessionTotalBytesReceived = 0;
        uint64_t    commSessionTotalBytesSent = 0;
        size_t  commSessionConnectionLossCount = 0;
        time_t commSessionConnectionTime = 0;
        std::timed_mutex * pcommSessionTimed_Mutex;

};

#endif // TRAINCOMMSESSION_HPP
