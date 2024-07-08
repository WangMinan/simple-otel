#ifndef TRACE_HEAD_VARIANT_SAMPLER_H
#define TRACE_HEAD_VARIANT_SAMPLER_H

#include "post_sampler.h"
#include "span_context.h"
#include <memory>
#include <string>

namespace trace {

class HeadVariantSampler : public PostSampler {

private:
  int threshold;
  int rate;

public:
  HeadVariantSampler(int threshold_, int rate_)
      : threshold(threshold_), rate(rate_){};
  ~HeadVariantSampler() = default;
  SampleResult ShouldSampled(SpanContext &context) override;
  SampleResult
  PostSample(std::vector<std::shared_ptr<RespContext>> &resp_contexts,
             SpanContext &context) override;
  std::unique_ptr<Sampler> Clone() override; 
  std::string Serialize() override;
  SampleStrategy GetSampleStrategy() override;
  std::unordered_map<std::string, std::string> GetAttributes() override;
};
} // namespace trace

#endif // !TRACE_HEAD_VARIANT_SAMPLER_H
