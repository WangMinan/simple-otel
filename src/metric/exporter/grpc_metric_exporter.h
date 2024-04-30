#ifndef METRIC_GRPC_EXPORTER_H
#define METRIC_GRPC_EXPORTER_H

#include "collector/metric_service.grpc.pb.h"
#include "exporter/metric_exporter.h"
#include "metric_record.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <vector>

using arktouros::proto::collector::v1::MetricService;
using grpc::Channel;

namespace metric {

struct MetricExportResponse {
  bool success;
  std::string error_message;
};

class GrpcMetricClient {
private:
  std::unique_ptr<MetricService::Stub> stub;

public:
  GrpcMetricClient(std::shared_ptr<Channel> channel)
      : stub(MetricService::NewStub(
            std::static_pointer_cast<grpc::ChannelInterface>(channel))){};
  ~GrpcMetricClient() = default;
  MetricExportResponse Export(std::vector<MetricRecord *> &records);
};

class GrpcMetricExporter : public MetricExporter {
private:
  std::unique_ptr<GrpcMetricClient> client;

public:
  GrpcMetricExporter(std::shared_ptr<Channel> channel)
      : client(std::make_unique<GrpcMetricClient>(channel)){};
  ~GrpcMetricExporter() = default;
  void Export(std::vector<MetricRecord *> &records) override;
};
} // namespace metric
  // namespace metric

#endif // !METRIC_GRPC_EXPORTER_H