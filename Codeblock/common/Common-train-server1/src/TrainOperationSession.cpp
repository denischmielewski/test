#include "TrainOperationSession.hpp"
#include "log.hpp"

const std::string GLOBAL_CONFIG_FOLDER = "/home/train/config/global/";

TrainOperationSession::TrainOperationSession()
{
    operationSessionTimed_Mutex_ = new(std::timed_mutex);
    logger_ = movement_logger::get();
    lastTimeTrainPositionReceived_ = std::chrono::high_resolution_clock::now();
}

TrainOperationSession::~TrainOperationSession()
{
    delete operationSessionTimed_Mutex_;
}

void TrainOperationSession::SetSoftwareConfig(config const * config)
{
    softwareConfig_ = config;
    operationSessionTimed_Mutex_ = new(std::timed_mutex);
}

bool TrainOperationSession::TryLockOperationSessionMutexFor(size_t milliseconds)
{
    if(operationSessionTimed_Mutex_->try_lock_for(std::chrono::milliseconds(milliseconds)))    return true;
    else return false;
}

void TrainOperationSession::UnlockOperationSessionMutex(void)
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

int16_t TrainOperationSession::GetMode(void)
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

uint16_t TrainOperationSession::GetCurrentSegmentID(void)
{
    return currentSegmentID_;
}

uint16_t TrainOperationSession::GetCurrentSegmentMoveStatus(void)
{
    return currentSegmentMoveStatus_;
}

void TrainOperationSession::SetCurrentSegmentMoveStatus(uint16_t i)
{
    currentSegmentMoveStatus_ = i;
}

void TrainOperationSession::SetLastTimeTrainPositionReceived(std::chrono::high_resolution_clock::time_point datetime)
{
    lastTimeTrainPositionReceived_ = datetime;
}
std::chrono::high_resolution_clock::time_point TrainOperationSession::GetLastTimeTrainPositionReceived(void)
{
    return lastTimeTrainPositionReceived_;
}

void TrainOperationSession::SetOperationSessionAsATrain(void)
{
    isATrain_ = true;
}

bool TrainOperationSession::IsThisSessionATrain(void)
{
    return isATrain_;
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

    BOOST_LOG_SEV(logger_, threads) << "+++++++++++ hello from move thread !!! +++++++++++++++";

    std::list<MoveSegment>::iterator it = segmentsList.begin();
    std::advance(it, currentSegmentID_ - 1);

    uint16_t loop = 0;
    float f = ((float)(softwareConfig_->movementThreadBeatMilliseconds_))/((float)(1000));
    //a few tmp variable because we don't want to mess with mutex in the switch-case
    uint16_t modetmp = mode_;
    uint16_t currentSegmentMoveStatustmp = currentSegmentMoveStatus_;
    float KpPositiontmp = KpPosition_;
    uint16_t currentSegmentIDtmp = currentSegmentID_;
    float currentSpeedtmp = currentSpeed_;
    uint16_t directiontmp = direction_;

    while(!g_signal_received)
    {
        loop ++;

        switch (modetmp)
        {
            case AUTOMATIC:
                switch (currentSegmentMoveStatustmp)
                {
                    case STOPPED:   //just start to accelerate. Next loop we'll calculate distance ran and speed
                        if(directiontmp == 1)
                        {
                            //first verify if we are not stopped at end of segment, might happen in case of unexpected reboot
                            if(KpPositiontmp >= (*it).stopKp_)
                            {
                                //yes we are
                                currentSegmentMoveStatustmp = ARRIVED;
                            }
                            else
                            {
                                currentSpeed_ = 0;
                                currentSegmentMoveStatustmp = ACCELERATION;
                            }
                        }
                        else if(directiontmp == 2)
                        {
                            if(KpPositiontmp >= (*it).startKp_)
                            {
                                currentSpeedtmp = 0;
                                currentSegmentMoveStatustmp = ACCELERATION;

                            }
                            else
                            {
                                currentSegmentMoveStatustmp = ARRIVED;
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown stopped movement direction :  " << directiontmp;
                        break;
                    case ACCELERATION:
                        //integrate position from speed as per direction
                        if(directiontmp == 1)
                        {
                            KpPositiontmp += currentSpeedtmp/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else if (directiontmp == 2)
                        {
                            KpPositiontmp -= currentSpeedtmp/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown movement direction :  " << directiontmp;
                        //verify if cruise speed as been reached
                        if(currentSpeedtmp >= (*it).speed_)
                        {
                            //cruise speed reached, no more acceleration
                            currentSegmentMoveStatustmp = CRUISE;
                        }
                        else
                        {
                            currentSpeedtmp += (float)((*it).acceleration_*3.6) * f;
                        }
                        //verify if brake area has been reached
                        if(directiontmp == 1)
                        {
                            if(KpPositiontmp >= (*it).stopKp_ - (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeedtmp > (*it).stopApproachSpeed_)
                                {
                                    currentSegmentMoveStatustmp = BRAKING;
                                }
                                else
                                {
                                    currentSegmentMoveStatustmp = APPROCHING;
                                    currentSpeedtmp = (*it).stopApproachSpeed_;
                                }
                            }
                        }
                        else if (directiontmp == 2)
                        {
                            if(KpPositiontmp <= (*it).startKp_ + (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeedtmp > (*it).stopApproachSpeed_)
                                {
                                    currentSegmentMoveStatustmp = BRAKING;
                                }
                                else
                                {
                                    currentSegmentMoveStatustmp = APPROCHING;
                                    currentSpeedtmp = (*it).stopApproachSpeed_;
                                }
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown acceleration movement direction :  " << directiontmp;
                        break;
                    case CRUISE:
                        //integrate position from speed as per direction
                        if(directiontmp == 1) KpPositiontmp += currentSpeedtmp/3.6 * (softwareConfig_->movementThreadBeatMilliseconds_)/1000;
                        else if (directiontmp == 2) KpPositiontmp -= currentSpeedtmp/3.6 * (softwareConfig_->movementThreadBeatMilliseconds_)/1000;
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown cruise movement direction :  " << directiontmp;
                        //verify if brake area has been reached
                        if(directiontmp == 1)
                        {
                            if(KpPositiontmp >= (*it).stopKp_ - (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeedtmp > (*it).stopApproachSpeed_)
                                {
                                    currentSegmentMoveStatustmp = BRAKING;
                                }
                                else
                                {
                                    currentSegmentMoveStatustmp = APPROCHING;
                                    currentSpeedtmp = (*it).stopApproachSpeed_/3.6;
                                }
                            }
                        }
                        else if (directiontmp == 2)
                        {
                            if(KpPositiontmp <= (*it).startKp_ + (*it).brakeDistanceFromStop_)
                            {
                                if(currentSpeedtmp > (*it).stopApproachSpeed_)
                                {
                                    currentSegmentMoveStatustmp = BRAKING;
                                }
                                else
                                {
                                    currentSegmentMoveStatustmp = APPROCHING;
                                    currentSpeedtmp = (*it).stopApproachSpeed_;
                                }
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown cruise movement direction :  " << directiontmp;
                        break;
                    case BRAKING:
                        //integrate position from speed as per direction
                        if(directiontmp == 1)
                        {
                            KpPositiontmp += (float)currentSpeedtmp/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else if (direction_ == 2)
                        {
                            KpPositiontmp -= (float)currentSpeedtmp/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown braking movement direction :  " << directiontmp;
                        //verify approaching speed reached
                        if(currentSpeedtmp <= (*it).stopApproachSpeed_)
                        {

                            currentSegmentMoveStatustmp = APPROCHING;
                        }
                        else
                        {
                            //integrate speed from brake deceleration
                            currentSpeedtmp += (float)(*it).brake_*3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        break;
                   case APPROCHING:
                        if(directiontmp == 1)
                        {
                            if(KpPositiontmp > ((*it).stopKp_ - 0.1) && KpPositiontmp < ((*it).stopKp_ + 0.1))
                            {
                                currentSegmentMoveStatustmp = ARRIVED;
                                currentSpeedtmp = 0;
                                tTimePointForStationStop = std::chrono::high_resolution_clock::now();
                            }
                        }
                        else if (directiontmp == 2)
                        {
                            if(KpPositiontmp > ((*it).startKp_ - 0.1) && KpPositiontmp < ((*it).startKp_ + 0.1))
                            {
                                currentSegmentMoveStatustmp = ARRIVED;
                                currentSpeedtmp = 0;
                                tTimePointForStationStop = std::chrono::high_resolution_clock::now();
                            }
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown approaching movement direction :  " << direction_;
                        //integrate position from speed as per direction
                        if(directiontmp == 1)
                        {
                            KpPositiontmp += (float)currentSpeedtmp/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else if (directiontmp == 2)
                        {
                            KpPositiontmp -= (float)currentSpeedtmp/3.6 * (float)(softwareConfig_->movementThreadBeatMilliseconds_)/(float)1000;
                        }
                        else BOOST_LOG_SEV(logger_, critical)    << "unknown approching movement direction :  " << directiontmp;
                        break;
                   case ARRIVED:
                        if(std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tTimePointForStationStop).count() >= (*it).stopDuration_*1000)
                        {
                            currentSegmentMoveStatustmp = STOPPED;
                            if (directiontmp == 1 && currentSegmentIDtmp < numberOfSegments_)
                            {
                                currentSegmentIDtmp++;
                                BOOST_LOG_SEV(logger_, threads)    << " *************************NEXT SEGMENT : #" \
                                                                        << currentSegmentIDtmp << " !!!*********************** " ;
                                std::advance(it, 1);
                            }
                            else if (directiontmp == 2 && currentSegmentIDtmp > 1)
                            {
                                currentSegmentIDtmp--;
                                BOOST_LOG_SEV(logger_, threads)    << " *************************NEXT SEGMENT : #" \
                                                                        << currentSegmentIDtmp << " !!!*********************** " ;

                                it = segmentsList.begin();
                                std::advance(it, currentSegmentIDtmp - 1);
                            }
                            else if (directiontmp == 1 && currentSegmentIDtmp == numberOfSegments_)
                            {
                                directiontmp = 2;
                                BOOST_LOG_SEV(logger_, threads)    << " *************************REVERSE DIRECTION : " \
                                                                        << directiontmp << " !!!*********************** " ;
                            }
                            else if (directiontmp == 2 && currentSegmentIDtmp == 1)
                            {
                                directiontmp = 1;
                                BOOST_LOG_SEV(logger_, threads)    << " *************************REVERSE DIRECTION : " \
                                                                        << directiontmp << " !!!*********************** " ;
                            }
                            else BOOST_LOG_SEV(logger_, critical) << loop << " IMPOSSIBLE direction/segmentID combination !!!" ;
                        }
                        break;
                }
                break;
            case SEMIAUTOMATIC:

                break;
            case MANUAL:

                break;
            default:
                BOOST_LOG_SEV(logger_, critical)    << "unknown train mode :  " << modetmp;
        }

        //make log user-friendly
        std::string smode;
        switch(modetmp)
        {
            case NONE: smode = "NONE";break;
            case AUTOMATIC: smode = "AUTOMATIC";break;
            case SEMIAUTOMATIC: smode = "SEMIAUTOMATIC";break;
            case MANUAL: smode = "MANUAL";break;
        }
        std::string smove;
        switch(currentSegmentMoveStatustmp)
        {
            case STOPPED: smove = "STOPPED";break;
            case ACCELERATION: smove = "ACCELERATION";break;
            case CRUISE: smove = "CRUISE";break;
            case BRAKING: smove = "BRAKING";break;
            case APPROCHING: smove = "APPROCHING";break;
            case ARRIVED: smove = "ARRIVED";break;
        }
        BOOST_LOG_SEV(logger_, debug) << loop << " Segment #" << (currentSegmentIDtmp) << " "\
                                                                    << smode << " " << smove \
                                                                    << KpPositiontmp << " Direction " << directiontmp \
                                                                    << " Speed " << currentSpeedtmp << " km/h";

        //store store operation session values with mutex lock
        if(TryLockOperationSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
        {
            mode_ = modetmp;
            currentSegmentMoveStatus_ = currentSegmentMoveStatustmp;
            KpPosition_ = KpPositiontmp;
            currentSegmentID_ = currentSegmentIDtmp;
            currentSpeed_ = currentSpeedtmp;
            direction_ = directiontmp;
            UnlockOperationSessionMutex();
        }
        else
        {
            BOOST_LOG_SEV(logger_, warning) << "Train Operation Session Lock failed !!!";
        }

        //in case of stress test : std::this_thread::sleep_for(std::chrono::milliseconds(5));

        tNow = std::chrono::high_resolution_clock::now();

        if(std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tForNotification).count() >= softwareConfig_->ThreadsLogNotificationFrequencyMilliseconds_)
        {
            BOOST_LOG_SEV(logger_, threads) << "+++++++++++ hello from move thread !!! +++++++++++++++";
            tForNotification = std::chrono::high_resolution_clock::now();
        }

        if(std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tEndBeat).count() > threadBeat)
        {
            BOOST_LOG_SEV(logger_, critical)    << "movement thread beat exceeded : " \
                                                << std::chrono::duration_cast<std::chrono::milliseconds>(tNow-tForthreadBeat).count() \
                                                << " ms measured ! Ending all threads and program !!!";
            return;
        }

        BOOST_LOG_SEV(logger_, debug)    << "movement thread will go to sleep for remaining of " \
                                                << softwareConfig_->movementThreadBeatMilliseconds_ << " milliseconds beat !";
        std::this_thread::sleep_until(tNow + std::chrono::milliseconds(threadBeat));
        BOOST_LOG_SEV(logger_, debug) << "movement thread wakes up !";
        tEndBeat = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(threadBeat);

    }

    if(g_signal_received) BOOST_LOG_SEV(logger_, threads) << "Signal received, terminating Protobuf Synchronous Client for Train GUI Thread";
}

void TrainOperationSession::TrainOperationSessionWatchdogThreadCode(void)
{

}

void TrainOperationSession::StartTrainOperationSessionThreadCode(void)
{
    trainOperationSessionThreadCode_ = new std::thread(&TrainOperationSession::TrainOperationSessionThreadCode, this);
}

void TrainOperationSession::JoinTrainOperationSessionThreadCode(void)
{
    if(trainOperationSessionThreadCode_->joinable()) trainOperationSessionThreadCode_->join();
}

void TrainOperationSession::StartTrainOperationSessionWatchdogThreadCode(void)
{
}

void TrainOperationSession::JoinTrainOperationSessionWatchdogThreadCode(void)
{
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
        BOOST_LOG_SEV(logger_, notification) << "Current Segment : " << GetCurrentSegmentID() << " Direction : " << GetDirection();
    }
    else
    {
        BOOST_LOG_SEV(logger_, critical) << "unknown or no direction : " <<  GetDirection()  << " !!!";
        return ERROR_LOADING_TRAIN_SESSION_OPERATION_DATA;
    }

    return NO_ERROR;
}
