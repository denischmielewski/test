#ifndef PROTOBUFSYNCHRONOUSCLIENTFORSERVER1_HPP
#define PROTOBUFSYNCHRONOUSCLIENTFORSERVER1_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include <unordered_map>
#include "TrainSession.hpp"

#include "RCFProtoServices.pb.h"
#include <google/protobuf/text_format.h>

class ProtobufSynchronousClientForServer1
{
    public:
        ProtobufSynchronousClientForServer1(config * const, std::unordered_map<std::string, TrainSession> * const);
        ~ProtobufSynchronousClientForServer1();
        void ProtobufSynchronousClientThreadCode(void);
        void Start(void);
        void Join();
        const config * clientconf=nullptr;
        std::unordered_map<std::string, TrainSession> * trainsSessions_;
        std::thread ProtobufSynchronousClientThread;
    protected:
    private:
};

#endif // PROTOBUFSYNCHRONOUSCLIENTFORSERVER1_HPP
