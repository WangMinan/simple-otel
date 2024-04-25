
#ifndef TRACE_TRACE_CONTEXT_H
#define TRACE_TRACE_CONTEXT_H

#include "sampler.h"
#include "span_processor.h"
#include <memory>
namespace trace {
class TraceContext {
private:
  std::shared_ptr<SpanProcessor> processor;
  std::unique_ptr<Sampler> sampler;

public:
  TraceContext(std::shared_ptr<SpanProcessor> processor_,
               std::unique_ptr<Sampler> &&sampler_)
      : processor(processor_), sampler(std::move(sampler_)){};
  ~TraceContext() = default;
  std::shared_ptr<SpanProcessor> Processor() { return processor; }
  Sampler &GetSampler() { return *sampler; }
};

}; // namespace trace

#endif // !TRACE_TRACE_CONTEXT_H
