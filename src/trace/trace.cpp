#include "trace.h"
#include <memory>
#include <string>

namespace trace
{

    std::shared_ptr<Span> Trace::StartSpan(std::string name, std::string service_name, SpanContext &span_context)
    {
        std::string parent_id;
        // 如果当前服务已经有span了，则数组内上一个span就是父span
        if (!this->spans.empty())
        {

            Span &parent_span = this->spans.back();
            std::string parent_id = parent_span.Id();
        }
        // 如果没有span，尝试从上下文中获取父span
        // 否则为root span
        else if (span_context.IsValid())
        {
            parent_id = span_context.SpanId();
        }
        Span span(name, service_name, this->trace_id, parent_id);
        this->spans.emplace_back(span);
        return std::make_shared<Span>(span);
    }
} // namespace trace
