#include "post_sample_processor.h"
#include "span.h"
#include "span_context.h"
#include <memory>

namespace trace {

void PostSampleProcessor::OnStart(Span &span) {}
void PostSampleProcessor::OnEnd(Span &span) {
  auto resp_context = Context::GetCurrentRespContext();
  SpanContext &span_context = Context::GetCurrentContext();
  SampleResult result = this->sampler->PostSample(*resp_context, span_context);
  auto parent_context = Context::GetParentRespContext();
  if (parent_context != nullptr) {
    auto resp = std::make_shared<RespContext>(result.GetTraceFlag());
    parent_context->push_back(resp);
  } else {
    Context::SetReturnContext(result.GetTraceFlag());
  }
  if (result.ShouldSample()) {
    SpanRecord record = SpanToRecord(span);
    this->exporter->Export(record);
  }
}
} // namespace trace