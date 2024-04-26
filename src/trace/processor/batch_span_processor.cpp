#include "batch_span_processor.h"
#include <mutex>

namespace trace {
void BatchSpanProcessor::OnStart(Span &span) {}

void BatchSpanProcessor::OnEnd(Span &span) {

  SpanRecord record = SpanToRecord(span);
  std::lock_guard<std::mutex> lock_guard(this->mutex);
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
