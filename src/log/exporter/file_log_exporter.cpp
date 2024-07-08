#include "file_log_exporter.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <sys/stat.h>

namespace logger {

FileLogExporter::~FileLogExporter() {
    // 析构函数的实现，如果需要
}

void FileLogExporter::Export(LogRecord &record) {
    std::string filename = GetLogFileName(record.GetTimestamp());
    WriteToFile(filename, record);
}

void FileLogExporter::Export(std::vector<LogRecord> &records) {
    for (auto &record : records) {
        std::string filename = GetLogFileName(record.GetTimestamp());
        WriteToFile(filename, record);
    }
}

std::string FileLogExporter::GetLogFileName(long startTime) {
    std::time_t time = startTime / 1000;
    std::tm tm = *std::localtime(&time);

    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H", &tm);
    std::ostringstream oss;
    oss << "/log_" << buffer << ".log";
    return oss.str();
}

void FileLogExporter::WriteToFile(const std::string &filename, LogRecord &record) {
    std::lock_guard<std::mutex> guard(file_mutex);
    std::ofstream outfile;
    outfile.open(directory + filename, std::ios::out | std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file: " << directory + filename << std::endl;
        return;
    }

    outfile << logger::Serialize(record) << std::endl;
    outfile.close();
}

std::string FileLogExporter::GetCurrentWorkingDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return std::string(cwd);
    } else {
        return "";
    }
}

} // namespace logger
