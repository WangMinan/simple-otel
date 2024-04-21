#include "log.h"

namespace log {
class LogExporter {
public:
  LogExporter() = default;
  ~LogExporter() = default;
  virtual void Export(Log &log) = 0;
};

} // namespace log
