#include "exporter/ostream_span_exporter.h"
#include "processor/batch_span_processor.h"
#include "span_metadata.h"
#include "trace_provider.h"
#include <memory>
#include <unistd.h>

void initTrace() {
  auto exporter = common::make_unique<trace::OstreamSpanExporter>();
  auto processor =
      common::make_unique<trace::BatchSpanProcessor>(std::move(exporter), .10);
  trace::TraceProvider::InitProvider(std::move(processor), "logger_test");
}

int main(int argc, char const *argv[]) {
  initTrace();
  auto trace = trace::TraceProvider::GetTrace();
  auto span1 = trace->StartSpan("span1");
  auto span2 = trace->StartSpan("span2");
  span2->SetStatus(trace::StatusCode::kOk);
  span2->End();
  sleep(3);
  span1->SetStatus(trace::StatusCode::kOk);
  span1->End();
  return 0;
}
