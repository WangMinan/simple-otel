#include "trace.h"
#include "span_context.h"
#include <memory>
#include <string>

namespace trace {

std::shared_ptr<Span> Trace::StartSpan(std::string name,
                                       std::string service_name) {

  SpanContext &span_context = Context::GetParentContext();
  auto result = this->context->GetSampler().ShouldSampled(span_context);
  if (!result.ShouldSample()) {
    Context::SetTraceFlag(result.GetTraceFlag());
    return std::make_shared<NoopSpan>();
  } else {
    Context::SetTraceFlag(TraceFlag::kIsSampled);
  }
  std::string parent_id;
  // 如果没有span，尝试从上下文中获取父span
  // 否则为root span
  if (span_context.IsValid()) {
    parent_id = span_context.GetSpanId();
  }
  auto span = std::make_shared<Span>(name, service_name, this->trace_id,
                                     parent_id, this->context);
  Context::SetSpanId(span->GetId());
  return span;
}

} // namespace trace
