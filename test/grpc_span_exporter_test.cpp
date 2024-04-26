#include "exporter/grpc_span_exporter.h"
#include "span_metadata.h"
#include "trace_provider.h"
#include <arpa/inet.h>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>


void initTrace() {

  auto exporter = std::make_unique<trace::GrpcSpanExporter>(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  auto processor =
      std::make_unique<trace::SimpleSpanProcessor>(std::move(exporter));
  trace::TraceProvider::InitProvider(std::move(processor));
}

int main() {

  initTrace();
  std::cout << "init" << std::endl;
  auto tracer = trace::TraceProvider::GetTrace();
  auto span1 = tracer->StartSpan("test", "service1");
  sleep(3);
  auto span2 = tracer->StartSpan("test2", "service1");
  sleep(2);
  span2->SetStatus(trace::StatusCode::kOk);
  sleep(1);
  span2->End();
  sleep(4);
  span1->SetStatus(trace::StatusCode::kOk);
  span1->End();
}
