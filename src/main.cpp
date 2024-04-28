#include "exporter/ostream_span_exporter.h"
#include "processor/post_sample_processor.h"
#include "sampler/tail_sampler.h"
#include "span_context.h"
#include "span_metadata.h"
#include "trace/trace.h"
#include "trace_provider.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>

#include "exporter/ostream_log_exporter.h"
#include <unistd.h>
void initTrace() {
  auto exporter = std::make_unique<trace::OstreamSpanExporter>();
  auto processor =
      std::make_unique<trace::SimpleSpanProcessor>(std::move(exporter));
  trace::TraceProvider::InitProvider(std::move(processor), "server");
}

void initPostTrace() {
  auto exporter = std::make_unique<trace::OstreamSpanExporter>();
  auto sampler = std::make_unique<trace::TailSampler>(3);
  auto processor = std::make_unique<trace::PostSampleProcessor>(
      std::move(exporter), std::move(sampler));
  trace::TraceProvider::InitProvider(std::move(processor), "server");
}

int main(int argc, char const *argv[]) {
  initTrace();
  auto trace = trace::TraceProvider::GetTrace();
  auto span1 = trace->StartSpan("outer" );
  std::cout << span1->GetId() << std::endl;
  auto span2 = trace->StartSpan("inner");
  std::cout << span2->GetId() << std::endl;
  std::cout << trace::Context::GetCurrentContext().GetSpanId() << std::endl;
  std::cout << trace::Context::GetParentContext().GetSpanId() << std::endl;
  auto span3 = trace->StartSpan("inner-inner");
  usleep(1000 * 1000);
  span3->SetStatus(trace::StatusCode::kOk);
  span3->End();
  usleep(500 * 1000);
  span2->SetStatus(trace::StatusCode::kOk);
  span2->End();
  std::cout << trace::Context::GetCurrentContext().GetSpanId() << std::endl;

  span1->SetStatus(trace::StatusCode::kOk);
  span1->End();

  return 0;
}