#include "sampler.h"
#include <memory>

namespace trace {
class AlwaysOffSampler : public Sampler {
public:
  AlwaysOffSampler() = default;
  ~AlwaysOffSampler() = default;
  SampleResult ShouldSampled(SpanContext &context) override {
    return SampleResult(false, SampleStrategy::kNeverSample,
                        TraceFlag::kIsDiscarded);
  }
  std::unordered_map<std::string, std::string> GetAttributes() override {
    return {};
  }
  std::unique_ptr<Sampler> Clone() override {
    return std::make_unique<AlwaysOffSampler>();
  }

  SampleStrategy GetSampleStrategy() override {
    return SampleStrategy::kNeverSample;
  }
};
} // namespace trace