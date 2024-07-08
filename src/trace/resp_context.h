#include "../protocol/message.h"
#include "trace_metadata.h"
#include <memory>
#include <string>
#include <utility>

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
  static std::shared_ptr<RespContext> FromMessage(protocol::Message &message) {
    std::string trace_flag_ = message.GetHeader("trace_flag");
    TraceFlag trace_flag = TraceFlagHandler::GetTraceFlag(trace_flag_);
    return std::make_shared<RespContext>(trace_flag);
  }
  TraceFlag GetTraceFlag() { return trace_flag; }
  protocol::Message ToMessage() {
    protocol::Message msg;
    msg.SetHeader("trace_flag", TraceFlagHandler::Serialize(trace_flag));
    return msg;
  };
};

} // namespace trace

#endif // !TRACE_RESP_CONTEXT_H
