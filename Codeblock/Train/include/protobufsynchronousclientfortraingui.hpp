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
        std::thread ProtobufSynchronousClientThread;
    protected:
    private:
};

#endif // PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP
