#ifndef TRACE_OSTREAM_EXPORTER_H
#define TRACE_OSTREAM_EXPORTER_H

#include "span_exporter.h"
#include <ostream>
#include <iostream>

namespace trace
{
    class OstreamSpanExporter : public SpanExporter
    {
    private:
        std::ostream &sout;

    public:
        OstreamSpanExporter(std::ostream &sout_ = std::cout) : sout(sout_){};
        ~OstreamSpanExporter() = default;
        void Export_(Span &span);
    };
} // namespace trace

#endif // !TRACE_OSTREAM_EXPORTER_H
