#ifndef PROTOBUFPOSITIONINFORMATIONSERVICEIMPL_HPP
#define PROTOBUFPOSITIONINFORMATIONSERVICEIMPL_HPP

#include "log.hpp"
#include <RCFProto.hpp>

#include "RCFProtoServices.pb.h"

#include <QObject>
#include <string>
#include <unordered_map>
#include "TrainSession.hpp"

class PositionInformationImpl : public QObject, public PositionInformationService
{
    Q_OBJECT
public:
    PositionInformationImpl();
    config const * softwareConfig_  = nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;
    void PositionInformation(
        google::protobuf::RpcController *           controller,
        const PositionInformationTransmit *         request,
        PositionInformationReceive *                response,
        google::protobuf::Closure *                 done);
    void SetSoftwareConfigPointer(config const *);
    void SetTrainSessionpointer(std::unordered_map<std::string, TrainSession> *);
signals:
    void PositionReceivedFromTrain(float);
};

#endif // PROTOBUFPOSITIONINFORMATIONSERVICEIMPL_HPP
