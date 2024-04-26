#include "sampler.h"
#include <memory>
#include <resp_context.h>
#include <vector>

#ifndef TRACE_POST_SAMPLER_H
#define TRACE_POST_SAMPLER_H
namespace trace {
class PostSampler : public Sampler {
public:
  virtual ~PostSampler() = default;
  virtual SampleResult
  PostSample(std::vector<std::shared_ptr<RespContext>> &resp_contexts,
             SpanContext &context) = 0;
  bool IsPostSampler() override { return true; }
};

} // namespace trace

#endif // !TRACE_POST_SAMPLER_H
