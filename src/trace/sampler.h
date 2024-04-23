#ifndef TRACE_TRACE_SAMPLER_H
#define TRACE_TRACE_SAMPLER_H
#include "trace_metadata.h"
#include <memory>
#include <string>
namespace trace {

class SpanContext;

enum class SampleStrategy {
  kAlwaysSample = 0,
  kNeverSample = 1,
  kRandomSample = 2,
  kTailSample = 3
};

class SampleResult {
private:
  bool should_sample;
  SampleStrategy sample_strategy;
  TraceFlag new_trace_flag;

public:
  SampleResult(bool shouldSample_, SampleStrategy sample_strategy_,
               TraceFlag new_trace_flag_)
      : should_sample(shouldSample_), sample_strategy(sample_strategy_),
        new_trace_flag(new_trace_flag_){};
  ~SampleResult() = default;
  bool ShouldSample() { return should_sample; }
  SampleStrategy GetSampleStrategy() { return sample_strategy; }
  TraceFlag GetTraceFlag() { return new_trace_flag; }
};

class Sampler {
public:
  virtual SampleResult ShouldSampled(SpanContext &context) = 0;
  virtual ~Sampler() = default;
  virtual std::unique_ptr<Sampler> Clone() = 0;
  virtual std::string Serialize() = 0;
};

} // namespace trace

#endif // !TRACE_TRACE_SAMPLER_H
