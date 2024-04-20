#include "span_context.h"
#include "sampler.h"
#include <memory>
#include <string>

std::shared_ptr<trace::Sampler> getSampler(std::string sampler) {
  switch (atoi(sampler.c_str())) {
  case 0:
    return std::make_shared<trace::AlwaysOnSampler>();
  default:
    return std::make_shared<trace::AlwaysOnSampler>();
  }
}

namespace trace {

thread_local SpanContext Context::parent_context;
thread_local SpanContext Context::current_context;

bool SpanContext::IsValid() { return !trace_id.empty() && !span_id.empty(); }

void Context::Extract(protocol::Message message) {
  auto trace_id = message.GetHeader("trace_id");
  auto span_id = message.GetHeader("span_id");
  auto trace_flag = message.GetHeader("trace_flag");
  auto sampler = message.GetHeader("sampler");
  if (trace_id.empty()) {

    parent_context = SpanContext(trace_id, span_id);
  } else {
    parent_context = SpanContext(trace_id, span_id,
                                 TraceFlagHandler::GetTraceFlag(trace_flag),
                                 getSampler(sampler));
  }
}

SpanContext &Context::GetParentContext() { return parent_context; }

SpanContext &Context::GetCurrentContext() { return current_context; }

void Context::SetTraceId(std::string trace_id_) {
  current_context.trace_id = trace_id_;
}

void Context::SetSpanId(std::string span_id_) {
  current_context.span_id = span_id_;
}

void Context::SetTraceFlag(TraceFlag trace_flag_) {
  current_context.trace_flag = trace_flag_;
}

void Context::SetSampler(std::shared_ptr<Sampler> &&sampler_) {
  current_context.sampler = sampler_;
}

void Context::WriteToMessage(protocol::Message &message) {
  message.SetHeader("trace_id", current_context.trace_id);
  message.SetHeader("span_id", current_context.span_id);
  message.SetHeader("trace_flag", TraceFlagHandler::Serialize(
                                      current_context.GetTraceFlag()));
  message.SetHeader("sampler", current_context.GetSampler()->Serialize());
}
} // namespace trace