#include "trace.h"
#include "span_context.h"
#include <memory>
#include <string>

namespace trace {

std::shared_ptr<Span> Trace::StartSpan(std::string name) {

  // 获取当前span的上下文，它将成为新span的父span
  SpanContext &parent_context = Context::GetCurrentContext();
  auto result = this->context->GetSampler().ShouldSampled(parent_context);
  if (!result.ShouldSample() &&
      result.GetTraceFlag() != TraceFlag::kIsWaiting) {
    Context::Attach(this->trace_id, "", result.GetTraceFlag(),
                    parent_context.GetSampler()->Clone());
    return std::make_shared<NoopSpan>();
  }
  std::string parent_id;
  // 如果没有span，尝试从上下文中获取父span
  // 否则为root span
  if (parent_context.IsValid()) {
    parent_id = parent_context.GetSpanId();
  }
  auto span = std::make_shared<Span>(name, this->service_name, this->trace_id,
                                     parent_id, this->context);
  Context::Attach(span->GetTraceId(), span->GetId(), result.GetTraceFlag(),
                  span->GetTraceContext()->GetSampler().Clone());
  Context::AddActiveSpan(span);
  return span;
}

} // namespace trace
