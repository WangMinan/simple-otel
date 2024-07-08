#include "period_task.h"
#include "span.h"
#include "span_processor.h"
#include "unique_ptr.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#ifndef TRACE_BATCH_SPAN_PROCESSOR_H
#define TRACE_BATCH_SPAN_PROCESSOR_H
namespace trace {

class BatchSpanProcessor : public SpanProcessor {
private:
  std::mutex mutex;
  std::vector<SpanRecord> span_records;
  // when the number of spans reaches threshold, export them
  int threshold;
  std::unique_ptr<PeriodTask> back_task;

public:
  BatchSpanProcessor(std::unique_ptr<SpanExporter> exporter_, int threshold_)
      : SpanProcessor(std::move(exporter_)), threshold(threshold_) {
    auto task = [this]() {
      std::lock_guard<std::mutex> lock_guard(this->mutex);
      std::cout << "Back task exporting" << std::endl;
      this->exporter->Export(this->span_records);
      this->span_records.clear();
    };
    back_task = common::make_unique<PeriodTask>(task, 1);
  };
  ~BatchSpanProcessor();
  void OnStart(Span &span) override;
  void OnEnd(Span &span) override;
};

} // namespace trace

#endif // !TRACE_BATCH_SPAN_PROCESSOR_H
