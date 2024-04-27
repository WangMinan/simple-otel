#ifndef LOG_LOGGER_FACTORY_H
#define LOG_LOGGER_FACTORY_H

#include "logger.h"
#include <memory>
#include <string>
namespace logger {
class LoggerFactory {
private:
  std::string service_name;
  std::shared_ptr<LogProcessor> processor;
  static LoggerFactory factory;

public:
  LoggerFactory() = default;
  LoggerFactory(std::string service_name_,
                std::shared_ptr<LogProcessor> processor_)
      : service_name(service_name_), processor(processor_){};
  ~LoggerFactory() = default;
  static LoggerFactory &GetInstance();
  static void InitFactory(std::shared_ptr<LogProcessor> processor_,
                   std::string service_name_);
  static std::unique_ptr<Logger> Create();
};

} // namespace logger

#endif // !LOG_LOGGER_FACTORY_H