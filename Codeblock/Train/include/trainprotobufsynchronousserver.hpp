#ifndef TRAINPROTOBUFSYNCHRONOUSSERVER_HPP
#define TRAINPROTOBUFSYNCHRONOUSSERVER_HPP

#include <protobufsynchronousserver.hpp>


class TrainProtobufSynchronousServer : public ProtobufSynchronousServer
{
    public:
        TrainProtobufSynchronousServer(config const * config, std::unordered_map<std::string, TrainSession> * trainsSessions)
            : ProtobufSynchronousServer(config, trainsSessions){}
        ~TrainProtobufSynchronousServer();
        int BindProtobufServices(RCF::RcfProtoServer & rcfProtoServer);
        int GetRCFProtoServerListeningPort(void);
    protected:
    private:
};

#endif // TRAINPROTOBUFSYNCHRONOUSSERVER_HPP
