#include "collector/span_service.grpc.pb.h"
#include "span.h"
#include <grpcpp/grpcpp.h>
#include <memory>

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
  SpanExportResponse Export(Span &span);
};

class GrpcSpanExporter : public SpanExporter {
private:
  std::unique_ptr<GrpcSpanClient> client;

public:
  GrpcSpanExporter(std::shared_ptr<Channel> channel)
      : client(std::make_unique<GrpcSpanClient>(channel)){};

  ~GrpcSpanExporter() = default;

  void Export(Span &span) override;
};
} // namespace trace