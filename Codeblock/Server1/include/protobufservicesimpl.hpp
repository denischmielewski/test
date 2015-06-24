#ifndef PROTOBUFTRAINSERVERIMPL_HPP
#define PROTOBUFTRAINSERVERIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>

#include "RCFProtoServices.pb.h"

class PositionInformationImpl : public PositionInformationService
{
public:

    void PositionInformation(
        google::protobuf::RpcController *           controller,
        const PositionInformationTransmit *         request,
        PositionInformationReceive *                response,
        google::protobuf::Closure *                 done);
};

#endif // PROTOBUFTRAINSERVERIMPL_HPP
