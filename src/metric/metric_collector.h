#include <atomic>
#include <mutex>
#include <thread>
#include <utility>

#ifndef METRIC_COLLECTOR_H
#define METRIC_COLLECTOR_H

namespace metric {

class MetricsCollector {
public:
  MetricsCollector(unsigned int intervalSeconds)
      : intervalSeconds(intervalSeconds) {}
  void start();

private:
  unsigned int intervalSeconds;
  std::thread collectingThread;
  static std::atomic_bool created;
  static std::mutex mutex;
  double collectCpuUsage();
  std::pair<uint64_t, uint64_t> collectMemoryUsage();
};
} // namespace metric

#endif // !METRIC_COLLECTOR_H