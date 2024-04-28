#include "exporter/grpc_span_exporter.h"
#include "exporter/grpc_log_exporter.h"
#include "logger_factory.h"
#include "processor/batch_span_processor.h"
#include "processor/simple_log_processor.h"
#include "span_metadata.h"
#include "trace_provider.h"
#include <arpa/inet.h>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <utility>

void initLogger() {

  auto exporter = std::make_unique<logger::GrpcLogExporter>(grpc::CreateChannel(
      "38.147.172.149:50051", grpc::InsecureChannelCredentials()));
  auto processor =
      std::make_unique<logger::SimpleLogProcessor>(std::move(exporter));
  logger::LoggerFactory::InitFactory(std::move(processor),
                                     "grpc_span_exporter_test");
}

void initTrace() {

  auto exporter = std::make_unique<trace::GrpcSpanExporter>(grpc::CreateChannel(
      "38.147.172.149:50051", grpc::InsecureChannelCredentials()));
  auto processor =
      std::make_unique<trace::BatchSpanProcessor>(std::move(exporter), 100);
  trace::TraceProvider::InitProvider(std::move(processor),
                                     "grpc_span_exporter_test");
}

int main() {
  initLogger();
  initTrace();
  auto logger = logger::LoggerFactory::Create();
  std::cout << "init" << std::endl;
  auto tracer = trace::TraceProvider::GetTrace();
  auto span1 = tracer->StartSpan("test");
  logger->Info("span1 start");
  logger->Warn("sleep 3 seconds");
  sleep(3);
  auto span2 = tracer->StartSpan("test2");
  logger->Info("span2 start");
  sleep(2);
  span2->SetStatus(trace::StatusCode::kOk);
  sleep(1);
  logger->Info("span2 end");
  span2->End();
  sleep(4);
  span1->SetStatus(trace::StatusCode::kOk);
  logger->Info("span1 end");
  span1->End();
}
