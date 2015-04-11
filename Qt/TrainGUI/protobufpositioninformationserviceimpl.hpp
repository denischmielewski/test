#ifndef PROTOBUFPOSITIONINFORMATIONSERVICEIMPL_HPP
#define PROTOBUFPOSITIONINFORMATIONSERVICEIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

#include <QObject>
#include <QString>
#include <unordered_map>
#include "TrainSession.hpp"

// PositionInformationService declaration.
class PositionInformationImpl : public QObject, public PositionInformationService
{
    Q_OBJECT
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
signals:
    void PositionReceivedFromTrain(QString);
};

#endif // PROTOBUFPOSITIONINFORMATIONSERVICEIMPL_HPP
