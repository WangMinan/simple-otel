#ifndef LOG_GRPC_LOG_EXPORTER_H
#define LOG_GRPC_LOG_EXPORTER_H

#include "collector/log_service.grpc.pb.h"
#include "log_exporter.h"
#include "log_record.h"
#include <grpcpp/channel.h>
#include <memory>

using arktouros::proto::collector::v1::LogService;
using grpc::Channel;

namespace logger {

struct LogExportResponse {
  bool success;
  std::string error_message;
};

class GrpcLogClient {
private:
  std::unique_ptr<LogService::Stub> stub;

public:
  GrpcLogClient(std::shared_ptr<Channel> channel)
      : stub(LogService::NewStub(
            std::static_pointer_cast<grpc::ChannelInterface>(channel))){};
  ~GrpcLogClient() = default;
  LogExportResponse Export(LogRecord &log);
};

class GrpcLogExporter : public LogExporter {
private:
  std::unique_ptr<GrpcLogClient> client;

public:
  GrpcLogExporter(std::shared_ptr<Channel> channel)
      : client(std::make_unique<GrpcLogClient>(channel)){};
  ~GrpcLogExporter() = default;
  void Export(LogRecord &log) override;
};

} // namespace logger

#endif // !LOG_GRPC_LOG_EXPORTER_H