#ifndef TRAINPROTOBUFPOSITIONINFORMATION_H
#define TRAINPROTOBUFPOSITIONINFORMATION_H

#include <protobufpositioninformationserviceimpl.hpp>


class TrainProtobufPositionInformation : public PositionInformationImpl
{
    public:
        TrainProtobufPositionInformation(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions)
            : PositionInformationImpl(config, trainsSessions){}

        ~TrainProtobufPositionInformation();
    protected:
    private:
};

#endif // TRAINPROTOBUFPOSITIONINFORMATION_H
