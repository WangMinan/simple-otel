#include "ostream_log_exporter.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <memory>
#include <string>

namespace nlog {
template <typename... Args>
std::string string_format(const std::string &format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) +
               1; // Extra space for '\0'
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1); // We don't want the '\0' inside
}

std::string printLogLevel(LogLevel level) {
  switch (level) {
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARN:
    return "WARN";
  case LogLevel::ERROR:
    return "ERROR";
  }
}

void OstreamLogExporter::Export(LogRecord &log_record) {
  std::string msg = fmt::format(
      "[{}] service: {} trace_id: {} span_id: {} content: {}",
      printLogLevel(log_record.GetLogLevel()), log_record.GetServiceName(),
      log_record.GetTraceId(), log_record.GetSpanId(), log_record.GetContent());
  std::cout << msg << std::endl;
}

} // namespace nlog
