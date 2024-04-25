#include "grpc_span_exporter.h"
#include "collector/span_service.pb.h"
#include <fmt/format.h>
#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>
#include <iostream>
#include <memory>

using arktouros::proto::collector::v1::SpanRequest;

using arktouros::proto::collector::v1::SpanResponse;
namespace trace {

SpanExportResponse GrpcSpanClient::Export(Span &span) {
  std::cout << "start export" << std::endl;
  SpanRequest req;
  auto new_span = req.add_spans();
  new_span->set_id(span.GetId());
  new_span->set_trace_id(span.GetTraceId());
  new_span->set_name(span.GetName());
  new_span->set_parent_span_id(span.GetParentId());
  new_span->set_end_time(span.GetEndTime());
  new_span->set_start_time(span.GetStartTime());
  new_span->set_service_name(span.GetServiceName());
  new_span->set_root(span.GetParentId().empty());
  for (auto &tag : span.GetTags()) {
    auto new_tag = new_span->add_tags();
    new_tag->set_key(tag.first);
    new_tag->set_value(tag.second);
  }
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

void GrpcSpanExporter::Export(Span &span) {
  SpanExportResponse resp = client->Export(span);
  if (!resp.success) {
    throw std::runtime_error(resp.error_message);
  }
}

} // namespace trace
