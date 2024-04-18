#include "span_context.h"

namespace trace
{
    thread_local SpanContext Context::span_context;
    bool SpanContext::IsValid()
    {
        return !trace_id.empty() && !span_id.empty();
    }

    void Context::Extract(protocol::Message message)
    {
        auto trace_id = message.GetHeader("trace_id");
        auto span_id = message.GetHeader("span_id");
        auto trace_flag = message.GetHeader("trace_flag");
        if (trace_flag.empty())
        {

            span_context = SpanContext(trace_id, span_id);
        }
        else
        {
            span_context = SpanContext(trace_id, span_id, TraceFlagHandler::GetTraceFlag(trace_flag));
        }
    }

    std::string Context::GetTraceId()
    {
        return span_context.TraceId();
    }

    std::string Context::GetSpanId()
    {
        return span_context.SpanId();
    }

    SpanContext &Context::GetSpanContext()
    {
        return span_context;
    }

    TraceFlag Context::GetTraceFlag()
    {
        return span_context.GetTraceFlag();
    }
}