#include <vector>
#include "log_record.h"
#include "metric_record.h"
#include "span.h"
#include <thread>
#include <atomic>

#ifndef TCP_EXPORTER_H
#define TCP_EXPORTER_H
namespace common
{
    class TCPExporter
    {
    public:
        TCPExporter(const std::string &ip, int port);
        virtual ~TCPExporter();

    private:
        std::string server_ip;
        int server_port;
        int sockfd;
        std::thread worker_thread;
        std::atomic<bool> running;

        const int HEADER_SIZE = sizeof(uint32_t);

        void WriteToServer();
        void connectToServer();
        void disconnectFromServer();
        bool isConnected() const;
    };

} // namespace logger

#endif // !LOG_LOG_EXPORTER_H