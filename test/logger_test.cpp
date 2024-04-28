
#include "exporter/ostream_span_exporter.h"
#include "logger_factory.h"
#include "exporter/ostream_log_exporter.h"
#include "processor/span_processor.h"
#include "processor/simple_log_processor.h"
#include "span_metadata.h"
#include "trace_provider.h"
#include <memory>

void initLogger() {
  auto exporter = std::make_unique<logger::OstreamLogExporter>();
  auto processor =
      std::make_shared<logger::SimpleLogProcessor>(std::move(exporter));
  logger::LoggerFactory::InitFactory(processor, "test");
  auto factory = logger::LoggerFactory::GetInstance();
}

void initTrace() {
  auto exporter = std::make_unique<trace::OstreamSpanExporter>();
  auto processor =
      std::make_unique<trace::SimpleSpanProcessor>(std::move(exporter));
  trace::TraceProvider::InitProvider(std::move(processor), "logger_test");
}

int main(int argc, char const *argv[]) {
    initLogger();
    initTrace();
    auto trace = trace::TraceProvider::GetTrace();
    auto span = trace->StartSpan("trace-log-test");
    sleep(1);
    span->SetStatus(trace::StatusCode::kOk);
    span->End();
}
