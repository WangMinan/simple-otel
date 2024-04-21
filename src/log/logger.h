#include "log_exporter.h"
#include <initializer_list>
#include <memory>
#include <string>

namespace log {
class Logger {
private:
  std::unique_ptr<LogExporter> exporter;

public:
  Logger(std::unique_ptr<LogExporter> &&exporter_)
      : exporter(std::move(exporter_)){};
  ~Logger() = default;
  void Debug(std::string content);
  void
  Debug(std::string trace_id, std::string span_id, std::string content,
        std::string service_name,
        std::initializer_list<std::pair<std::string, std::string>> tags = {});
};

} // namespace log
