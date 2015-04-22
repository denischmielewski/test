#ifndef TRAINPROTOBUFSYNCHRONOUSSERVER_HPP
#define TRAINPROTOBUFSYNCHRONOUSSERVER_HPP

#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include "errors.hpp"
#include <unordered_map>
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

#include "trainprotobufpositioninformation.hpp"
#include "protobufpositioninformationserviceimpl.hpp"


class TrainProtobufSynchronousServer
{
    public:
        TrainProtobufSynchronousServer(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions);
        ~TrainProtobufSynchronousServer();
        void TrainProtobufSynchronousServerThreadCode(void);
        int BindProtobufServices(RCF::RcfProtoServer & rcfProtoServer);
        int GetRCFProtoServerListeningPort(void);
        void Start(void);
        void Join(void);
        const config * serverconf=nullptr;
        std::unordered_map<std::string, TrainSession> * sessions_;
    protected:
    private:
        std::thread TrainProtobufSynchronousServerThread;
};

#endif // TRAINPROTOBUFSYNCHRONOUSSERVER_HPP
