#include "log_record.h"
#include <vector>

#ifndef LOG_LOG_EXPORTER_H
#define LOG_LOG_EXPORTER_H
namespace logger {
class LogExporter {
public:
  LogExporter() = default;
  ~LogExporter() = default;
  virtual void Export(LogRecord &log) = 0;
  virtual void Export(std::vector<LogRecord> &records) = 0;
};

} // namespace logger

#endif // !LOG_LOG_EXPORTER_H