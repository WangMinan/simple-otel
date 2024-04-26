
#ifndef TRACE_TRACE_PROVIDER_H
#define TRACE_TRACE_PROVIDER_H
#include "span_context.h"
#include "processor/span_processor.h"
#include "trace.h"
#include "trace_context.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
namespace trace {
class AlwaysOnSampler;
class TraceProvider {
private:
  std::unordered_map<std::string, std::shared_ptr<Trace>> traces;
  std::shared_ptr<TraceContext> context;
  static TraceProvider provider;
  static std::recursive_mutex lock;
  TraceProvider(std::shared_ptr<SpanProcessor> processor_,
                std::unique_ptr<Sampler> &&sampler_)
      : context(std::make_shared<TraceContext>(processor_,
                                               std::move(sampler_))){};
  TraceProvider() = default;

public:
  ~TraceProvider() = default;
  static TraceProvider &GetInstance();
  static std::shared_ptr<Trace> GetTrace();
  static void InitProvider(std::unique_ptr<SpanProcessor> &&processor_,
                           std::unique_ptr<Sampler> &&sampler_ =
                               std::make_unique<AlwaysOnSampler>());
};

};     // namespace trace
#endif // !TRACE_TRACE_PROVIDER_H
