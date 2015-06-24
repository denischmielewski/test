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

//if modifying this enum is necessary, also modify code to make log user-friendly
enum mode
{
        NONE = 0,
        AUTOMATIC = 1,
        SEMIAUTOMATIC =2,
        MANUAL = 3
};
//if modifying this enum is necessary, also modify code to make log user-friendly
enum segmentMoveStatuses
{
    STOPPED = 10,
    ACCELERATION = 11,
    CRUISE = 12,
    BRAKING = 13,
    APPROCHING = 14,
    ARRIVED = 15
};

struct MoveSegment
{
        std::string startLocation_ = "start station";
        float startKp_ = 222.222;
        std::string stopLocation_ = "stop station";
        float stopKp_ = 333.333;
        float speed_ = 44;
        uint16_t acceleration_ = 9;  // km/h per second
        int16_t brake_ = -9;         // km/h per second
        uint16_t brakeDistanceFromStop_ = 111; // meters
        float stopApproachSpeed_ = 5;  // km/h per second
        uint16_t stopDuration_ = 22;    // seconds
        uint16_t numForks_ = 99;
        std::string activeForks_ = "f1;f2;";
        std::string inactiveForks_ = "f10;f20;";
};



class TrainOperationSession
{
    public:
        TrainOperationSession();
        ~TrainOperationSession();
        config const * softwareConfig_;
        void SetSoftwareConfig(config const *);
        startup_severity_channel_logger_mt logger_;
        bool TryLockOperationSessionMutexFor(size_t milliseconds);
        void UnlockOperationSessionMutex(void);
        void SetModeAutomatic(void);
        void SetModeSemiAutomatic(void);
        void SetModemanual(void);
        int16_t GetMode(void);
        void SetScenario(std::string const &);
        std::string getScenario(void);
        void SetPath(std::string const &);
        std::string GetPath(void);
        void SetDirection(uint16_t);
        uint16_t GetDirection(void);
        void SetNumberOfSegments(uint16_t);
        uint16_t GetNumberOfSegments(void);
        void SetKpPosition(float f);
        float GetKpPosition(void);
        void SetCurrentSegmentStart(std::string const &);
        std::string GetCurrentSegmentStart(void);
        void SetCurrentSegmentStop(std::string const &);
        std::string GetCurrentSegmentStop(void);
        void SetCurrentSegmentID(uint16_t);
        uint16_t GetCurrentSegmentID(void);
        void SetCurrentSegmentMoveStatus(uint16_t);
        uint16_t GetCurrentSegmentMoveStatus(void);
        void TrainOperationSessionThreadCode(void);
        void TrainOperationSessionWatchdogThreadCode(void);
        void StartTrainOperationSessionThreadCode(void);
        void JoinTrainOperationSessionThreadCode(void);
        void StartTrainOperationSessionWatchdogThreadCode(void);
        void JoinTrainOperationSessionWatchdogThreadCode(void);
        int16_t LoadTrainOperationSession(void);
        int16_t LoadPathSegmentsData(void);
        void SetLastTimeTrainPositionReceived(std::chrono::high_resolution_clock::time_point);
        std::chrono::high_resolution_clock::time_point GetLastTimeTrainPositionReceived(void);
        void SetOperationSessionAsATrain(void);
        bool IsThisSessionATrain(void);
        std::list<MoveSegment> segmentsList;
    protected:
    private:
        std::timed_mutex * operationSessionTimed_Mutex_ = nullptr;
        int16_t mode_ = 0;
        std::string scenario_ = "scenario";
        std::string path_ = "path";
        uint16_t numberOfSegments_ = 0;
        uint16_t direction_ = 0;
        float KpPosition_ = 1.1;
        std::string currentSegmentStart_ = "start";
        std::string currentSegmentStop_ = "stop";
        uint16_t currentSegmentID_ = 0;
        uint16_t currentSegmentMoveStatus_ = 0;
        float currentSpeed_ = 0;
        std::chrono::high_resolution_clock::time_point lastTimeTrainPositionReceived_;
        bool trainCommStatusOK_ = false;
        std::thread * trainOperationSessionThreadCode_;
        std::thread * TrainOperationSessionWatchdogThreadCode_;
        bool isATrain_ = false;
};

#endif // TRAINOPERATIONSESSION_HPP
