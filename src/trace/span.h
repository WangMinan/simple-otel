#include "processor/span_processor.h"
#include "span_metadata.h"
#include "trace_context.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <sstream>
#ifndef TRACE_SPAN_H
#define TRACE_SPAN_H
namespace trace {
class Span {
private:
  std::string id;
  std::string name;
  std::string service_name;
  std::string trace_id;
  // 父span id
  std::string parent_id;
  // span发送方的地址
  // Endpoint local_endpoint;
  // span下家的地址
  // Endpoint remote_endpoint;
  long start_time;
  long end_time;
  std::unordered_map<std::string, std::string> tags;
  StatusCode status;
  bool has_ended_;
  std::shared_ptr<TraceContext> trace_context;
  TraceFlag trace_flag;

protected:
  Span() = default;

public:
  Span(std::string name, std::string service_name, std::string trace_id,
       std::string parent_id, std::shared_ptr<TraceContext> trace_context_);
  ~Span();
  void SetTag(std::string key, std::string value);
  void End();
  void SetStatus(StatusCode status);
  std::string GetId() { return this->id; };
  std::string GetTraceId() { return this->trace_id; };
  std::string GetParentId() { return this->parent_id; };
  std::string GetName() { return this->name; };
  std::string GetServiceName() { return this->service_name; };
  long GetStartTime() { return this->start_time; };
  long GetEndTime() { return this->end_time; };
  std::unordered_map<std::string, std::string> GetTags() { return this->tags; };
  StatusCode GetStatus() { return this->status; };
  std::shared_ptr<TraceContext> GetTraceContext() {
    return this->trace_context;
  };
};

class NoopSpan : public Span {
public:
  NoopSpan(){};
};

struct SpanRecord {
  std::string id;
  std::string name;
  std::string service_name;
  std::string trace_id;
  std::string parent_id;
  long start_time;
  long end_time;
  std::unordered_map<std::string, std::string> tags;
  StatusCode status;
};

inline std::string StatusCodeToString(StatusCode status) {
    switch (status) {
        case StatusCode::kOk: return "OK";
        case StatusCode::kError: return "ERROR";
        case StatusCode::kUnset: return "UNSET";
        default: return "UNKNOWN";
    }
}

inline std::string EscapeJsonString(const std::string &input) {
    std::ostringstream ss;
    for (char c : input) {
        switch (c) {
            case '"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << c; break;
        }
    }
    return ss.str();
}

inline std::string Serialize(SpanRecord &spanRecord) {
  std::ostringstream oss;
    oss << "{";
    oss << "\"id\":\"" << EscapeJsonString(spanRecord.id) << "\",";
    oss << "\"name\":\"" << EscapeJsonString(spanRecord.name) << "\",";
    oss << "\"service_name\":\"" << EscapeJsonString(spanRecord.service_name) << "\",";
    oss << "\"trace_id\":\"" << EscapeJsonString(spanRecord.trace_id) << "\",";
    oss << "\"parent_id\":\"" << EscapeJsonString(spanRecord.parent_id) << "\",";
    oss << "\"start_time\":" << spanRecord.start_time << ",";
    oss << "\"end_time\":" << spanRecord.end_time << ",";
    oss << "\"tags\":{";

    bool first = true;
    for (const auto &tag : spanRecord.tags) {
        if (!first) {
            oss << ",";
        }
        oss << "\"" << EscapeJsonString(tag.first) << "\":\"" << EscapeJsonString(tag.second) << "\"";
        first = false;
    }

    oss << "},";
    oss << "\"status\":\"" << StatusCodeToString(spanRecord.status) << "\"";
    oss << "}";
    
    return oss.str();
}

inline std::string Serialize(Span &span) {
  std::ostringstream oss;
    oss << "{";
    oss << "\"id\":\"" << EscapeJsonString(span.GetId()) << "\",";
    oss << "\"name\":\"" << EscapeJsonString(span.GetName()) << "\",";
    oss << "\"service_name\":\"" << EscapeJsonString(span.GetServiceName()) << "\",";
    oss << "\"trace_id\":\"" << EscapeJsonString(span.GetTraceId()) << "\",";
    oss << "\"parent_id\":\"" << EscapeJsonString(span.GetParentId()) << "\",";
    oss << "\"start_time\":" << span.GetStartTime() << ",";
    oss << "\"end_time\":" << span.GetEndTime() << ",";
    oss << "\"tags\":{";

    bool first = true;
    for (const auto &tag : span.GetTags()) {
        if (!first) {
            oss << ",";
        }
        oss << "\"" << EscapeJsonString(tag.first) << "\":\"" << EscapeJsonString(tag.second) << "\"";
        first = false;
    }

    oss << "},";
    oss << "\"status\":\"" << StatusCodeToString(span.GetStatus()) << "\"";
    oss << "}";
    
    return oss.str();
}


inline SpanRecord SpanToRecord(Span &span) {
  SpanRecord record = {
    span.GetId(),
    span.GetName(),
    span.GetServiceName(),
    span.GetTraceId(),
    span.GetParentId(),
    span.GetStartTime(),
    span.GetEndTime(),
    span.GetTags(),
    span.GetStatus()
  };
  return record;
}

}; // namespace trace

#endif