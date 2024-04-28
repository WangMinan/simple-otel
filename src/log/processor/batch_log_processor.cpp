#include "batch_log_processor.h"
#include <mutex>

logger::BatchLogProcessor::~BatchLogProcessor() {
  std::lock_guard<std::mutex> lock_guard(this->mutex);
  this->exporter->Export(this->log_records);
  this->log_records.clear();
}

void logger::BatchLogProcessor::Process(LogRecord &log) {
  std::lock_guard<std::mutex> lock_guard(this->mutex);
  this->log_records.emplace_back(log);
  if (this->log_records.size() >= this->threshold) {
    this->exporter->Export(this->log_records);
    this->log_records.clear();
  }
}
