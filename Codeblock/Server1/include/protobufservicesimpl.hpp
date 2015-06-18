#ifndef PROTOBUFTRAINSERVERIMPL_HPP
#define PROTOBUFTRAINSERVERIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"

// PositionInformationService declaration.
class PositionInformationImpl : public PositionInformationService
{
public:

    // PositionInformation() method implementation.
    void PositionInformation(
        google::protobuf::RpcController *           controller,
        const PositionInformationTransmit *         request,
        PositionInformationReceive *                response,
        google::protobuf::Closure *                 done);
};

#endif // PROTOBUFTRAINSERVERIMPL_HPP
