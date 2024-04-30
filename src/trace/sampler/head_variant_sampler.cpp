#include "head_variant_sampler.h"
#include "sampler.h"
#include <string>
#include <unordered_map>

namespace trace {

SampleResult HeadVariantSampler::ShouldSampled(SpanContext &context) {
  if (context.IsValid() && (context.GetTraceFlag() == TraceFlag::kIsDiscarded ||
                            context.GetTraceFlag() == TraceFlag::kIsSampled)) {
    return SampleResult(context.GetTraceFlag() == TraceFlag::kIsSampled,
                        SampleStrategy::kHeadVariantSample,
                        context.GetTraceFlag());
  }
  if (threshold == 0) {
    srand(time(NULL));
    int res = rand() % 100;
    return SampleResult(res < rate, SampleStrategy::kRandomSample,
                        res < rate ? TraceFlag::kIsSampled
                                   : TraceFlag::kIsDiscarded);
  }
  auto &attributes = context.GetAttributes();
  attributes["threshold"] = std::to_string(threshold - 1);
  return SampleResult(true, SampleStrategy::kHeadVariantSample,
                      TraceFlag::kIsWaiting);
}
SampleResult HeadVariantSampler::PostSample(
    std::vector<std::shared_ptr<RespContext>> &resp_contexts,
    SpanContext &context) {
  if (context.GetTraceFlag() == TraceFlag::kIsDiscarded ||
      context.GetTraceFlag() == TraceFlag::kIsSampled) {
    return SampleResult(context.GetTraceFlag() == TraceFlag::kIsSampled,
                        SampleStrategy::kHeadVariantSample,
                        context.GetTraceFlag());
  }
  for (auto ctx : resp_contexts) {
    if (ctx->GetTraceFlag() == TraceFlag::kIsSampled) {
      return SampleResult(true, SampleStrategy::kHeadVariantSample,
                          TraceFlag::kIsSampled);
    }
  }
  return SampleResult(false, SampleStrategy::kHeadVariantSample,
                      TraceFlag::kIsDiscarded);
}
std::unique_ptr<Sampler> HeadVariantSampler::Clone() {
  return std::make_unique<HeadVariantSampler>(this->threshold, this->rate);
}

SampleStrategy HeadVariantSampler::GetSampleStrategy() {
  return SampleStrategy::kHeadVariantSample;
}
std::unordered_map<std::string, std::string>
HeadVariantSampler::GetAttributes() {
  std::unordered_map<std::string, std::string> ret = {
      {"threshold", std::to_string(threshold)}, {"rate", std::to_string(rate)}};
  return ret;
}
} // namespace trace
