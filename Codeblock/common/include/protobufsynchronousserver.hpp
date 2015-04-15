#ifndef PROTOBUFSYNCHRONOUSSERVER_HPP
#define PROTOBUFSYNCHRONOUSSERVER_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include "errors.hpp"
#include <unordered_map>
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

class ProtobufSynchronousServer
{
    public:
        ProtobufSynchronousServer();
        ProtobufSynchronousServer(config const *, std::unordered_map<std::string, TrainSession> *);
        ~ProtobufSynchronousServer();
        virtual void ProtobufSynchronousServerThreadsCode(void);
        virtual int BindProtobufServices(RCF::RcfProtoServer & rcfProtoServer) = 0;
        virtual int GetRCFProtoServerListeningPort(void) = 0;
        void Start(void);
        void Join(void);
        const config * serverconf=nullptr;
        std::unordered_map<std::string, TrainSession> * sessions_;
    protected:
    private:
        std::thread ProtobufSynchronousServerThread; //the default constructor is called so the thread is created.
};


#endif // PROTOBUFSYNCHRONOUSSERVER_HPP
