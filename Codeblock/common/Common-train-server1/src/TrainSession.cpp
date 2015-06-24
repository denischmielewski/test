#include "TrainSession.hpp"

static startup_severity_channel_logger_mt * logger;

TrainSession::TrainSession()
{
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    logger = &lg;
}

TrainSession::~TrainSession()
{
}

TrainCommSession & TrainSession::GetTrainCommSessionRef(void)
{
    return trainCommSession;
}

TrainOperationSession & TrainSession::GetTrainOperationSessionRef(void)
{
    return trainOperationSession;
}

