#include "TrainSession.hpp"

TrainSession::TrainSession()
{
    //ctor
}

TrainSession::~TrainSession()
{
    //dtor
}

TrainCommSession & TrainSession::GetTrainCommSessionRef(void)
{
    return trainCommSession;
}

TrainOperationSession & TrainSession::GetTrainOperationSessionRef(void)
{
    return trainOperationSession;
}

