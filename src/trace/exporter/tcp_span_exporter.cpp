#include "tcp_span_exporter.h"
#include "ThreadSafeQueue.h"
#include "span.h"
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

namespace trace
{

    std::mutex mtx;
    auto &queue = ThreadSafeQueue::getInstance();

    TCPSpanExporter::~TCPSpanExporter()
    {
        // 析构函数的实现，如果需要
    }

    void TCPSpanExporter::Export(SpanRecord &record)
    {
        std::string message = trace::Serialize(record);
        queue.push(message);
    }

    void TCPSpanExporter::Export(std::vector<SpanRecord> &records)
    {
        for (auto &record : records)
        {
            std::string message = trace::Serialize(record);
            queue.push(message);
        }
    }

} // namespace metric
