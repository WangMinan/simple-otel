
#include "trace_provider.h"
#include <memory>
#include <string>
#include <stdexcept>
#include "trace_metadata.h"
#include "span_context.h"

namespace trace
{
    std::mutex trace::TraceProvider::lock;
    trace::TraceProvider TraceProvider::provider;
    void TraceProvider::SetSpanProcessor(std::shared_ptr<SpanProcessor> processor)
    {
        std::lock_guard<std::mutex> lock_guard(TraceProvider::lock);
        provider = TraceProvider(processor);
    }

    TraceProvider &TraceProvider::GetInstance()
    {
        std::lock_guard<std::mutex> lock_guard(TraceProvider::lock);
        return provider;
    }

    /// @brief create a trace by context and return it
    /// @return trace
    std::shared_ptr<Trace> TraceProvider::GetTrace()
    {
        if (Context::GetTraceFlag() == kIsDiscarded)
        {
            return std::make_shared<NoopTrace>();
        }
        TraceProvider &provider = TraceProvider::GetInstance();
        std::string trace_id = Context::GetTraceId();
        if (trace_id.empty())
        {
            throw std::runtime_error("trace id is empty");
        }
        auto trace = std::make_shared<Trace>(trace_id, provider.processor);
        provider.traces[trace->Id()] = trace;
        return trace;
    }

    /// @brief get a trace by its id. If not exist, this function will throw an error.
    /// @param trace_id
    /// @return trace
    std::shared_ptr<Trace> TraceProvider::GetTrace(std::string trace_id)
    {
        if (Context::GetTraceFlag() == kIsDiscarded)
        {
            return std::make_shared<NoopTrace>();
        }
        TraceProvider &provider = TraceProvider::GetInstance();
        if (provider.traces.find(trace_id) == provider.traces.end())
        {
            throw std::runtime_error("trace not found");
        }
        return provider.traces[trace_id];
    }

    /// @brief start a new trace. This function should be invoked at the beginning.
    /// @return trace
    std::shared_ptr<Trace> TraceProvider::StartTrace()
    {
        auto provider = GetInstance();
        auto trace = std::make_shared<Trace>(provider.processor);
        provider.traces[trace->Id()] = trace;
        return trace;
    }
} // namespace trace
