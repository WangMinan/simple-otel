#include "span_context.h"

namespace trace
{
    bool SpanContext::IsValid()
    {
        return !trace_id.empty() && !span_id.empty();
    }
}