#include "exporter/metric_exporter.h"
#include "metric_record.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#ifndef METRIC_COLLECTOR_H
#define METRIC_COLLECTOR_H

namespace metric {

class MetricsCollector {
  public:
    MetricsCollector(unsigned int intervalSeconds, std::string service_name_,
                    std::unique_ptr<MetricExporter> &&exporter_)
        : intervalSeconds(intervalSeconds), service_name(service_name_),
          exporter(std::move(exporter_)) {}
    void start();

  private:
    unsigned int intervalSeconds;
    std::thread collectingThread;
    static std::atomic_bool created;
    static std::mutex mutex;
    std::string service_name;
    std::unique_ptr<MetricExporter> exporter;
    CpuMetricRecord collectCpuUsage();
    MemoryMetricRecord collectMemoryUsage();
  };
} // namespace metric

#endif // !METRIC_COLLECTOR_H