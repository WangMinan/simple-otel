#include "logger_factory.h"
#include "unique_ptr.h"
#include <memory>

namespace logger {

    LoggerFactory LoggerFactory::factory;
    LoggerFactory &LoggerFactory::GetInstance() { return factory; }

    void LoggerFactory::InitFactory(std::shared_ptr<LogProcessor> processor_,
                                    std::string service_name_) {
      factory = LoggerFactory(service_name_, processor_);
    }

    std::unique_ptr<Logger> LoggerFactory::Create(std::string function_name) {
      auto factory = GetInstance();
      return common::make_unique<Logger>(factory.processor, factory.service_name, function_name);
    }

} // namespace logger
