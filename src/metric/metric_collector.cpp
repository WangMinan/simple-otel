#include "metric_collector.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace metric {

std::atomic_bool MetricsCollector::created = false;
std::mutex MetricsCollector::mutex;

void MetricsCollector::start() {
  if (created) {
    return;
  }
  std::lock_guard<std::mutex> lock_guard(mutex);
  if (created) {
    return;
  }
  collectingThread = std::thread([this] {
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(this->intervalSeconds));
      double cpuUsage = this->collectCpuUsage();
      auto [totalMemory, availableMemory] = this->collectMemoryUsage();
      std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
      std::cout << "Memory Usage: " << availableMemory << "/" << totalMemory
                << std::endl;
    }
  });
  collectingThread.detach();
  created = true;
}

double MetricsCollector::collectCpuUsage() {
  std::ifstream proc_stat("/proc/stat");
  std::string cpu;
  long long int user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  proc_stat >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
      softirq >> steal >> guest >> guest_nice;
  long int Idle = idle + iowait;
  long int NonIdle = user + nice + system + irq + softirq + steal;
  long int PrevTotal = Idle + NonIdle;
  long int PrevIdle = Idle;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(100)); // Wait for a short time
  proc_stat.clear();
  proc_stat.seekg(0, std::ios::beg);
  proc_stat >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
      softirq >> steal >> guest >> guest_nice;
  Idle = idle + iowait;
  NonIdle = user + nice + system + irq + softirq + steal;
  long int Total = Idle + NonIdle;
  long int totald = Total - PrevTotal;
  long int idled = Idle - PrevIdle;
  double CPU_Percentage = (double)(totald - idled) / totald * 100;
  return CPU_Percentage;
}
std::pair<uint64_t, uint64_t> MetricsCollector::collectMemoryUsage() {
  std::ifstream meminfo("/proc/meminfo");
  std::uint64_t totalMemory = 0, availableMemory = 0;
  std::string line;
  while (getline(meminfo, line)) {
    if (line.find("MemTotal:") == 0)
      sscanf(line.c_str(), "%*s%lu", &totalMemory);
    else if (line.find("MemAvailable:") == 0)
      sscanf(line.c_str(), "%*s%lu", &availableMemory);
  }
  return std::make_pair(totalMemory, availableMemory);
}
} // namespace metric
