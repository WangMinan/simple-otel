#ifndef TCP_LOG_EXPORTER_H
#define TCP_LOG_EXPORTER_H

#include "log_exporter.h"
#include <mutex>
#include <string>
#include <unistd.h> // For getcwd function

namespace logger {
class TCPLogExporter : public LogExporter {
public:
    TCPLogExporter() {
    }
    virtual ~TCPLogExporter();  // 声明虚析构函数

    void Export(LogRecord &record) override;
    void Export(std::vector<LogRecord> &records) override;
};
} // namespace logger

#endif // FILE_LOG_EXPORTER_H
