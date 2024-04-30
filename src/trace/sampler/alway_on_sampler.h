#include "sampler.h"
#include <memory>
#include <string>
#include <unordered_map>

#ifndef TRACE_ALWAYS_ON_SAMPLER_H
#define TRACE_ALWAYS_ON_SAMPLER_H
namespace trace {
class AlwaysOnSampler : public Sampler {

public:
  AlwaysOnSampler() = default;
  ~AlwaysOnSampler() = default;
  SampleResult ShouldSampled(SpanContext &context) override;
  std::unique_ptr<Sampler> Clone() override;
  SampleStrategy GetSampleStrategy() override;
  std::unordered_map<std::string, std::string> GetAttributes() override;
};

inline SampleResult AlwaysOnSampler::ShouldSampled(SpanContext &context) {
  return SampleResult(true, SampleStrategy::kAlwaysSample,
                      TraceFlag::kIsSampled);
}

inline std::unique_ptr<Sampler> AlwaysOnSampler::Clone() {
  return std::make_unique<AlwaysOnSampler>();
}


inline SampleStrategy AlwaysOnSampler::GetSampleStrategy() {
  return SampleStrategy::kAlwaysSample;
}

inline std::unordered_map<std::string, std::string> AlwaysOnSampler::GetAttributes() {
  return {};  
}
} // namespace trace

#endif // !TRACE_ALWAYS_ON_SAMPLER_H
