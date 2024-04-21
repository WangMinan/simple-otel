#include "logger.h"
#include "span.h"
#include "span_context.h"
#include "timestamp_generator.h"
#include <string>

namespace log {
void Logger::Debug(std::string content) {
  trace::SpanContext &current_context = trace::Context::GetCurrentContext();
  auto current_span = trace::Context::GetCurrentSpan();
  std::string trace_id, span_id, service_name;
  if (current_context.IsValid()) {
    trace_id = current_context.GetTraceId();
    span_id = current_context.GetSpanId();
    service_name = current_span->GetServiceName();
  }
  Debug(trace_id, span_id, content, service_name, {});
}

void Logger::Debug(
    std::string trace_id, std::string span_id, std::string content,
    std::string service_name,
    std::initializer_list<std::pair<std::string, std::string>> tags) {
  Log log = Log(service_name, trace_id, span_id, content, false,
                utils::TimestampGenerator::Now(), log::LogLevel::DEBUG, tags);
  exporter->Export(log);
}
} // namespace log
