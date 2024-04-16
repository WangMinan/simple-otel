
#ifndef TRACE_SPAN_EXPORTER_H
#define TRACE_SPAN_EXPORTER_H
#include "span.h"
#include <memory>
#include <iostream>

namespace trace
{
    class OstreamExporter
    {
    private:
        std::shared_ptr<Span> span;
        std::ostream &sout;

    public:
        OstreamExporter(std::shared_ptr<Span> span_, std::ostream &sout_ = std::cout) : span(span_), sout(sout_){};
        ~OstreamExporter() = default;
        void Export();
    };

} // namespace trace
#endif // !TRACE_SPAN_EXPORTER_H
