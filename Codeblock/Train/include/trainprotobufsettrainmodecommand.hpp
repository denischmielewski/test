#ifndef TRAINPROTOBUFSETTRAINMODECOMMAND_HPP
#define TRAINPROTOBUFSETTRAINMODECOMMAND_HPP

#include <protobufsettrainmodecommandserviceimpl.hpp>


class TrainProtobufSetTrainModeCommand : public SetOperationModeImpl
{
    public:
        TrainProtobufSetTrainModeCommand(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions)
            : SetOperationModeImpl(config, trainsSessions){}
        ~TrainProtobufSetTrainModeCommand();
        void SetResponse(SetOperationModeResponse * response, google::protobuf::Closure * done);
        void UpdateSession(RCF::RcfProtoSession * pprotoSession, RCF::RcfSession &);
    protected:
    private:
};

#endif // TRAINPROTOBUFSETTRAINMODECOMMAND_HPP
