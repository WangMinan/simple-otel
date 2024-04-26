#ifndef TRACE_OSTREAM_EXPORTER_H
#define TRACE_OSTREAM_EXPORTER_H

#include "span_exporter.h"
#include <iostream>
#include <ostream>
#include <vector>

namespace trace {
class OstreamSpanExporter : public SpanExporter {
private:
  std::ostream &sout;

public:
  OstreamSpanExporter(std::ostream &sout_ = std::cout) : sout(sout_){};
  ~OstreamSpanExporter() = default;
  void Export(SpanRecord &record) override;
  void Export(std::vector<SpanRecord> &records) override;
}; // namespace trace
} // namespace trace
#endif // !TRACE_OSTREAM_EXPORTER_H
