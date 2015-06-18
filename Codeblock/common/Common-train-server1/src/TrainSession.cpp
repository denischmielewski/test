#include "TrainSession.hpp"

static startup_severity_channel_logger_mt * logger;

TrainSession::TrainSession()
{
    //ctor
    startup_severity_channel_logger_mt& lg = client_comm_logger::get();
    logger = &lg;
}

TrainSession::~TrainSession()
{
    //dtor
    BOOST_LOG_SEV(*logger, notification) << "enter DESTRUCTOR TrainSession class";
    BOOST_LOG_SEV(*logger, notification) << "leave DESTRUCTOR TrainSession class";
}

TrainCommSession & TrainSession::GetTrainCommSessionRef(void)
{
    return trainCommSession;
}

TrainOperationSession & TrainSession::GetTrainOperationSessionRef(void)
{
    return trainOperationSession;
}

