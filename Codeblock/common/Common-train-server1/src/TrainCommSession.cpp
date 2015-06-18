#include "TrainCommSession.hpp"

static startup_severity_channel_logger_mt * logger;

TrainCommSession::TrainCommSession()
{
    //ctor
    pcommSessionTimed_Mutex = new(std::timed_mutex);
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    logger = &lg;
}

TrainCommSession::~TrainCommSession()
{
    BOOST_LOG_SEV(*logger, notification) << "enter DESTRUCTOR TrainCommSession class";
    //dtor
    delete pcommSessionTimed_Mutex;
    BOOST_LOG_SEV(*logger, notification) << "leave DESTRUCTOR TrainCommSession class";
}

//=========SET members===============================================================

void TrainCommSession::SetIpAddress(const std::string ipaddress)
{
    commSessionIPAddress = ipaddress;
}

void TrainCommSession::SetSessionActive(void)
{
     commSessionState = COMM_SESSION_ACTIVE;
}

void TrainCommSession::SetSessionInactive(void)
{
    commSessionState = COMM_SESSION_INACTIVE;
    commSessionConnectionLossCount++;
}

void TrainCommSession::IncConnectionLossCount(void)
{
    commSessionConnectionLossCount++;
}

void TrainCommSession::SetSessionConnectionDuration(size_t duration)
{
    commSessionConnectionDuration = duration;
}


void TrainCommSession::SetSessionRemoteCallCount(size_t callcount)
{
    commSessionRemoteCallCount = callcount;
}

void TrainCommSession::SetSessionTotalBytesReceived(uint64_t received)
{
    commSessionTotalBytesReceived = received;
}

void TrainCommSession::SetSessionTotalBytesSent(uint64_t sent)
{
    commSessionTotalBytesSent = sent;
}

//=========GET members===============================================================

const std::string & TrainCommSession::GetIpAddress(void)
{
    return commSessionIPAddress;
}

bool TrainCommSession::GetSessionState(void)
{
    return commSessionState;
}

size_t TrainCommSession::GetSessionConnectionDuration(void)
{
    return commSessionConnectionDuration;
}

size_t TrainCommSession::GetSessionRemoteCallCount(void)
{
    return commSessionRemoteCallCount;
}

uint64_t TrainCommSession::GetSessionTotalBytesReceived(void)
{
    return commSessionTotalBytesReceived;
}

uint64_t TrainCommSession::GetSessionTotalBytesSent(void)
{
    return commSessionTotalBytesSent;
}

size_t TrainCommSession::GetSessionConnectionLossCount(void)
{
    return commSessionConnectionLossCount;
}

time_t TrainCommSession::GetSessionConnectionTime(void)
{
    return commSessionConnectionTime;
}

void TrainCommSession::SetSessionConnectionTime(time_t timeraw)
{
    commSessionConnectionTime = timeraw;
}

//=========OTHER members===============================================================

bool TrainCommSession::TryLockCommSessionMutexFor(size_t milliseconds)
{
    if(pcommSessionTimed_Mutex->try_lock_for(std::chrono::milliseconds(milliseconds)))    return true;
    else return false;
}

void TrainCommSession::UnlockCommSessionMutex(void)
{
    pcommSessionTimed_Mutex->unlock();
}



