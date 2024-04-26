
#include "trace_provider.h"
#include "processor/post_sample_processor.h"
#include <memory>

namespace trace {
std::recursive_mutex trace::TraceProvider::lock;
trace::TraceProvider TraceProvider::provider;
void TraceProvider::InitProvider(std::unique_ptr<SpanProcessor> &&processor_,
                                 std::unique_ptr<Sampler> &&sampler_) {
  std::lock_guard<std::recursive_mutex> lock_guard(TraceProvider::lock);
  provider = TraceProvider(std::move(processor_), std::move(sampler_));
}

TraceProvider &TraceProvider::GetInstance() {
  std::lock_guard<std::recursive_mutex> lock_guard(TraceProvider::lock);
  return provider;
}

/// @brief create a trace by context and return it. If the context is invalid,
/// create a trace by provider's context
/// @return trace
std::shared_ptr<Trace> TraceProvider::GetTrace() {
  std::lock_guard<std::recursive_mutex> lock_guard(TraceProvider::lock);
  TraceProvider &provider = TraceProvider::GetInstance();
  SpanContext &parent_context = Context::GetParentContext();
  // if parent context is not valid, it means this is root span
  // use provider's context
  if (!parent_context.IsValid()) {
    auto trace = std::make_shared<Trace>(provider.context);
    provider.traces.emplace(trace->GetId(), trace);
    return trace;
  } else if (provider.traces.find(parent_context.GetTraceId()) !=
             provider.traces.end()) {
    return provider.traces[parent_context.GetTraceId()];
  } else {
    std::shared_ptr<TraceContext> context;
    if (parent_context.GetSampler()->IsPostSampler()) {
      context = std::make_shared<TraceContext>(
          std::make_shared<PostSampleProcessor>(parent_context.GetSampler()->Clone()), parent_context.GetSampler()->Clone());
    } else {
      context = std::make_shared<TraceContext>(provider.context->Processor(), parent_context.GetSampler()->Clone());
    }
    auto trace = std::make_shared<Trace>(parent_context.GetTraceId(), context);
    return trace;
  }
}

} // namespace trace
