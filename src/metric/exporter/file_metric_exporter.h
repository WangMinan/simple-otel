#ifndef FILE_METRIC_EXPORTER_H
#define FILE_METRIC_EXPORTER_H

#include "metric_exporter.h"
#include <mutex>
#include <string>
#include <unistd.h> // For getcwd function

namespace metric {
class FileMetricExporter : public MetricExporter {
public:
    FileMetricExporter(const std::string &directory_ = "../tmp/metric") : directory(directory_) {}
    virtual ~FileMetricExporter();  // 声明虚析构函数

    void WriteToFile(const std::string &filename, MetricRecord &record);
    std::string GetLogFileName(long startTime);
    void Export(std::vector<MetricRecord*> &records) override;

private:
    std::string directory;
    std::mutex file_mutex;
    std::string GetCurrentWorkingDirectory();
};
} // namespace metric

#endif // FILE_LOG_EXPORTER_H
