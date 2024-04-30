
#include "collector/metric_service.pb.h"
#include "model/metric.pb.h"
#include <sstream>
#include <string>
#include <unordered_map>

#ifndef METRIC_RECORD_H
#define METRIC_RECORD_H

inline std::string
printLabels(std::unordered_map<std::string, std::string> &labels);

namespace metric {
class MetricRecord {
protected:
  std::string name;
  std::string service_name;
  std::string description;
  std::unordered_map<std::string, std::string> labels;
  long timestamp;

public:
  MetricRecord(std::string name_, std::string service_name_,
               std::string description_, long timestamp_,
               std::unordered_map<std::string, std::string> labels_ = {})
      : name(name_), service_name(service_name_), description(description_),
        timestamp(timestamp_), labels(labels_){};
  ~MetricRecord() = default;
  virtual std::string PrintJson() = 0;
  virtual void
  GetProtoMetric(arktouros::proto::collector::v1::Metric *new_metric) = 0;
};

class CpuMetricRecord : public MetricRecord {
private:
  double cpu_usage;

public:
  CpuMetricRecord(std::string name_, std::string service_name_,
                  std::string description_, long timestamp_, double cpu_usage_,
                  std::unordered_map<std::string, std::string> labels_ = {})
      : MetricRecord(name_, service_name_, description_, timestamp_, labels_),
        cpu_usage(cpu_usage_){};
  ~CpuMetricRecord() = default;
  std::string PrintJson() override {
    std::ostringstream oss;
    oss << "{"
        << "\n  \"name\": \"" << name << "\","
        << "\n  \"service_name\": \"" << service_name << "\","
        << "\n  \"description\": \"" << description << "\","
        << "\n  \"timestamp\": " << timestamp << ","
        << "\n  \"labels\": " << printLabels(labels) << ","
        << "\n  \"cpu_usage\": " << cpu_usage << "\n}";
    return oss.str();
  };

  void
  GetProtoMetric(arktouros::proto::collector::v1::Metric *new_metric) override {
    auto gauge = new_metric->mutable_gauge();
    gauge->set_value(this->cpu_usage);
    auto metric = gauge->mutable_metric();
    metric->set_metric_type(arktouros::proto::metric::v1::MetricType::GAUGE);
    metric->set_name(this->name);
    metric->set_service_name(this->service_name);
    metric->set_description(this->description);
    metric->set_timestamp(this->timestamp);
    auto labels_ = metric->mutable_labels();
    for (auto label : this->labels) {
      labels_->insert({label.first, label.second});
    }
  };
};

class MemoryMetricRecord : public MetricRecord

{
private:
  double memory_usage;
  double memory_available;
  double memory_total;

public:
  MemoryMetricRecord(std::string name_, std::string service_name_,
                     std::string description_, long timestamp_,
                     double memory_usage_, double memory_available_,
                     double memory_total_,
                     std::unordered_map<std::string, std::string> labels_ = {})
      : MetricRecord(name_, service_name_, description_, timestamp_, labels_),
        memory_usage(memory_usage_), memory_available(memory_available_),
        memory_total(memory_total_){};
  ~MemoryMetricRecord() = default;
  std::string PrintJson() override {
    std::ostringstream oss;
    oss << "{"
        << "\n  \"name\": \"" << name << "\","
        << "\n  \"service_name\": \"" << service_name << "\","
        << "\n  \"description\": \"" << description << "\","
        << "\n  \"timestamp\": " << timestamp << ","
        << "\n  \"labels\": " << printLabels(labels) << ","
        << "\n  \"memory_usage\": " << memory_usage << ","
        << "\n  \"memory_available\": " << memory_available << ","
        << "\n  \"memory_total\": " << memory_total << ","
        << "\n}";
    return oss.str();
  };

  void
  GetProtoMetric(arktouros::proto::collector::v1::Metric *new_metric) override {
    auto histogram = new_metric->mutable_histogram();
    histogram->set_sample_sum(this->memory_total);
    histogram->set_sample_count(this->memory_usage);

    auto metric = histogram->mutable_metric();
    metric->set_metric_type(arktouros::proto::metric::v1::MetricType::GAUGE);
    metric->set_name(this->name);
    metric->set_service_name(this->service_name);
    metric->set_description(this->description);
    metric->set_timestamp(this->timestamp);
    auto labels_ = metric->mutable_labels();
    for (auto label : this->labels) {
      labels_->insert({label.first, label.second});
    }
  }
};

} // namespace metric

inline std::string
printLabels(std::unordered_map<std::string, std::string> &labels) {
  std::string labels_str = "{";
  for (auto &label : labels) {
    labels_str += "\"" + label.first + "\": \"" + label.second + "\",";
  }
  labels_str += "}";
  return labels_str;
}

#endif // !METRIC_RECORD_H
