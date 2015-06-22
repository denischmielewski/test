#ifndef FLEETGUICOMMUNICATIONCLIENT_HPP
#define FLEETGUICOMMUNICATIONCLIENT_HPP

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

class FleetGUICommunicationClient : public QThread
{
    Q_OBJECT
public:
    ~FleetGUICommunicationClient();
    FleetGUICommunicationClient(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions);
    void run(void);
private:
    GetFleetCommand getFleetCommand_;
    GetFleetResponse getFleetResponse_;
    GetFleetService::Stub * getFleetServiceStub_;
    config const * clientconf=nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_;
public slots:
    void onCloseFleetGUI();
    void onTimerForClientThreadNotificationShot();
    void onTimerForClientToServer1GetFleetShot();
};

#endif // FLEETGUICOMMUNICATIONCLIENT_HPP
