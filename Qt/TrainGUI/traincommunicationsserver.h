#ifndef TRAINCOMMUNICATIONSSERVER_H
#define TRAINCOMMUNICATIONSSERVER_H

#include <QObject>
#include "config.hpp"
#include "log.hpp"
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include <unordered_map>
#include "protobufpositioninformationserviceimpl.hpp"
#include "protobufsettrainmodecommandserviceimpl.hpp"
#include <QTimer>
#include "TrainSession.hpp"
#include <string>

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"

#include <QThread>
#include <QMessageBox>
#include <QTimer>

class TrainCommunicationsServer : public QThread
{
    Q_OBJECT

public:
    ~TrainCommunicationsServer();
    TrainCommunicationsServer(config const * conf, std::unordered_map<std::string, TrainSession> * trainsSessions);
    void run(void);

private:
    config const * serverconf=nullptr;
    std::unordered_map<std::string, TrainSession> * trainsSessions_;
    //class Session session_;
public slots:
//    void sendModeAutomatic();
//    void sendModeManual();
    void onCloseTrainGUI();
    void onThreadTimerShot();
    void onPositionReceivedFromTrain(std::string s);
signals:

};

#endif // TRAINCOMMUNICATIONSSERVER_H
