#ifndef LOG_BATCH_LOG_PROCESSOR_H
#define LOG_BATCH_LOG_PROCESSOR_H

#include "log_record.h"
#include "period_task.h"
#include "processor/log_processor.h"
#include <memory>
#include <mutex>
#include <vector>

namespace logger {
class BatchLogProcessor : public LogProcessor {
private:
  std::mutex mutex;
  int threshold;
  std::vector<LogRecord> log_records;
  std::unique_ptr<PeriodTask> back_task;

public:
  BatchLogProcessor(std::unique_ptr<LogExporter> exporter_, int threshold_)
      : LogProcessor(std::move(exporter_)), threshold(threshold_) {
    auto task = [this]() {
      std::lock_guard<std::mutex> lock_guard(this->mutex);
      this->exporter->Export(this->log_records);
      this->log_records.clear();
    };
    back_task = std::make_unique<PeriodTask>(task, 1);
  };
  ~BatchLogProcessor();
  void Process(LogRecord &log) override;
};

} // namespace logger

#endif // !LOG_BATCH_LOG_PROCESSOR_H