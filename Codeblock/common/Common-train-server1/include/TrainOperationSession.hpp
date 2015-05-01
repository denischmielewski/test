#ifndef TRAINOPERATIONSESSION_HPP
#define TRAINOPERATIONSESSION_HPP

#include <string>
#include <mutex>
#include <list>
#include <thread>
#include <chrono>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "log.hpp"
#include "config.hpp"
#include "errors.hpp"

enum mode
{
        NONE = 0,
        AUTOMATIC = 1,
        SEMIAUTOMATIC =2,
        MANUAL = 3
};

struct MoveSegment
{
        std::string startLocation_ = "start station";
        float startKp_ = 222.222;
        std::string stopLocation_ = "stop station";
        float stopKp_ = 333.333;
        uint16_t speed_ = 44;
        uint16_t acceleration_ = 9;  // km/h per second
        int16_t brake_ = -9;         // km/h per second
        uint16_t brakeDistanceFromStop_ = 111; // meters
        uint16_t stopApproachSpeed_ = 5;  // km/h per second
        uint16_t stopDuration_ = 22;    // seconds
        uint16_t numForks_ = 99;
        std::string activeForks_ = "f1;f2;";
        std::string inactiveForks_ = "f10;f20;";
};


class TrainOperationSession
{
    public:
        TrainOperationSession(config const *);
        ~TrainOperationSession();
        config const * softwareConfig_  = nullptr;
        startup_severity_channel_logger_mt logger_;
        bool TryLockCommSessionMutexFor(size_t milliseconds);
        void UnlockCommSessionMutex(void);
        void SetModeAutomatic(void);
        void SetModeSemiAutomatic(void);
        void SetModemanual(void);
        int16_t getMode(void);
        void SetScenario(std::string const &);
        std::string getScenario(void);
        void SetPath(std::string const &);
        std::string GetPath(void);
        void SetDirection(uint16_t);
        uint16_t GetDirection(void);
        void SetKpPosition(float f);
        float GetKpPosition(void);
        void SetCurrentMoveStart(std::string const &);
        std::string GetCurrentMoveStart(void);
        void SetCurrentMoveStop(std::string const &);
        std::string GetCurrentMoveStop(void);
        void SetCurrentMoveID(uint16_t);
        uint16_t getCurrentMoveID(void);
        void TrainOperationSessionThreadCode(void);
        void TrainOperationSessionWatchdogThreadCode(void);
        void StartTrainOperationSessionThreadCode(void);
        void JoinTrainOperationSessionThreadCode(void);
        void StartTrainOperationSessionWatchdogThreadCode(void);
        void JoinTrainOperationSessionWatchdogThreadCode(void);
        int16_t LoadTrainOperationSession(void);
        int16_t LoadPathSegmentsData(void);
        std::list<MoveSegment> segmentsList;
    protected:
    private:
        std::timed_mutex * operationSessionTimed_Mutex_;
        int16_t mode_ = 0;
        std::string scenario_ = "scenario";
        std::string path_ = "path";
        uint16_t direction_ = 0;
        float KpPosition_ = 1.1;
        std::string currentMoveStart_ = "start";
        std::string currentMoveStop_ = "stop";
        uint16_t currentMoveID_ = 0;
        std::thread TrainOperationSessionThreadCode_;
        std::thread TrainOperationSessionWatchdogThreadCode_;
};

#endif // TRAINOPERATIONSESSION_HPP
