#ifndef PROTOBUFSYNCSERVER_HPP
#define PROTOBUFSYNCSERVER_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"

class Session
{
    public:
        Session();
        ~Session();
        bool sessionactive_ = false;
    protected:
    private:

};

class ProtobufSyncServer
{
    public:
        ProtobufSyncServer();
        ProtobufSyncServer(config const *);
        ~ProtobufSyncServer();
        void ProtobufSyncServerThreadsCode(void);
        void Start(void);
        void Join(void);
    protected:
    private:
        std::thread ProtobufSyncServerThread; //the default constructor is called so the thread is created.
        const config * serverconf=nullptr;
        class Session session_;
};


#endif // PROTOBUFSYNCSERVER_HPP
