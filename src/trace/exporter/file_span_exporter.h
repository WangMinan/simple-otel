#ifndef FILE_SPAN_EXPORTER_H
#define FILE_SPAN_EXPORTER_H

#include <string>
#include "span_exporter.h"
#include <mutex>
#include <unistd.h> // For getcwd function


namespace trace {

class FileSpanExporter : public SpanExporter {
public:
    FileSpanExporter(const std::string &directory_ = "../tmp/span") : directory(directory_) {};
    void WriteToFile(const std::string &filename, SpanRecord &record);
    std::string GetLogFileName(long startTime);
    void Export(SpanRecord &record) override;
    void Export(std::vector<SpanRecord> &records) override;

private:
    std::string directory;
    std::mutex file_mutex;
    std::string GetCurrentWorkingDirectory() {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            return std::string(cwd);
        } else {
            return "";
        }
    }
};

} // namespace trace

#endif // FILE_SPAN_EXPORTER_H
