#ifndef PROTOBUFTRAININTERNALIMPL_HPP
#define PROTOBUFTRAININTERNALIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>
#include <unordered_map>
#include "config.hpp"
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

// PositionInformationService declaration.
class SetOperationModeImpl : public SetOperationModeService
{
public:
    SetOperationModeImpl(config const *, std::unordered_map<std::string, TrainSession> *trainsSessions);
    config const * softwareConfig_  = nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;
    // PositionInformation() method implementation.
    void SetOperationMode(
        google::protobuf::RpcController *           controller,
        const SetOperationModeCommand *         request,
        SetOperationModeResponse *                response,
        google::protobuf::Closure *                 done);
};


#endif // PROTOBUFTRAININTERNALIMPL_HPP
