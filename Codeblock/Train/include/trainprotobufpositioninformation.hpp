#ifndef TRAINPROTOBUFPOSITIONINFORMATION_H
#define TRAINPROTOBUFPOSITIONINFORMATION_H

#include <protobufpositioninformationserviceimpl.hpp>


class TrainProtobufPositionInformation : public PositionInformationImpl
{
    public:
        TrainProtobufPositionInformation(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions)
            : PositionInformationImpl(config, trainsSessions){}
        ~TrainProtobufPositionInformation();
        void SetResponse(PositionInformationReceive * response, google::protobuf::Closure * done);
        void UpdateSession(RCF::RcfProtoSession * pprotoSession, RCF::RcfSession &);
    protected:
    private:
};

#endif // TRAINPROTOBUFPOSITIONINFORMATION_H
