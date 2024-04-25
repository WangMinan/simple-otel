#include "batch_span_processor.h"

namespace trace {
void BatchSpanProcessor::OnStart(Span &span) {}

void BatchSpanProcessor::OnEnd(Span &span) {

  SpanRecord record = SpanToRecord(span);
  this->span_records.emplace_back(record);
  if (this->span_records.size() >= this->threshold) {
    this->exporter->Export(this->span_records);
  }
  // TODO: 定时自动导出
}

BatchSpanProcessor::~BatchSpanProcessor() {
  this->exporter->Export(this->span_records);
}

} // namespace trace
