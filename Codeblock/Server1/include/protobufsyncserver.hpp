#ifndef PROTOBUFSYNCSERVER_HPP
#define PROTOBUFSYNCSERVER_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"

class ProtobufSyncServer
{
    public:
        ProtobufSyncServer();
        ProtobufSyncServer(const config * conf);
        ~ProtobufSyncServer();
        void ProtobufSyncServerThreadsCode(void);
        void Start()
        {
            // This will start the thread. Notice move semantics!
            ProtobufSyncServerThread = std::thread(&ProtobufSyncServer::ProtobufSyncServerThreadsCode,this);
        }
    protected:
    private:
        std::thread ProtobufSyncServerThread; //the default constructor is called so the thread is created.
        const config * serverconf=nullptr;
};

#endif // PROTOBUFSYNCSERVER_HPP
