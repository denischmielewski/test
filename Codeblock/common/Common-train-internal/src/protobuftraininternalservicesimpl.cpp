#include <protobuftraininternalservicesimpl.hpp>
#include <unordered_map>

using namespace std;
using namespace google::protobuf;

extern uint16_t g_commSessionMutexLockTimeoutMilliseconds;


// PositionInformation() method implementation.
void SetOperationModeImpl::SetOperationMode(  RpcController *                             controller,
                                                    const SetOperationModeCommand *         request,
                                                    SetOperationModeResponse *                response,
                                                    Closure *                                   done)
{

    startup_severity_channel_logger_mt& lg = train_internal_comm_logger_c1::get();

    BOOST_LOG_SEV(lg, notification) << "Set operation Mode received !!! ";

    RCF::RcfProtoController * rcfController = static_cast<RCF::RcfProtoController *>(controller);
    RCF::RcfProtoSession * pprotoSession = rcfController->getSession();
    RCF::RcfSession & rcfSession = rcfController->getSession()->getRcfSession();

    // Fill in the response.
    response->set_previousmode("Manual");
    response->set_newmode("Automatic");

    // Send response back to the client.
    done->Run();

    BOOST_LOG_SEV(lg, warning) << "Train Communication Session Lock failed !!!";
}

