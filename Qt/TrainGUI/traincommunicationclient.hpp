#ifndef TRAINCOMMUNICATIONCLIENT_HPP
#define TRAINCOMMUNICATIONCLIENT_HPP

#include <QObject>
#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QString>

#include <chrono>
#include <ctime>
#include <unordered_map>

#include <RCFProto.hpp>

#include "config.hpp"
#include "log.hpp"
#include "protobufpositioninformationserviceimpl.hpp"
#include "protobufsettrainmodecommandserviceimpl.hpp"
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"

class TrainCommunicationClient : public QThread
{
    Q_OBJECT
public:
    ~TrainCommunicationClient();
    TrainCommunicationClient(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions);
    void run(void);
private:
    //SetOperationModeCommand * operationModeRequest_ = 0;
    //SetOperationModeResponse * operationModeResponse_ = 0;
    SetOperationModeCommand operationModeRequest_;
    SetOperationModeResponse operationModeResponse_;
    SetOperationModeService::Stub * setOperationModeServiceStub_;
    config const * clientconf=nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_;
public slots:
    void onCloseTrainGUI();
    void onThreadTimerShot();
    void onChangeModeToManual();
    void onChangeModeToAutomatic();
};

#endif // TRAINCOMMUNICATIONCLIENT_HPP
