#ifndef TCP_METRIC_EXPORTER_H
#define TCP_METRIC_EXPORTER_H

#include "metric_exporter.h"
#include <mutex>
#include <string>
#include <unistd.h> // For getcwd function

namespace metric {
class TCPMetricExporter : public MetricExporter {
public:
    TCPMetricExporter() {
    }
    virtual ~TCPMetricExporter();  // 声明虚析构函数

    void Export(std::vector<MetricRecord*> &records) override;
    void Export(MetricRecord &records);
};
} // namespace metric

#endif // FILE_LOG_EXPORTER_H
