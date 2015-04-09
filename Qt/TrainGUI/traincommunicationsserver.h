#ifndef TRAINCOMMUNICATIONSSERVER_H
#define TRAINCOMMUNICATIONSSERVER_H

#include <QObject>
#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include <unordered_map>
#include "protobufservicesimpl.hpp"
#include "protobuftraininternalservicesimpl.hpp"
#include <QTimer>
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

#include<QThread>
#include <QMessageBox>
#include <QTimer>

using namespace std;
using namespace google::protobuf;

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
    void sendModeAutomatic();
    void sendModeManual();
    void onCloseTrainGUI();
    void onThreadTimerShot();

};

#endif // TRAINCOMMUNICATIONSSERVER_H
