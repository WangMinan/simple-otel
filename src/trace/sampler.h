#ifndef TRACE_TRACE_SAMPLER_H
#define TRACE_TRACE_SAMPLER_H
#include "trace_metadata.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace trace {

class SpanContext;

enum class SampleStrategy {
  kAlwaysSample = 0,
  kRandomSample = 1,
  kTailSample = 2,
  kHeadVariantSample = 3,
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
  virtual bool IsPostSampler() { return false; }
  virtual SampleStrategy GetSampleStrategy() = 0;
  virtual std::unordered_map<std::string, std::string> GetAttributes() = 0;
};

} // namespace trace

#endif // !TRACE_TRACE_SAMPLER_H
