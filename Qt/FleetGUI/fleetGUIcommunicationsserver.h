#ifndef FLEETGUICOMMUNICATIONSSERVER_H
#define FLEETGUICOMMUNICATIONSSERVER_H

#include <QObject>
#include "config.hpp"
#include "log.hpp"
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include <unordered_map>
#include <QTimer>
#include "TrainSession.hpp"
#include <string>

#include "RCFProtoServices.pb.h"

#include <QThread>
#include <QMessageBox>
#include <QTimer>

class FleetGUICommunicationsServer : public QThread
{
    Q_OBJECT

public:
    ~FleetGUICommunicationsServer();
    FleetGUICommunicationsServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions);
    void run(void);

private:
    config const * serverconf=nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_;
public slots:
    void onCloseFleetGUI();
    void onThreadTimerShot();
signals:

};

#endif // FLEETGUICOMMUNICATIONSSERVER_H
