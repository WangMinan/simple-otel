#include "../protocol/message.h"
#include "trace_metadata.h"
#include <string>

#ifndef TRACE_RESP_CONTEXT_H
#define TRACE_RESP_CONTEXT_H
namespace trace {
/// @brief return context to the invoker for post processing
class RespContext {
private:
  TraceFlag trace_flag;

public:
  RespContext(TraceFlag trace_flag_) : trace_flag(trace_flag_){};
  ~RespContext() = default;
  RespContext FromMessage(protocol::Message &message) {
    std::string trace_flag_ = message.GetHeader("trace_flag");
    TraceFlag trace_flag = TraceFlagHandler::GetTraceFlag(trace_flag_);
    return RespContext(trace_flag);
  }
  TraceFlag GetTraceFlag() { return trace_flag; }
};

} // namespace trace

#endif // !TRACE_RESP_CONTEXT_H
