#include "trainprotobufsynchronousserver.hpp"
#include "protobufpositioninformationserviceimpl.hpp"

using namespace google::protobuf;

TrainProtobufSynchronousServer::~TrainProtobufSynchronousServer()
{
    //dtor
}

int TrainProtobufSynchronousServer::BindProtobufServices(RCF::RcfProtoServer & rcfProtoServer)
{
    startup_severity_channel_logger_mt& lg = server_logger::get();

    try
    {
        PositionInformationImpl positionInformationImpl(serverconf, sessions_);
        rcfProtoServer.bindService(positionInformationImpl);
        BOOST_LOG_SEV(lg, notification) << "Bind PositionInformationServiceImpl Successfull !!!";
        return NO_ERROR;
    }
    catch (google::protobuf::FatalException fe)
    {
        BOOST_LOG_SEV(lg, critical) << "UNSUCCESSFULL bind for PositionInformationServiceImpl!!!" << fe.what();
        return ERROR_WITH_PROTOCOL_BUFFER_SERVER;
    }
}

int TrainProtobufSynchronousServer::GetRCFProtoServerListeningPort(void)
{
    return std::stoi(serverconf->main_listener_port_);
}
