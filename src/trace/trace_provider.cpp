
#include "trace_provider.h"
#include <memory>

namespace trace
{
    TraceProvider &TraceProvider::GetInstance()
    {
        static TraceProvider provider;
        return provider;
    }

    /// @brief create a trace and return it
    /// @return trace
    std::shared_ptr<Trace> TraceProvider::GetTrace()
    {
        if (Context::GetTraceFlag() == kIsDiscarded) {
            return std::make_shared<NoopTrace>();
        }
        TraceProvider &provider = TraceProvider::GetInstance();
        auto trace = std::make_shared<Trace>();
        provider.traces[trace->Id()] = *trace;
        return trace;
    }

    /// @brief get a trace by its id. If not exist, this function will create it.
    /// @param trace_id 
    /// @return trace 
    std::shared_ptr<Trace> TraceProvider::GetTrace(std::string trace_id)
    {
        if (Context::GetTraceFlag() == kIsDiscarded) {
            return std::make_shared<NoopTrace>();
        }
        TraceProvider &provider = TraceProvider::GetInstance();
        if (provider.traces.find(trace_id) == provider.traces.end())
        {
            provider.traces[trace_id] = Trace(trace_id);
        }
        return std::make_shared<Trace>(provider.traces[trace_id]);
    }

} // namespace trace
