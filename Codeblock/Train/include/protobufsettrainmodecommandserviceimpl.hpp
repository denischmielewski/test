#ifndef PROTOBUFSETTRAINMODECOMMANDSERVICEIMPL_HPP
#define PROTOBUFSETTRAINMODECOMMANDSERVICEIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>
#include <unordered_map>
#include "config.hpp"
#include "TrainSession.hpp"

#include "RCFProtoServices.pb.h"

class SetOperationModeImpl : public SetOperationModeService
{
public:
    SetOperationModeImpl(config const *, std::unordered_map<std::string, TrainSession> *trainsSessions);
    config const * softwareConfig_  = nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;
    void SetOperationMode(
        google::protobuf::RpcController *           controller,
        const SetOperationModeCommand *         request,
        SetOperationModeResponse *                response,
        google::protobuf::Closure *                 done);
};


#endif // PROTOBUFSETTRAINMODECOMMANDSERVICEIMPL_HPP
