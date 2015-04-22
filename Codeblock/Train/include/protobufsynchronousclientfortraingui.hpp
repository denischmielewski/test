#ifndef PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP
#define PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP

#include "config.hpp"
#include "log.hpp"
#include <thread>
#include <chrono>
#include <RCFProto.hpp>
#include <ctime>

// Include protoc-generated header.
#include "TestRCFProto.pb.h"
#include <google/protobuf/text_format.h>

class ProtobufSynchronousClientForTrainGUI
{
    public:
        ProtobufSynchronousClientForTrainGUI(config * config);
        ~ProtobufSynchronousClientForTrainGUI();
        void ProtobufSynchronousClientThreadCode(void);
        void Start();
        void Join();
        const config * clientconf=nullptr;
        std::thread ProtobufSynchronousClientThread; //the default constructor is called so the thread is created.
    protected:
    private:
};

#endif // PROTOBUFSYNCHRONOUSCLIENTFORTRAINGUI_HPP
