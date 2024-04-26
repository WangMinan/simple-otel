
#ifndef TRACE_GRPC_SPAN_EXPORTER_H
#define TRACE_GRPC_SPAN_EXPORTER_H
#include "collector/span_service.grpc.pb.h"
#include "span.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <vector>

using arktouros::proto::collector::v1::SpanService;
using grpc::Channel;

namespace trace {

struct SpanExportResponse {
  bool success;
  std::string error_message;
};

class GrpcSpanClient {
private:
  std::unique_ptr<SpanService::Stub> stub;

public:
  GrpcSpanClient(std::shared_ptr<Channel> channel)
      : stub(SpanService::NewStub(
            std::static_pointer_cast<grpc::ChannelInterface>(channel))){};
  ~GrpcSpanClient() = default;
  SpanExportResponse Export(SpanRecord &span);
  SpanExportResponse Export(std::vector<SpanRecord> &records);
};

class GrpcSpanExporter : public SpanExporter {
private:
  std::unique_ptr<GrpcSpanClient> client;

public:
  GrpcSpanExporter(std::shared_ptr<Channel> channel)
      : client(std::make_unique<GrpcSpanClient>(channel)){};

  ~GrpcSpanExporter() = default;

  void Export(SpanRecord &span) override;

  void Export(std::vector<SpanRecord> &records) override;
};
} // namespace trace

#endif // !TRACE_GRPC_SPAN_EXPORTER_H
