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
