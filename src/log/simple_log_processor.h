#ifndef LOG_LOG_PROCESSOR_H
#define LOG_LOG_PROCESSOR_H

#include "log_processor.h"
#include <memory>
namespace logger {
class SimpleLogProcessor : public LogProcessor {
public:
  SimpleLogProcessor(std::unique_ptr<LogExporter> &&exporter_)
      : LogProcessor(std::move(exporter_)) {}
  ~SimpleLogProcessor() = default;
  void Process(LogRecord &log) override;
};

} // namespace logger

#endif // !LOG_LOG_PROCESSOR_H