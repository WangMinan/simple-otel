#include "trace.h"
#include "span_context.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace trace {

std::shared_ptr<Span> Trace::StartSpan(std::string name) {
  // 获取当前context集合中最新的context，它将成为新span的父span
  // SpanContext &parent_server_context = ServerContext::GetParentContext();
  //修改Context
  SpanContext &parent_context = ServerContext::GetParentContext();
  auto result = this->context->GetSampler().ShouldSampled(parent_context);
  
  // 根据头部连贯采样策略，如果当前span不应该采集，则直接返回空
  if (!result.ShouldSample() &&
      result.GetTraceFlag() != TraceFlag::kIsWaiting) {
        //修改Context
    ServerContext::Attach(this->trace_id, "", std::unordered_map<std::string, std::string>{}, result.GetTraceFlag(),
                    this->context->GetSampler().GetSampleStrategy());
    return std::make_shared<NoopSpan>();
  }
  std::string parent_id;
  std::unordered_map<std::string, std::string> attributes;
  
  // 尝试从上下文中获取父span，如果没有则当前span默认为root span
  if (parent_context.IsValid()) {
    parent_id = parent_context.GetSpanId();
    attributes = parent_context.GetAttributes();
  } else {
    attributes = this->context->GetSampler().GetAttributes();
  }
  // 新建
  auto span = std::make_shared<Span>(name, this->service_name, this->trace_id,
                                     parent_id, this->context);
  
  // 将当前span的context捆绑到线程相关的Context集合中
  //修改Context
  ServerContext::Attach(span->GetTraceId(), span->GetId(), attributes,
                  result.GetTraceFlag(),
                  this->context->GetSampler().GetSampleStrategy());
  
  // 将当前span捆绑到线程相关的Span集合中
  //修改Context
  ServerContext::AddActiveSpan(span);
  return span;
}

} // namespace trace
