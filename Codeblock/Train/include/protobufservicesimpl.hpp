#ifndef PROTOBUFTRAINSERVERIMPL_HPP
#define PROTOBUFTRAINSERVERIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

#include <unordered_map>
#include "TrainSession.hpp"


// PositionInformationService declaration.
class PositionInformationImpl : public PositionInformationService
{
public:
    PositionInformationImpl(config const *, std::unordered_map<std::string, TrainSession> *trainsSessions);
    config const * softwareConfig_  = nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;

    // PositionInformation() method implementation.
    void PositionInformation(
        google::protobuf::RpcController *           controller,
        const PositionInformationTransmit *         request,
        PositionInformationReceive *                response,
        google::protobuf::Closure *                 done);
};

#endif // PROTOBUFTRAINSERVERIMPL_HPP
