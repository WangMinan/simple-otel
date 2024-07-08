#include <vector>
#include <fstream>
#include <iostream>
#include "span_exporter.h" // 假设包含SpanExporter类定义
#include "../span.h"
#include <iomanip>
#include "file_span_exporter.h"
#include <ctime>
#include <mutex>
#include <chrono>
#include <string>
#include <memory>
#include <ctime>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

namespace trace {

void FileSpanExporter::Export(SpanRecord &record)  {
    std::string filename = FileSpanExporter::GetLogFileName(record.start_time);
    WriteToFile(filename, record);
}

void FileSpanExporter::Export(std::vector<SpanRecord> &records)  {
    for (auto &record : records) {
        std::string filename = FileSpanExporter::GetLogFileName(record.start_time);
        WriteToFile(filename, record);
    }
}

std::string FileSpanExporter::GetLogFileName(long startTime) {
    std::time_t time = startTime / 1000;
    std::tm tm = *std::localtime(&time);

    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H", &tm);
    std::ostringstream oss;
    oss << "/log_" << buffer << ".log";
    return oss.str();
}

void FileSpanExporter::WriteToFile(const std::string &filename, SpanRecord &record) {
    std::lock_guard<std::mutex> guard(file_mutex);
    // std::string absolutePath = GetCurrentWorkingDirectory() + "/" + filename; // Combine current working directory and relative path
    std::ofstream outfile;
    outfile.open("../tmp/span" + filename, std::ios::out | std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file: " << "../tmp/span" + filename << std::endl;
        return;
    }
    
    std::cout << trace::Serialize(record) << std::endl;
    outfile << trace::Serialize(record) << std::endl;
    outfile.close();
}

} // namespace trace
