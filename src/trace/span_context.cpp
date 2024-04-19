#include "span_context.h"
#include "sampler.h"
#include <memory>
#include <string>

std::shared_ptr<trace::Sampler> getSampler(std::string sampler)
{
    switch (atoi(sampler.c_str()))
    {
    case 0:
        return std::make_shared<trace::AlwaysOnSampler>();
    default:
        return std::make_shared<trace::AlwaysOnSampler>();
    }
}

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
        auto sampler = message.GetHeader("sampler");
        if (trace_id.empty())
        {

            span_context = SpanContext(trace_id, span_id);
        }
        else
        {
            span_context = SpanContext(trace_id, span_id, TraceFlagHandler::GetTraceFlag(trace_flag), getSampler(sampler));
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

    bool Context::IsValid()
    {
        return span_context.IsValid();
    }

    std::shared_ptr<Sampler> Context::GetSampler() {
        return span_context.GetSampler();
    }
}