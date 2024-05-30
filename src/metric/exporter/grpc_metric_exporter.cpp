#include "grpc_metric_exporter.h"
#include "collector/metric_service.pb.h"
#include <grpcpp/client_context.h>
#include <vector>

using arktouros::proto::collector::v1::MetricRequest;
using arktouros::proto::collector::v1::MetricResponse;

namespace metric {
void GrpcMetricExporter::Export(std::vector<MetricRecord *> &records) {
  auto resp = client->Export(records);
  if (!resp.success) {
    throw std::runtime_error(resp.error_message);
  }
}

MetricExportResponse
GrpcMetricClient::Export(std::vector<MetricRecord *> &records) {
  MetricRequest req;
  for (auto record : records) {
    auto metric = req.add_metrics();
    record->GetProtoMetric(metric);
    std::cout << metric->gauge().metric().metric_type() << std::endl;
  }
  grpc::ClientContext ctx;
  MetricResponse resp;
  grpc::Status status = stub->Export(&ctx, req, &resp);
  if (!status.ok()) {
    return MetricExportResponse{false, status.error_message()};
  }
  return MetricExportResponse{true, ""};
}
} // namespace metric