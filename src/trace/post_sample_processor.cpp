#include "post_sample_processor.h"
#include "span.h"
#include "span_context.h"
#include <memory>
#include <stdexcept>

namespace trace {
using std::make_unique;

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
    this->exporter->Export(span);
  }
}
std::unique_ptr<SpanProcessor> PostSampleProcessor::Clone() {
  auto sampler = this->sampler->Clone();
  if (auto post_sampler = dynamic_cast<PostSampler *>(sampler.get())) {
    sampler.release();
    std::unique_ptr<PostSampler> s(post_sampler);
    return make_unique<PostSampleProcessor>(std::move(this->exporter->Clone()),
                                            std::move(s));
  }
  throw std::runtime_error("clone failed");
}
} // namespace trace