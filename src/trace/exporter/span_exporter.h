
#ifndef TRACE_SPAN_EXPORTER_H
#define TRACE_SPAN_EXPORTER_H

namespace trace
{
    class Span;
    class SpanExporter
    {
    public:
        virtual ~SpanExporter() = default;
        virtual void Export(Span &span) = 0;
    };

};     // namespace trace
#endif // !TRACE_SPAN_EXPORTER_H
