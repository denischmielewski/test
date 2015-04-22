#ifndef PROTOBUFSYNCHRONOUSCLIENT_HPP
#define PROTOBUFSYNCHRONOUSCLIENT_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"
#include <google/protobuf/text_format.h>

class ProtobufSynchronousClient
{
    public:
        ProtobufSynchronousClient();
        ProtobufSynchronousClient(config *);
        virtual ~ProtobufSynchronousClient();
        virtual void ProtobufSynchronousClientThreadCode(void);
        virtual void Start();
        void Join();
        const config * clientconf=nullptr;
        std::thread ProtobufSynchronousClientThread; //the default constructor is called so the thread is created.
    protected:
    private:

};

#endif // PROTOBUFSYNCHRONOUSCLIENT_HPP
