#include "grpc_log_exporter.h"
#include "collector/log_service.pb.h"
#include "common/source_type.pb.h"
#include <grpcpp/client_context.h>
#include <vector>

using arktouros::proto::collector::v1::LogRequest;
using arktouros::proto::collector::v1::LogResponse;
using arktouros::proto::common::v1::SourceType;

namespace logger {

void BuildProtoLog(arktouros::proto::log::v1::Log *new_log, LogRecord &log) {
  new_log->set_trace_id(log.GetTraceId());
  new_log->set_span_id(log.GetSpanId());
  new_log->set_severity_text(printLogLevel(log.GetLogLevel()));
  new_log->set_content(log.GetContent());
  new_log->set_service_name(log.GetServiceName());
  new_log->set_timestamp(log.GetTimestamp());
  new_log->set_error(log.GetError());
  new_log->set_type(SourceType::LOG);
  for (auto &tag : log.GetTags()) {
    auto new_tag = new_log->add_tags();
    new_tag->set_key(tag.first);
    new_tag->set_value(tag.second);
  }
}

LogExportResponse GrpcLogClient::Export(std::vector<LogRecord> &records) {
  LogRequest req;
  for (auto record : records) {

    auto new_log = req.add_logs();
    BuildProtoLog(new_log, record);
  }
  grpc::ClientContext ctx;
  LogResponse resp;
  auto status = stub->Export(&ctx, req, &resp);
  if (status.ok()) {
    return LogExportResponse{true, ""};
  } else {
    return LogExportResponse{false, status.error_message()};
  }
}

void GrpcLogExporter::Export(LogRecord &log) {
  std::vector<LogRecord> records = std::vector<LogRecord>{log};
  auto resp = this->client->Export(records);
  if (!resp.success) {
    throw std::runtime_error(resp.error_message);
  }
}

void GrpcLogExporter::Export(std::vector<LogRecord> &records) {
  auto resp = this->client->Export(records);
  if (!resp.success) {
    throw std::runtime_error(resp.error_message);
  }
}

} // namespace logger
