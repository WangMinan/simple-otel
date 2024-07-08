#ifndef FILE_LOG_EXPORTER_H
#define FILE_LOG_EXPORTER_H

#include "log_exporter.h"
#include <mutex>
#include <string>
#include <unistd.h> // For getcwd function

namespace logger {
class FileLogExporter : public LogExporter {
public:
    FileLogExporter(const std::string &directory_ = "../tmp/log") : directory(directory_) {}
    virtual ~FileLogExporter();  // 声明虚析构函数

    void WriteToFile(const std::string &filename, LogRecord &record);
    std::string GetLogFileName(long startTime);
    void Export(LogRecord &record) override;
    void Export(std::vector<LogRecord> &records) override;

private:
    std::string directory;
    std::mutex file_mutex;
    std::string GetCurrentWorkingDirectory();
};
} // namespace logger

#endif // FILE_LOG_EXPORTER_H
