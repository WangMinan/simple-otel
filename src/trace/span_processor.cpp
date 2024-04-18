#include "span_processor.h"
#include "span.h"

namespace trace
{
    void SimpleSpanProcessor::OnStart(Span &span)
    {
    }

    void SimpleSpanProcessor::OnEnd(Span &span)
    {

        this->exporter->Export(span);
    }
}