#include "sampler.h"
#include "span_context.h"
#include "trace_metadata.h"
#include <memory>
#include <stdlib.h>
#include <string>

namespace trace {
class RandomSampler : public Sampler {
private:
  int rate;

public:
  /// @brief constructor for RandomSampler
  /// @param rate_ the rate of sampling. 0 <= rate_ <= 100
  RandomSampler(int rate_) : rate(rate_){};
  SampleResult ShouldSampled(SpanContext &context) override;
  ~RandomSampler() = default;
  std::unique_ptr<Sampler> Clone() override {
    return std::make_unique<RandomSampler>(rate);
  };
  // TODO: Serialize
  std::string Serialize() override {
    return std::to_string(static_cast<int>(SampleStrategy::kRandomSample));
  }
};

inline SampleResult RandomSampler::ShouldSampled(SpanContext &context) {
  if (context.GetTraceFlag() == TraceFlag::kIsSampled) {
    return SampleResult(true, SampleStrategy::kRandomSample,
                        TraceFlag::kIsSampled);
  } else if (context.GetTraceFlag() == TraceFlag::kIsDiscarded) {
    return SampleResult(false, SampleStrategy::kRandomSample,
                        TraceFlag::kIsDiscarded);
  }
  srand(time(NULL));
  int res = rand() % 100;
  return SampleResult(res < rate, SampleStrategy::kRandomSample,
                      res < rate ? TraceFlag::kIsSampled
                                 : TraceFlag::kIsDiscarded);
}

} // namespace trace