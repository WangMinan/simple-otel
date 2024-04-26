#include "log_record.h"

#ifndef LOG_LOG_EXPORTER_H
#define LOG_LOG_EXPORTER_H
namespace nlog {
class LogExporter {
public:
  LogExporter() = default;
  ~LogExporter() = default;
  virtual void Export(LogRecord &log) = 0;
};

} // namespace log

#endif // !LOG_LOG_EXPORTER_H