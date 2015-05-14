#include "TrainOperationSession.hpp"
#include "log.hpp"

const std::string GLOBAL_CONFIG_FOLDER = "/home/train/config/global/";

TrainOperationSession::TrainOperationSession()
{
    //ctor
    operationSessionTimed_Mutex_ = new(std::timed_mutex);
    //retrieve BOOSTlogger
    logger_ = movement_logger::get();
}

TrainOperationSession::~TrainOperationSession()
{
    //dtor
    if(trainOperationSessionThreadCode_->joinable()) trainOperationSessionThreadCode_->join();
    //if(TrainOperationSessionWatchdogThreadCode_->joinable()) TrainOperationSessionWatchdogThreadCode_->join();
    delete operationSessionTimed_Mutex_;
}

void TrainOperationSession::SetSoftwareConfig(config const * config)
{
    softwareConfig_ = config;
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

void TrainOperationSession::SetNumberOfSegments(uint16_t ns)
{
    numberOfSegments_ = ns;
}

uint16_t TrainOperationSession::GetNumberOfSegments(void)
{
    return numberOfSegments_;
}

void TrainOperationSession::SetKpPosition(float f)
{
    KpPosition_ = f;
}

float TrainOperationSession::GetKpPosition(void)
{
    return KpPosition_;
}

void TrainOperationSession::SetCurrentSegmentStart(std::string const & s)
{
    currentSegmentStart_ = s;
}

std::string TrainOperationSession::GetCurrentSegmentStart(void)
{
    return currentSegmentStart_;
}

void TrainOperationSession::SetCurrentSegmentStop(std::string const & s)
{
    currentSegmentStop_ = s;
}

std::string TrainOperationSession::GetCurrentSegmentStop(void)
{
    return currentSegmentStop_;
}

void TrainOperationSession::SetCurrentSegmentID(uint16_t i)
{
    currentSegmentID_ = i;
}

uint16_t TrainOperationSession::getCurrentSegmentID(void)
{
    return currentSegmentID_;
}

void TrainOperationSession::TrainOperationSessionThreadCode(void)
{
    extern volatile int g_signal_received;
    std::chrono::high_resolution_clock::time_point tEndBeat = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point tNow;
    std::chrono::high_resolution_clock::time_point tForthreadBeat = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point tForNotification = std::chrono::high_resolution_clock::now();
    uint16_t threadBeat = softwareConfig_->movementThreadBeatMilliseconds_;
    std::chrono::high_resolution_clock::time_point tTimePointForStationStop;

    BOOST_LOG_SEV(logger_, notification) << "+++++++++++ hello from movement thread !!! +++++++++++++++";

    std::list<MoveSegment>::iterator it = segmentsList.begin();
    std::advance(it, currentSegmentID_ - 1);

    uint16_t loop = 0;
    float f = ((float)(softwareConfig_->movementThreadBeatMilliseconds_))/((float)(1000));

    while(!g_signal_received)
    {
        loop ++;

        switch (mode_)
        {
            case AUTOMATIC:
                switch (currentSegmentMoveStatus_)
                {
                    case STOPPED:   //just start to accelerate. Next loop we'll calculate distance ran and speed
                        //BOOST_LOG_SEV(logger_, notification) << loop << "*************************enter STOPPED STATUS !!!  ";
                        if(direction_ == 1)
                        {
                            //first verify if we are not stopped at end of segment, might happen in case of unexpected reboot
                            if(KpPosition_ >= (*it).stopKp_)
                            {
                                //yes we are
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train arrived to Stop :" << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSegmentMoveStatus_ = ARRIVED;
                            }
                            else
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " acceleration from position :" << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSpeed_ = 0;
                                currentSegmentMoveStatus_ = ACCELERATION;
                            }
                        }
                        else if(direction_ == 2)
                        {
                            if(KpPosition_ >= (*it).startKp_)
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " acceleration from position :" << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSpeed_ = 0;
                                currentSegmentMoveStatus_ = ACCELERATION;

                            }
                            else
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train arrived to Stop " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSegmentMoveStatus_ = ARRIVED;
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        break;
                    case ACCELERATION:
                        //integrate position from speed as per direction
                        if(direction_ == 1)
                        {
                            KpPosition_ += currentSpeed_/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                            //BOOST_LOG_SEV(logger_, notification) << loop << " position " << KpPosition_;
                        }
                        else if (direction_ == 2)
                        {
                            KpPosition_ -= currentSpeed_/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        //verify if cruise speed as been reached
                        //BOOST_LOG_SEV(logger_, notification) << loop << " speed " << currentSpeed_ << " cruise speed " << (*it).speed_;
                        if(currentSpeed_ >= (*it).speed_)
                        {
                            //cruise speed reached, no more acceleration
                            BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train cruising " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                            currentSegmentMoveStatus_ = CRUISE;
                        }
                        else
                        {
                            currentSpeed_ += (float)((*it).acceleration_*3.6) * f;
                        }                        //verify if brake area has been reached
                        if(direction_ == 1)
                        {
                            if(KpPosition_ >= (*it).stopKp_ - (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeed_ > (*it).stopApproachSpeed_)
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train braking1 " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = BRAKING;
                                }
                                else
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train approaching stop " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = APPROCHING;
                                    currentSpeed_ = (*it).stopApproachSpeed_;
                                }
                            }
                        }
                        else if (direction_ == 2)
                        {
                            if(KpPosition_ <= (*it).startKp_ + (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeed_ > (*it).stopApproachSpeed_)
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train braking2 " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = BRAKING;
                                }
                                else
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train approaching stop " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = APPROCHING;
                                    currentSpeed_ = (*it).stopApproachSpeed_;
                                }
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        break;
                    case CRUISE:
                        //BOOST_LOG_SEV(logger_, notification) << loop << "*************************enter CRUISE STATUS !!!  ";
                        //integrate position from speed as per direction
                        if(direction_ == 1) KpPosition_ += currentSpeed_/3.6 * (softwareConfig_->movementThreadBeatMilliseconds_)/1000;
                        else if (direction_ == 2) KpPosition_ -= currentSpeed_/3.6 * (softwareConfig_->movementThreadBeatMilliseconds_)/1000;
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        //verify if brake area has been reached
                        if(direction_ == 1)
                        {
                            if(KpPosition_ >= (*it).stopKp_ - (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeed_ > (*it).stopApproachSpeed_)
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train braking3 " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    BOOST_LOG_SEV(logger_, notification) << loop << "  " << currentSegmentMoveStatus_ << " " <<  "Segment #" << (currentSegmentID_) << " Train braking3 :" << KpPosition_;
                                    currentSegmentMoveStatus_ = BRAKING;
                                }
                                else
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train approaching stop " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = APPROCHING;
                                    currentSpeed_ = (*it).stopApproachSpeed_/3.6;
                                }
                            }
                        }
                        else if (direction_ == 2)
                        {
                            if(KpPosition_ <= (*it).startKp_ + (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeed_ > (*it).stopApproachSpeed_)
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train braking4 " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = BRAKING;
                                }
                                else
                                {
                                    BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train approaching stop " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                    currentSegmentMoveStatus_ = APPROCHING;
                                    currentSpeed_ = (*it).stopApproachSpeed_;
                                }
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        break;
                    case BRAKING:
                        //BOOST_LOG_SEV(logger_, notification) << loop << "*************************enter BRAKING STATUS !!!  ";
                        //integrate position from speed as per direction
                        if(direction_ == 1)
                        {
                            KpPosition_ += (float)currentSpeed_/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else if (direction_ == 2)
                        {
                            KpPosition_ -= (float)currentSpeed_/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        //verify approaching speed reached
                        if(currentSpeed_ <= (*it).stopApproachSpeed_)
                        {

                            BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train approaching stop " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                            currentSegmentMoveStatus_ = APPROCHING;
                        }
                        else
                        {
                            //integrate speed from brake deceleration
                            currentSpeed_ += (float)(*it).brake_*3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        //BOOST_LOG_SEV(logger_, notification) << loop << " position " << KpPosition_;
                        //BOOST_LOG_SEV(logger_, notification) << loop << " speed " << currentSpeed_ << " km/h  brake force " << (*it).brake_ << " m/s/s";
                        break;
                   case APPROCHING:
                        //BOOST_LOG_SEV(logger_, notification) << loop << "*************************enter APPROCHING STATUS !!!  ";
                        //cruise at approach speed until stop location is reached
                        if(direction_ == 1)
                        {
                            if(KpPosition_ > ((*it).stopKp_ - 0.1) && KpPosition_ < ((*it).stopKp_ + 0.1))
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train Arrived1 " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSegmentMoveStatus_ = ARRIVED;
                                currentSpeed_ = 0;
                                tTimePointForStationStop = std::chrono::high_resolution_clock::now();
                            }
                        }
                        else if (direction_ == 2)
                        {
                            if(KpPosition_ > ((*it).startKp_ - 0.1) && KpPosition_ < ((*it).startKp_ + 0.1))
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " Train Arrived2 " << KpPosition_ << " Direction " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSegmentMoveStatus_ = ARRIVED;
                                currentSpeed_ = 0;
                                tTimePointForStationStop = std::chrono::high_resolution_clock::now();
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        //integrate position from speed as per direction
                        if(direction_ == 1)
                        {
                            KpPosition_ += (float)currentSpeed_/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else if (direction_ == 2)
                        {
                            KpPosition_ -= (float)currentSpeed_/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << direction_;
                        //BOOST_LOG_SEV(logger_, notification) << loop << " position " << KpPosition_;
                        //BOOST_LOG_SEV(logger_, notification) << loop << " speed " << currentSpeed_ << " approach speed " << (*it).stopApproachSpeed_;
                        break;
                   case ARRIVED:
                        //BOOST_LOG_SEV(logger_, notification) << loop << "*************************enter ARRIVED STATUS !!!  ";
                        //stop for specified configured time
                        if(std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tTimePointForStationStop).count() >= (*it).stopDuration_*1000)
                        {
                            currentSegmentMoveStatus_ = STOPPED;
                            if (direction_ == 1 && currentSegmentID_ < numberOfSegments_)
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " *************************next segment !!! " << KpPosition_ << " New Direction = " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSegmentID_++;
                                std::advance(it, 1);
                            }
                            else if (direction_ == 2 && currentSegmentID_ > 1)
                            {
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " *************************next segment !!! " << KpPosition_ << " New Direction = " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                                currentSegmentID_--;
                                it = segmentsList.begin();
                                std::advance(it, currentSegmentID_ - 1);

                            }
                            if (direction_ == 1 && currentSegmentID_ == numberOfSegments_)
                            {
                                direction_ = 2;
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " *************************reverse direction !!! " << KpPosition_ << " New Direction = " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                            }
                            if (direction_ == 2 && currentSegmentID_ == 1)
                            {
                                direction_ = 1;
                                BOOST_LOG_SEV(logger_, notification) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " *************************reverse direction !!! " << KpPosition_ << " New Direction = " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                            }
                            else BOOST_LOG_SEV(logger_, critical) << loop << " Segment #" << (currentSegmentID_) \
                                                                    << " IMPOSSIBLE direction/segmentID combination !!!" << KpPosition_ << " Direction = " << direction_ \
                                                                    << " Speed " << currentSpeed_ << " km/h";
                        }
                        break;
                }
                break;
            case SEMIAUTOMATIC:

                break;
            case MANUAL:

                break;
            default:
                BOOST_LOG_SEV(logger_, critical)    << "unknown train mode :  " << mode_;
        }


        //std::this_thread::sleep_for(std::chrono::milliseconds(5));

        tNow = std::chrono::high_resolution_clock::now();

        if(std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tForNotification).count() >= softwareConfig_->ThreadsLogNotificationFrequencyMilliseconds_)
        {
            BOOST_LOG_SEV(logger_, notification) << "+++++++++++ hello from move thread !!! +++++++++++++++";
            tForNotification = std::chrono::high_resolution_clock::now();
        }

        if(std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tEndBeat).count() > threadBeat)
        {
            BOOST_LOG_SEV(logger_, critical)    << "movement thread beat exceeded : " \
                                                << std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tForthreadBeat).count() \
                                                << " ms measured ! Ending all threads and program !!!";
            //g_signal_received = 1;
            return;
        }

        BOOST_LOG_SEV(logger_, debug)    << "movement thread will go to sleep for remaining of " \
                                                << softwareConfig_->movementThreadBeatMilliseconds_ << " milliseconds beat !";
        std::this_thread::sleep_until(tNow + std::chrono::milliseconds(threadBeat));
        BOOST_LOG_SEV(logger_, debug) << "movement thread wakes up !";
        tEndBeat = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(threadBeat);

    }

    if(g_signal_received) BOOST_LOG_SEV(logger_, notification) << "Signal received, terminating Protobuf Synchronous Client for Train GUI Thread";
}

void TrainOperationSession::TrainOperationSessionWatchdogThreadCode(void)
{

}

void TrainOperationSession::StartTrainOperationSessionThreadCode(void)
{
    // This will start the thread. Notice move semantics!
    //std::thread t = std::thread(&TrainOperationSession::TrainOperationSessionThreadCode, this);
    trainOperationSessionThreadCode_ = new std::thread(&TrainOperationSession::TrainOperationSessionThreadCode, this);
}

void TrainOperationSession::JoinTrainOperationSessionThreadCode(void)
{
    if(trainOperationSessionThreadCode_->joinable()) trainOperationSessionThreadCode_->join();
}

void TrainOperationSession::StartTrainOperationSessionWatchdogThreadCode(void)
{
    // This will start the thread. Notice move semantics!
    //TrainOperationSessionWatchdogThreadCode_ = std::thread(&TrainOperationSession::TrainOperationSessionWatchdogThreadCode,this);
}

void TrainOperationSession::JoinTrainOperationSessionWatchdogThreadCode(void)
{
    //if(TrainOperationSessionWatchdogThreadCode_.joinable()) TrainOperationSessionWatchdogThreadCode_.join();
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
        if(softwareConfig_->defaultMode_ == AUTOMATIC) SetModeAutomatic();
        else if(softwareConfig_->defaultMode_ == SEMIAUTOMATIC) SetModeSemiAutomatic();
        else if(softwareConfig_->defaultMode_ == MANUAL) SetModemanual();
        else {BOOST_LOG_SEV(logger_, critical) << "unknown or no default mode : " <<  softwareConfig_->defaultMode_  << " !!!";return ERROR_LOADING_TRAIN_SESSION_OPERATION_DATA;}
        SetKpPosition(softwareConfig_->defaultKpPosition_);
        SetPath(softwareConfig_->defaultPath_);
        SetDirection(softwareConfig_->defaultDirection_);
        currentSegmentMoveStatus_ = STOPPED;
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
    SetNumberOfSegments(segmentsList.size());

    // second : identify current segment from last known position data
    // we need to handle both direction because if position matches a segment start or stop, the segment will be the one for which position matches startKp_
    if(GetDirection() == 1 || GetDirection() == 2)
    {
        std::list<MoveSegment>::iterator it = std::find_if  ( segmentsList.begin(), \
                                                            segmentsList.end(), \
                                                            [this] (const MoveSegment& ms) \
                                                            {
                                                                bool b = (ms.startKp_ <= this->GetKpPosition() && ms.stopKp_ > this->GetKpPosition());
                                                                return b;
                                                            }
                                                        );
        int k = std::distance(segmentsList.begin(), it);
        if(GetDirection() == 1) SetCurrentSegmentID(k+1);   //no segment 0 so +1
        if(GetDirection() == 2) SetCurrentSegmentID(k);
        BOOST_LOG_SEV(logger_, notification) << "Current Segment : " << getCurrentSegmentID() << " Direction : " << GetDirection();
    }
    else
    {
        BOOST_LOG_SEV(logger_, critical) << "unknown or no direction : " <<  GetDirection()  << " !!!";
        return ERROR_LOADING_TRAIN_SESSION_OPERATION_DATA;
    }

    return NO_ERROR;
}

