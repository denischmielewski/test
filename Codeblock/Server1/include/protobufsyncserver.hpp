#ifndef PROTOBUFSYNCSERVER_HPP
#define PROTOBUFSYNCSERVER_HPP

#include "config.hpp"
#include "log.hpp"
#include <unordered_map>
#include "TrainSession.hpp"

#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"

class ProtobufSyncServer
{
    public:
        ProtobufSyncServer();
        ProtobufSyncServer(config const *, std::unordered_map<std::string, TrainSession> *);
        ~ProtobufSyncServer();
        void ProtobufSyncServerThreadsCode(void);
        void Start(void);
        void Join(void);
    protected:
    private:
        std::thread ProtobufSyncServerThread; //the default constructor is called so the thread is created.
        const config * serverconf_ =nullptr;
        std::unordered_map<std::string, TrainSession> * trainsSessions_ = nullptr;
};


#endif // PROTOBUFSYNCSERVER_HPP
