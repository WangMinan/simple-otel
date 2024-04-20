#include "sampler.h"
#include <memory>
#include <string>

namespace trace {
class AlwaysOnSampler : public Sampler {

public:
  AlwaysOnSampler() = default;
  ~AlwaysOnSampler() = default;
  SampleResult ShouldSampled(SpanContext &context) override;
  std::unique_ptr<Sampler> Clone() override;
  std::string Serialize() override;
};

inline SampleResult AlwaysOnSampler::ShouldSampled(SpanContext &context) {
  return SampleResult(true, SampleStrategy::kAlwaysSample,
                      TraceFlag::kIsSampled);
}

inline std::unique_ptr<Sampler> AlwaysOnSampler::Clone() {
  return std::make_unique<AlwaysOnSampler>();
}

inline std::string AlwaysOnSampler::Serialize() {
  return std::to_string(static_cast<int>(SampleStrategy::kAlwaysSample));
}
} // namespace trace
