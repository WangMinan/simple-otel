
#include "trace_provider.h"
#include <memory>

namespace trace
{
    std::mutex trace::TraceProvider::lock;
    trace::TraceProvider TraceProvider::provider;
    void TraceProvider::InitProvider(std::unique_ptr<SpanProcessor> &&processor_,
                                     std::unique_ptr<Sampler> &&sampler_)
    {
        std::lock_guard<std::mutex> lock_guard(TraceProvider::lock);
        provider = TraceProvider(std::move(processor_), std::move(sampler_));
    }

    TraceProvider &TraceProvider::GetInstance()
    {
        std::lock_guard<std::mutex> lock_guard(TraceProvider::lock);
        return provider;
    }

    /// @brief create a trace by context and return it. If the context is invalid,
    /// create a trace by provider's context
    /// @return trace
    std::shared_ptr<Trace> TraceProvider::GetTrace()
    {
        TraceProvider &provider = TraceProvider::GetInstance();
        if (!Context::IsValid())
        {
            auto trace =
                std::make_shared<Trace>(provider.context);
            return trace;
        }
        else
        {
            auto context = std::make_shared<TraceContext>(
                provider.context->Processor().Clone(), Context::GetSampler()->Clone());
            auto trace = std::make_shared<Trace>(Context::GetTraceId(), context);
            return trace;
        }
    }

} // namespace trace
