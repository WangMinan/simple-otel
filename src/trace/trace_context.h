
#ifndef TRACE_TRACE_CONTEXT_H
#define TRACE_TRACE_CONTEXT_H

#include <memory>
#include "sampler.h"
#include "span_processor.h"
namespace trace
{
    class TraceContext
    {
    private:
        std::unique_ptr<SpanProcessor> processor;
        std::unique_ptr<Sampler> sampler;

    public:
        TraceContext(std::unique_ptr<SpanProcessor> &&processor_, std::unique_ptr<Sampler> &&sampler_) : processor(std::move(processor_)), sampler(std::move(sampler_)){};
        ~TraceContext() = default;
        SpanProcessor &Processor() { return *processor; }
        Sampler &GetSampler() { return *sampler; }
    };

}; // namespace trace

#endif // !TRACE_TRACE_CONTEXT_H
