#include "tcp_log_exporter.h"
#include "ThreadSafeQueue.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <mutex>
#include <sys/stat.h>
#include <cstring>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

namespace logger
{

    std::mutex mtx;
    auto &queue = ThreadSafeQueue::getInstance();

    TCPLogExporter::~TCPLogExporter()
    {
        // 析构函数的实现，如果需要
    }

    void TCPLogExporter::Export(LogRecord &record)
    {
        std::string message = logger::Serialize(record).c_str();
        queue.push(message);
    }

    void TCPLogExporter::Export(std::vector<LogRecord> &records)
    {
        for (auto &record : records)
        {
            std::string message = logger::Serialize(record).c_str();
            queue.push(message);
        }
    }

} // namespace logger
