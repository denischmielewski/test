#include "TrainOperationSession.hpp"
#include "log.hpp"

const std::string GLOBAL_CONFIG_FOLDER = "/home/train/config/global/";

TrainOperationSession::TrainOperationSession(config const * config)
{
    //ctor
    operationSessionTimed_Mutex_ = new(std::timed_mutex);
    softwareConfig_ = config;
    //retrieve BOOSTlogger
    logger_ = movement_logger::get();

}

TrainOperationSession::~TrainOperationSession()
{
    //dtor
    if(TrainOperationSessionThreadCode_.joinable()) TrainOperationSessionThreadCode_.join();
    if(TrainOperationSessionWatchdogThreadCode_.joinable()) TrainOperationSessionWatchdogThreadCode_.join();
    delete operationSessionTimed_Mutex_;
}

bool TrainOperationSession::TryLockCommSessionMutexFor(size_t milliseconds)
{
    if(operationSessionTimed_Mutex_->try_lock_for(std::chrono::milliseconds(milliseconds)))    return true;
    else return false;
}

void TrainOperationSession::UnlockCommSessionMutex(void)
{
    operationSessionTimed_Mutex_->unlock();
}

void TrainOperationSession::SetModeAutomatic(void)
{
    mode_ = AUTOMATIC;
}

void TrainOperationSession::SetModeSemiAutomatic(void)
{
    mode_ = SEMIAUTOMATIC;
}

void TrainOperationSession::SetModemanual(void)
{
    mode_ = MANUAL;
}

int16_t TrainOperationSession::getMode(void)
{
    return mode_;
}

void TrainOperationSession::SetScenario(std::string const & s)
{
    scenario_ = s;
}

std::string TrainOperationSession::getScenario(void)
{
    return scenario_;
}

void TrainOperationSession::SetPath(std::string const & s)
{
    path_ = s;
}

std::string TrainOperationSession::GetPath(void)
{
    return path_;
}

void TrainOperationSession::SetDirection(uint16_t d)
{
    direction_ = d;
}

uint16_t TrainOperationSession::GetDirection(void)
{
    return direction_;
}

void TrainOperationSession::SetKpPosition(float f)
{
    KpPosition_ = f;
}

float TrainOperationSession::GetKpPosition(void)
{
    return KpPosition_;
}

void TrainOperationSession::SetCurrentMoveStart(std::string const & s)
{
    currentMoveStart_ = s;
}

std::string TrainOperationSession::GetCurrentMoveStart(void)
{
    return currentMoveStart_;
}

void TrainOperationSession::SetCurrentMoveStop(std::string const & s)
{
    currentMoveStop_ = s;
}

std::string TrainOperationSession::GetCurrentMoveStop(void)
{
    return currentMoveStop_;
}

void TrainOperationSession::SetCurrentMoveID(uint16_t i)
{
    currentMoveID_ = i;
}

uint16_t TrainOperationSession::getCurrentMoveID(void)
{
    return currentMoveID_;
}

void TrainOperationSession::TrainOperationSessionThreadCode(void)
{

}

void TrainOperationSession::TrainOperationSessionWatchdogThreadCode(void)
{

}

void TrainOperationSession::StartTrainOperationSessionThreadCode(void)
{
    // This will start the thread. Notice move semantics!
    TrainOperationSessionThreadCode_ = std::thread(&TrainOperationSession::TrainOperationSessionThreadCode,this);
}

void TrainOperationSession::JoinTrainOperationSessionThreadCode(void)
{
    if(TrainOperationSessionThreadCode_.joinable()) TrainOperationSessionThreadCode_.join();
}

void TrainOperationSession::StartTrainOperationSessionWatchdogThreadCode(void)
{
    // This will start the thread. Notice move semantics!
    TrainOperationSessionWatchdogThreadCode_ = std::thread(&TrainOperationSession::TrainOperationSessionWatchdogThreadCode,this);
}

void TrainOperationSession::JoinTrainOperationSessionWatchdogThreadCode(void)
{
    if(TrainOperationSessionWatchdogThreadCode_.joinable()) TrainOperationSessionWatchdogThreadCode_.join();
}

int16_t TrainOperationSession::LoadTrainOperationSession(void)
{
// TODO (dev#1#15-04-30): add loading from current operational data saved on disk (case reboot for example)
    if(0)
    {
        return ERROR_LOADING_TRAIN_SESSION_OPERATION_DATA;
    }
    //development case, loading from defaultdata.xml. SHOULD NOT HAPPEN in real life !!!
    else
    {
        SetCurrentMoveStart(softwareConfig_->defaultStart_);
        SetCurrentMoveStop(softwareConfig_->defaultStop_);
        SetKpPosition(softwareConfig_->defaultKpPosition_);
        SetPath(softwareConfig_->defaultPath_);

    }
    return LoadPathSegmentsData();
}

int16_t TrainOperationSession::LoadPathSegmentsData(void)
{
    boost::property_tree::ptree pt1;
    std::ifstream pathFile;
    std::string s;

    s = GLOBAL_CONFIG_FOLDER + GetPath();
    pathFile.open(s.c_str());
    if(pathFile.fail())
    {
        BOOST_LOG_SEV(logger_, critical) << "PROBLEM opening path file : " <<  s  << " !!!";
        return ERROR_LOADING_PATH_FILE;
    }
    else
    {
        BOOST_LOG_SEV(logger_, notification) << "Path file opening OK : " <<  s  << " !";
        pathFile.close();
        try{boost::property_tree::read_xml( s, pt1 );}
        catch(const boost::property_tree::ptree_error& ex)
        {
            BOOST_LOG_SEV(logger_, critical) << "ERROR in path file : " << s << " !!!";
            BOOST_LOG_SEV(logger_, critical) << ex.what();
            return ERROR_PARSING_PATH_FILE;
        }
        BOOST_LOG_SEV(logger_, notification) << "PARSED OK path file : " << s << " !!!";
    }

    std::string node = "PATH.TOTAL_SEGMENTS";
    int16_t numSegments = stoi(pt1.get<std::string>(node), nullptr, 10);
    BOOST_LOG_SEV(logger_, notification) << "number of segments : " << numSegments;

    node = "PATH.DIRECTION";
    int16_t direction = stoi(pt1.get<std::string>(node), nullptr, 10);
    BOOST_LOG_SEV(logger_, notification) << "path direction : " << direction;

    node = "PATH.GARAGE_PARK_LINES";
    int16_t numGarageParkLines = stoi(pt1.get<std::string>(node), nullptr, 10);
    BOOST_LOG_SEV(logger_, notification) << "number of garage park lines : " << numGarageParkLines;

    node = "PATH.GARAGE_PARK_PER_GARAGE_PARK_LINE";
    int16_t numParkPerGarageParkLine = stoi(pt1.get<std::string>(node), nullptr, 10);
    BOOST_LOG_SEV(logger_, notification) << "number of park per garage line : " << numParkPerGarageParkLine;

    node = "PATH.GARAGE_PARK_LINE_OFFSET";
    int16_t garageParkLinesOffset = stoi(pt1.get<std::string>(node), nullptr, 10);
    BOOST_LOG_SEV(logger_, notification) << "garage park line offset : " << garageParkLinesOffset;

    // first : load all segments from xml file. Later we will search in which segment we are...
    struct MoveSegment ms;
    for (int j = 1 ; j <= numSegments ; j++)
    {
        try
        {
            BOOST_LOG_SEV(logger_, notification) << "================= Loading segment " << j << " .......";
            node = "PATH.SEGMENT_" + std::to_string(j);

            std::string item = node + ".START";
            ms.startLocation_ = pt1.get<std::string>(item);
            BOOST_LOG_SEV(logger_, notification) << "Start location : " << ms.startLocation_;

            item = node + ".START_Kp";
            ms.startKp_ = stof(pt1.get<std::string>(item), nullptr);
            BOOST_LOG_SEV(logger_, notification) << "Start Kp : " << ms.startKp_;

            item = node + ".STOP";
            ms.stopLocation_ = pt1.get<std::string>(item);
            BOOST_LOG_SEV(logger_, notification) << "Stop location : " << ms.stopLocation_;

            item = node + ".STOP_Kp";
            ms.stopKp_ = stof(pt1.get<std::string>(item), nullptr);
            BOOST_LOG_SEV(logger_, notification) << "Stop Kp : " << ms.stopKp_;

            item = node + ".SPEED";
            ms.speed_ = stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "segment speed : " << ms.speed_;

            item = node + ".ACC";
            ms.acceleration_= stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "segment acceleration : " << ms.acceleration_;

            item = node + ".BRK";
            ms.brake_= stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "segment brake : " << ms.brake_;

            item = node + ".BRK_DISTANCE_FROM_STOP";
            ms.brakeDistanceFromStop_ = stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "brake distance from stop : " << ms.brakeDistanceFromStop_;

            item = node + ".STOP_APPROACH_SPEED";
            ms.stopApproachSpeed_ = stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "stop approach speed : " << ms.stopApproachSpeed_;

            item = node + ".STOP_DURATION";
            ms.stopDuration_ = stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "stop duration : " << ms.stopDuration_;

            item = node + ".FORKS";
            ms.numForks_ = stoi(pt1.get<std::string>(item), nullptr, 10);
            BOOST_LOG_SEV(logger_, notification) << "number of forks : " << ms.numForks_;

            segmentsList.push_back(ms);

            BOOST_LOG_SEV(logger_, notification) << "================= Segment " << j << " loaded !!!!";
        }
        catch(const boost::property_tree::ptree_error& ex)
        {
            BOOST_LOG_SEV(logger_, critical) << "ERROR while retrieving config parameter !";
            BOOST_LOG_SEV(logger_, critical) << ex.what();
            return ERROR_PARSING_PATH_FILE;
        }
    }
    BOOST_LOG_SEV(logger_, notification) << "Path segments have been loaded. Number of segments : " << segmentsList.size();

    // second : identify current segment from last known position data
    std::list<MoveSegment>::iterator it = std::find_if  ( segmentsList.begin(), \
                                                            segmentsList.end(), \
                                                            [this] (const MoveSegment& ms) \
                                                            {
                                                                bool b = (ms.startKp_ <= this->GetKpPosition() && ms.stopKp_ > this->GetKpPosition());
                                                                return b;
                                                            }
                                                        );
    int k = std::distance(segmentsList.begin(), it);
    BOOST_LOG_SEV(logger_, notification) << "Current Segment : " << k+1; //no segment 0 so +1

    return NO_ERROR;
}
