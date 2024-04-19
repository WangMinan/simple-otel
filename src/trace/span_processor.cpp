#include "span_processor.h"
#include "span.h"
#include <memory>

namespace trace
{
    void SimpleSpanProcessor::OnStart(Span &span)
    {
    }

    void SimpleSpanProcessor::OnEnd(Span &span)
    {

        this->exporter->Export(span);
    }

    std::unique_ptr<SpanProcessor> SimpleSpanProcessor::Clone()
    {
        return std::make_unique<SimpleSpanProcessor>(std::move(this->exporter->Clone()));
    }
}