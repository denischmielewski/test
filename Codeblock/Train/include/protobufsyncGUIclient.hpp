#ifndef PROTOBUFSYNCGUICLIENT_HPP
#define PROTOBUFSYNCGUICLIENT_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"


class ProtobufSyncGUIClient
{
    public:
        ProtobufSyncGUIClient();
        ProtobufSyncGUIClient(config *);
        ~ProtobufSyncGUIClient();
        void ProtobufSyncGUIClientThreadsCode(void);
        void Start();
        void Join();
    protected:
    private:
        std::thread ProtobufSyncGUIClientThread; //the default constructor is called so the thread is created.
        const config * clientconf=nullptr;
};

#endif // PROTOBUFSYNCGUICLIENT_HPP
