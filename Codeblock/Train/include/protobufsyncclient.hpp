#ifndef PROTOBUFSYNCCLIENT_HPP
#define PROTOBUFSYNCCLIENT_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"


class ProtobufSyncClient
{
    public:
        ProtobufSyncClient();
        ProtobufSyncClient(config *);
        ~ProtobufSyncClient();
        void ProtobufSyncClientThreadsCode(void);
        void Start();
        void Join();
    protected:
    private:
        std::thread ProtobufSyncClientThread; //the default constructor is called so the thread is created.
        const config * clientconf=nullptr;
};

#endif // PROTOBUFSYNCCLIENT_HPP
