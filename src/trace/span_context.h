#ifndef TRACE_SPAN_CONTEXT_H
#define TRACE_SPAN_CONTEXT_H
#include <string>
#include "../protocol/message.h"
namespace trace
{
    class SpanContext
    {
    private:
        std::string trace_id;
        std::string span_id;

    public:
        SpanContext() = default;
        SpanContext(std::string trace_id_, std::string span_id_) : trace_id(trace_id_), span_id(span_id_){};
        ~SpanContext() = default;
        bool IsValid();
        std::string TraceId() { return this->trace_id; };
        std::string SpanId()
        {
            return this->span_id;
        }
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

        std::string static GetTraceId();
        std::string static GetSpanId();
        static SpanContext &GetSpanContext();
    };

} // namespace trace

#endif // !TRACE_SPAN_CONTEXT_H
