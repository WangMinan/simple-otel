#include "ostream_log_exporter.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>

namespace logger {

void OstreamLogExporter::Export(LogRecord &log_record) {
  auto tags = log_record.GetTags();
  std::cout << "{"
            << "\n \"trace_id\": \"" << log_record.GetTraceId() << "\","
            << "\n \"span_id\": \"" << log_record.GetSpanId() << "\","
            << "\n \"service_name\": \"" << log_record.GetServiceName() << "\","
            << "\n \"content\": \"" << log_record.GetContent() << "\","
            << "\n \"timestamp\": " << log_record.GetTimestamp() << ","
            << "\n \"level\": \"" << printLogLevel(log_record.GetLogLevel())
            << "\","
            << "\n \"tags\": " << printTags(tags) << ","
            << "\n \"error\": " << printError(log_record.GetError()) << "\n }"
            << std::endl;
}

void OstreamLogExporter::Export(std::vector<LogRecord> &records) {
  for (auto &record : records) {
    Export(record);
  }
}

} // namespace logger
