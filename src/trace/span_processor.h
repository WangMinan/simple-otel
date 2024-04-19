
#include "span_exporter.h"
#include <memory>
#ifndef TARCE_SPAN_PROCESSOR_H
#define TARCE_SPAN_PROCESSOR_H

namespace trace
{
    class SpanProcessor
    {
    protected:
        std::unique_ptr<SpanExporter> exporter;

    public:
        SpanProcessor(std::unique_ptr<SpanExporter> &&exporter_) : exporter(std::move(exporter_)){};
        virtual ~SpanProcessor() = default;
        virtual void OnStart(Span &span) = 0;
        virtual void OnEnd(Span &span) = 0;
        virtual std::unique_ptr<SpanProcessor> Clone() = 0;
    };

    class SimpleSpanProcessor : public SpanProcessor
    {
    public:
        SimpleSpanProcessor(std::unique_ptr<SpanExporter> &&exporter_) : SpanProcessor(std::move(exporter_)){};
        ~SimpleSpanProcessor() = default;
        void OnStart(Span &span) override;
        void OnEnd(Span &span) override;
        std::unique_ptr<SpanProcessor> Clone() override;
    };
}; // namespace trace

#endif // !TARCE_SPAN_PROCESSOR_H
