#include "span.h"
#include "span_exporter.h"

namespace trace
{

    void SpanExporter::Export(Span &span)
    {
        if (span.IsNoop())
        {
            return;
        }
        Export_(span);
    }

}