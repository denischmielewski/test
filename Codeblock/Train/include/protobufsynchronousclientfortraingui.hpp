#ifndef PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP
#define PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>
#include <unordered_map>
#include "TrainSession.hpp"

// Include protoc-generated header.
#include "RCFProtoServices.pb.h"
#include <google/protobuf/text_format.h>

class ProtobufSynchronousClientForTrainGUI
{
    public:
        ProtobufSynchronousClientForTrainGUI(config * config, std::unordered_map<std::string, TrainSession> * const);
        ~ProtobufSynchronousClientForTrainGUI();
        void ProtobufSynchronousClientThreadCode(void);
        void Start();
        void Join();
        const config * clientconf=nullptr;
        std::unordered_map<std::string, TrainSession> * trainsSessions_;
        std::thread ProtobufSynchronousClientThread; //the default constructor is called so the thread is created.
    protected:
    private:
};

#endif // PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP
