#ifndef TRACE_SPAN_CONTEXT_H
#define TRACE_SPAN_CONTEXT_H
#include "../protocol/message.h"
#include "sampler.h"
#include "sampler/alway_on_sampler.h"
#include "trace_metadata.h"
#include <memory>
#include <string>
namespace trace
{
    class SpanContext
    {
    private:
        std::string trace_id;
        std::string span_id;
        TraceFlag trace_flag;
        std::shared_ptr<Sampler> sampler;

    public:
        SpanContext() = default;
        SpanContext(
            std::string trace_id_, std::string span_id_,
            TraceFlag trace_flag_ = kIsSampled,
            std::shared_ptr<Sampler> &&sampler_ = std::make_shared<AlwaysOnSampler>())
            : trace_id(trace_id_), span_id(span_id_), trace_flag(trace_flag_),
              sampler(sampler_){};
        ~SpanContext() = default;
        SpanContext(SpanContext &&other) = default;
        SpanContext &operator=(SpanContext &&other) = default;
        bool IsValid();
        std::string TraceId() { return this->trace_id; };
        std::string SpanId() { return this->span_id; }
        TraceFlag GetTraceFlag() { return this->trace_flag; }
        std::shared_ptr<Sampler> GetSampler() { return this->sampler; }
    };

    /// @brief thread local span context
    class Context
    {
    private:
        thread_local static SpanContext span_context;

    public:
        /// @brief create a span context from message
        /// @param message
        void static Extract(protocol::Message message);

        static std::string GetTraceId();
        static std::string GetSpanId();
        static TraceFlag GetTraceFlag();
        static SpanContext &GetSpanContext();
        static bool IsValid();
        static std::shared_ptr<Sampler> GetSampler();
    };

} // namespace trace

#endif // !TRACE_SPAN_CONTEXT_H
