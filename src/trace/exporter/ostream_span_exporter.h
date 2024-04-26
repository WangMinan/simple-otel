#ifndef TRACE_OSTREAM_EXPORTER_H
#define TRACE_OSTREAM_EXPORTER_H

#include "../log/logger.h"
#include "logger_factory.h"
#include "span_exporter.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

namespace trace {
class OstreamSpanExporter : public SpanExporter {
private:
  std::ostream &sout;
  const std::unique_ptr<logger::Logger> log;

public:
  OstreamSpanExporter(
      std::ostream &sout_ = std::cout,
      std::unique_ptr<logger::Logger> &&log_ = logger::LoggerFactory::Create())
      : sout(sout_), log(std::move(log_)){};
  ~OstreamSpanExporter() = default;
  void Export(SpanRecord &record) override;
  void Export(std::vector<SpanRecord> &records) override;
}; // namespace trace
} // namespace trace
#endif // !TRACE_OSTREAM_EXPORTER_H
