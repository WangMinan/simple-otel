#ifndef LOGGER_LOG_PROCESSOR_H
#define LOGGER_LOG_PROCESSOR_H

#include "exporter/log_exporter.h"
#include <memory>
#include <utility>
namespace logger {
class LogProcessor {
protected:
  std::unique_ptr<LogExporter> exporter;

public:
  LogProcessor(std::unique_ptr<LogExporter> &&exporter_)
      : exporter(std::move(exporter_)){};
  ~LogProcessor() = default;
  virtual void Process(LogRecord &log) = 0;
};

} // namespace logger

#endif // !LOG_LOG_PROCESSOR_H