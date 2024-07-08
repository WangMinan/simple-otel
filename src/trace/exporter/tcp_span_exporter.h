#ifndef TCP_METRIC_EXPORTER_H
#define TCP_METRIC_EXPORTER_H

#include "span_exporter.h"
#include <mutex>
#include <string>
#include <unistd.h> // For getcwd function

namespace trace {
class TCPSpanExporter : public SpanExporter {
public:
    TCPSpanExporter() {
    }
    virtual ~TCPSpanExporter();  // 声明虚析构函数

    void Export(std::vector<SpanRecord> &records) override;
    void Export(SpanRecord &records);
};
} // namespace metric

#endif // FILE_LOG_EXPORTER_H
