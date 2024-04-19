#include "trace.h"
#include <memory>
#include <string>
#include "span_context.h"

namespace trace
{

    std::shared_ptr<Span> Trace::StartSpan(std::string name, std::string service_name)
    {

        SpanContext &span_context = Context::GetSpanContext();
        if (!this->context->GetSampler().ShouldSampled(span_context))
        {
            // TODO: 修改TraceFlag
            return std::make_shared<NoopSpan>();
        }
        std::string parent_id;
        // 如果没有span，尝试从上下文中获取父span
        // 否则为root span
        if (span_context.IsValid())
        {
            parent_id = span_context.SpanId();
        }
        return std::make_shared<Span>(name, service_name, this->trace_id, parent_id, this->context);
    }

} // namespace trace
