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
        TrainOperationSession & GetTrainOperationSessionRef(void);
    protected:
    private:
        TrainCommSession        trainCommSession;
        TrainOperationSession   trainOperationSession;
};

#endif // TRAINSESSION_HPP
