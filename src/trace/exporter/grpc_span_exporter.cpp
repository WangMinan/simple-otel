#include "grpc_span_exporter.h"
#include "collector/span_service.pb.h"
#include <fmt/format.h>
#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>
#include <iostream>
#include <memory>
#include <vector>

using arktouros::proto::collector::v1::SpanRequest;

using arktouros::proto::collector::v1::SpanResponse;
namespace trace {

void BuildProtoSpan(arktouros::proto::span::v1::Span *new_span,
                    SpanRecord &span) {
  new_span->set_id(span.id);
  new_span->set_trace_id(span.trace_id);
  new_span->set_name(span.name);
  new_span->set_parent_span_id(span.parent_id);
  new_span->set_end_time(span.end_time);
  new_span->set_start_time(span.start_time);
  new_span->set_service_name(span.service_name);
  new_span->set_root(span.parent_id.empty());
  for (auto &tag : span.tags) {
    auto new_tag = new_span->add_tags();
    new_tag->set_key(tag.first);
    new_tag->set_value(tag.second);
  }
}

SpanExportResponse GrpcSpanClient::Export(SpanRecord &span) {
  std::cout << "start export" << std::endl;
  SpanRequest req;
  auto new_span = req.add_spans();
  BuildProtoSpan(new_span, span);
  grpc::ClientContext ctx;
  SpanResponse resp;
  grpc::Status status = stub->Export(&ctx, req, &resp);
  if (status.ok()) {
    std::cout << "success" << std::endl;
    return SpanExportResponse{true, ""};
  } else {
    std::cout << status.error_message() << " " << resp.error_message()
              << std::endl;
    return SpanExportResponse{false, fmt::format(" {}, message: {}",
                                                 status.error_message(),
                                                 resp.error_message())};
  }
}

SpanExportResponse GrpcSpanClient::Export(std::vector<SpanRecord> &records) {
  SpanRequest req;
  for (auto record : records) {
    auto span = req.add_spans();
    BuildProtoSpan(span, record);
  }
  grpc::ClientContext ctx;
  SpanResponse resp;
  grpc::Status status = stub->Export(&ctx, req, &resp);
  if (status.ok()) {
    return SpanExportResponse{true, ""};
  } else {
    return SpanExportResponse{false, fmt::format(" {}, message: {}",
                                                 status.error_message(),
                                                 resp.error_message())};
  }
}

void GrpcSpanExporter::Export(SpanRecord &span) {
  SpanExportResponse resp = client->Export(span);
  if (!resp.success) {
    throw std::runtime_error(resp.error_message);
  }
}

void GrpcSpanExporter::Export(std::vector<SpanRecord> &records) {
  SpanExportResponse resp = client->Export(records);
  if (!resp.success) {
    throw std::runtime_error(resp.error_message);
  }
}
} // namespace trace