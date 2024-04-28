#include "exporter/log_exporter.h"
#include "processor/log_processor.h"
#include <initializer_list>
#include <memory>
#include <string>

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H
namespace logger {
class Logger {
private:
  std::shared_ptr<LogProcessor> processor;
  std::string service_name;

protected:
  void
  EmitLog(std::string trace_id, std::string span_id, std::string content,
          std::string service_name, LogLevel level,
          std::initializer_list<std::pair<std::string, std::string>> tags = {});
  void EmitLog(std::string content, LogLevel level);

public:
  Logger(std::shared_ptr<LogProcessor> processor_, std::string service_name_)
      : processor(processor_), service_name(service_name_){};
  ~Logger() = default;
  void Debug(std::string content);
  void Info(std::string content);
  void Warn(std::string content);
  void Error(std::string content);
};

} // namespace logger
#endif // !LOG_LOGGER_H
