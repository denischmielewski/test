#ifndef PROTOBUFGETFLEETIMPL_HPP
#define PROTOBUFGETFLEETIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>
#include <unordered_map>
#include "config.hpp"
#include "TrainSession.hpp"

#include "RCFProtoServices.pb.h"

class GetFleetImpl : public GetFleetService
{
public:
    GetFleetImpl(config const *, std::unordered_map<std::string, TrainSession> *trainsSessions);
    config const * softwareConfig_  = nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;
    void GetFleet(
        google::protobuf::RpcController *   controller,
        const GetFleetCommand *             request,
        GetFleetResponse *           response,
        google::protobuf::Closure *         done);
};


#endif // PROTOBUFTRAININTERNALIMPL_HPP
