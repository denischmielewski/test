#ifndef TRAINSESSION_HPP
#define TRAINSESSION_HPP

#include "TrainCommSession.hpp"
#include "TrainOperationSession.hpp"

class TrainSession
{
    public:
        TrainSession();
        ~TrainSession();
        TrainCommSession & GetTrainCommSessionRef(void);
    protected:
    private:    //as trainSeesion will be used as a static singleton I put everything private
        TrainCommSession        trainCommSession;
        TrainOperationSession   trainOperationSession;
};

#endif // TRAINSESSION_HPP
