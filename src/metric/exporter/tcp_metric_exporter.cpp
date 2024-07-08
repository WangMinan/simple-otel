#include "tcp_metric_exporter.h"
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

namespace metric
{

    std::mutex mtx;
    auto &queue = ThreadSafeQueue::getInstance();

    TCPMetricExporter::~TCPMetricExporter()
    {
        // 析构函数的实现，如果需要
    }

    void TCPMetricExporter::Export(MetricRecord &record)
    {
        std::string message = record.PrintJson();
        queue.push(message);
    }

    void TCPMetricExporter::Export(std::vector<MetricRecord *> &records)
    {
        for (auto &record : records)
        {
            std::string message = record->PrintJson();
            queue.push(message);
        }
    }

} // namespace metric
