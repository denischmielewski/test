#ifndef PROTOBUFTRAININTERNALIMPL_HPP
#define PROTOBUFTRAININTERNALIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

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

// PositionInformationService declaration.
class SetOperationModeImpl : public SetOperationModeService
{
public:

    // PositionInformation() method implementation.
    void SetOperationMode(
        google::protobuf::RpcController *           controller,
        const SetOperationModeCommand *         request,
        SetOperationModeResponse *                response,
        google::protobuf::Closure *                 done);
};


#endif // PROTOBUFTRAININTERNALIMPL_HPP
