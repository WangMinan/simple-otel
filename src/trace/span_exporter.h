
#ifndef TRACE_SPAN_EXPORTER_H
#define TRACE_SPAN_EXPORTER_H

namespace trace
{
    class Span;
    class SpanExporter
    {
    protected:
        virtual void Export_(Span &span) = 0;

    public:
        virtual ~SpanExporter() = default;
        virtual void Export(Span &span) final;
    };

};     // namespace trace
#endif // !TRACE_SPAN_EXPORTER_H
