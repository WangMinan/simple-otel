
#ifndef TRACE_SPAN_EXPORTER_H
#define TRACE_SPAN_EXPORTER_H

#include <memory>
namespace trace
{
    class Span;
    class SpanExporter
    {
    public:
        virtual ~SpanExporter() = default;
        virtual void Export(Span &span) = 0;
        virtual std::unique_ptr<SpanExporter> Clone() = 0;
    };

};     // namespace trace
#endif // !TRACE_SPAN_EXPORTER_H
