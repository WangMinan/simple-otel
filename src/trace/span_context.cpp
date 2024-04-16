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

        span_context = SpanContext(message.GetHeader("trace-id"), message.GetHeader("span-id"));
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
}