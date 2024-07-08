#include "post_sample_processor.h"
#include "span.h"
#include "span_context.h"
#include <memory>

namespace trace {


// 根据下游回传的信息，决定是否进行采样
void PostSampleProcessor::OnStart(Span &span) {}
void PostSampleProcessor::OnEnd(Span &span) {
  //修改Context
  auto resp_context = ServerContext::GetCurrentRespContext();
  SpanContext &span_context = ServerContext::GetCurrentContext();
  PostSampler &sampler =
      dynamic_cast<PostSampler &>(span.GetTraceContext()->GetSampler());
  SampleResult result = sampler.PostSample(*resp_context, span_context);
  auto parent_context = ServerContext::GetParentRespContext();
  if (parent_context != nullptr) {
    auto resp = std::make_shared<RespContext>(result.GetTraceFlag());
    parent_context->push_back(resp);
  } else {
    ServerContext::SetReturnContext(result.GetTraceFlag());
  }
  if (result.ShouldSample()) {
    SpanRecord record = SpanToRecord(span);
    this->exporter->Export(record);
  }
}
} // namespace trace