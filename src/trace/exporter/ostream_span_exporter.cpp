
#include "ostream_span_exporter.h"
#include "span.h"
#include "span_exporter.h"
#include "span_metadata.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace trace {
std::string printTags(std::unordered_map<std::string, std::string> &tags);
std::string printStatus(StatusCode status);

void OstreamSpanExporter::Export(SpanRecord &span) {
  sout << "{"
       << "\n  \"trace_id\": \"" << span.trace_id << "\","
       << "\n  \"span_id\": \"" << span.id << "\","
       << "\n  \"parent_span_id\": \"" << span.parent_id << "\","
       << "\n  \"name\": \"" << span.name << "\","
       << "\n  \"service_name\": \"" << span.service_name << "\","
       << "\n  \"start_time\": " << span.start_time << ","
       << "\n  \"end_time\": " << span.end_time << ","
       << "\n  \"tags\": " << printTags(span.tags) << ","
       << "\n  \"status\": \"" << printStatus(span.status) << "\""
       << "\n}" << std::endl;
}

void OstreamSpanExporter::Export(std::vector<SpanRecord> &records) {
  for (auto record : records) {
    Export(record);
  }
}

std::string printTags(std::unordered_map<std::string, std::string> &tags) {
  std::string tags_str = "{";
  for (auto &tag : tags) {
    tags_str += "\"" + tag.first + "\": \"" + tag.second + "\",";
  }
  tags_str += "}";
  return tags_str;
}

std::string printStatus(StatusCode status) {
  switch (status) {
  case StatusCode::kUnset:
    return "Unset";
  case StatusCode::kOk:
    return "Ok";
  case StatusCode::kError:
    return "Error";
  default:
    return "Unknown";
  }
}

} // namespace trace
