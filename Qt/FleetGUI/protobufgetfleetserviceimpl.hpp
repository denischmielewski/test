#ifndef PROTOBUFGETFLEETSERVICEIMPL_HPP
#define PROTOBUFGETFLEETSERVICEIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>
#include <unordered_map>
#include "config.hpp"
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"

class GetFleetImpl : public GetFleetService
{
public:
    GetFleetImpl(config const *, std::unordered_map<std::string, TrainSession> *trainsSessions);
    config const * softwareConfig_  = nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;
    // PositionInformation() method implementation.
    void GetFleet(
        google::protobuf::RpcController *           controller,
        const GetFleetCommand *         request,
        GetFleetResponse *                response,
        google::protobuf::Closure *                 done);
};


#endif // PROTOBUFGETFLEETSERVICEIMPL_HPP
