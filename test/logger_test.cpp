
#include "exporter/ostream_span_exporter.h"
#include "exporter/file_log_exporter.h"
#include "exporter/tcp_log_exporter.h"
#include "tcp_exporter.h"
#include "logger_factory.h"
#include "exporter/ostream_log_exporter.h"
#include "processor/span_processor.h"
#include "processor/simple_log_processor.h"
#include "span_metadata.h"
#include "trace_provider.h"
#include <memory>
#include <unistd.h>

void initLogger()
{
  auto exporter = common::make_unique<logger::TCPLogExporter>();
  auto processor =
      std::make_shared<logger::SimpleLogProcessor>(std::move(exporter));
  logger::LoggerFactory::InitFactory(std::move(processor), "test");
  // auto factory = logger::LoggerFactory::GetInstance();
}

void initTrace()
{
  auto exporter = common::make_unique<trace::OstreamSpanExporter>();
  auto processor =
      common::make_unique<trace::SimpleSpanProcessor>(std::move(exporter));
  trace::TraceProvider::InitProvider(std::move(processor), "logger_test");
}

int main(int argc, char const *argv[])
{
  std::string ip = "8.149.132.190";
  int port = 8088;
  initLogger();
  common::TCPExporter tcp_exporter(ip,port);
  initTrace();
  auto trace = trace::TraceProvider::GetTrace();
  auto span = trace->StartSpan("trace-log-test");
  auto loggrt = logger::LoggerFactory::Create("main");
  loggrt->Info("111");
  auto loggrt1 = logger::LoggerFactory::Create("main");
  loggrt->Info("222");
  sleep(3);
  span->SetStatus(trace::StatusCode::kOk);
  span->End();
}
