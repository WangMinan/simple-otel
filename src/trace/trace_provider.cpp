
#include "trace_provider.h"

namespace trace
{
    TraceProvider &TraceProvider::GetInstance()
    {
        static TraceProvider provider;
        return provider;
    }

    std::shared_ptr<Trace> TraceProvider::GetTrace()
    {
        TraceProvider &provider = TraceProvider::GetInstance();
        auto trace = std::make_shared<Trace>();
        provider.traces[trace->Id()] = *trace;
        return trace;
    }

    std::shared_ptr<Trace> TraceProvider::GetTrace(std::string trace_id)
    {
        TraceProvider &provider = TraceProvider::GetInstance();
        if (provider.traces.find(trace_id) == provider.traces.end())
        {
            return nullptr;
        }
        return std::make_shared<Trace>(provider.traces[trace_id]);
    }
} // namespace trace
