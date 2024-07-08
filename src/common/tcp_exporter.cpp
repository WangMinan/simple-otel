#include "tcp_exporter.h"
#include "ThreadSafeQueue.h"
#include "log_record.h"
#include "metric_record.h"
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

namespace common
{
    auto &queue = ThreadSafeQueue::getInstance();

    TCPExporter::TCPExporter(const std::string &ip, int port) : server_ip(ip), server_port(port), sockfd(-1), running(true)
    {
        connectToServer();
        worker_thread = std::thread(&TCPExporter::WriteToServer, this);
    }

    TCPExporter::~TCPExporter()
    {
        running = false;
        if (worker_thread.joinable())
        {
            worker_thread.join();
        }
        disconnectFromServer();
        std::cerr << "Connection disconnected" << std::endl;
    }

    void TCPExporter::connectToServer()
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server_port);
        if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0)
        {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            close(sockfd);
            sockfd = -1;
            return;
        }

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            std::cerr << "Connection failed" << std::endl;
            close(sockfd);
            sockfd = -1;
            return;
        }
    }

    void TCPExporter::disconnectFromServer()
    {
        if (sockfd != -1)
        {
            close(sockfd);
            sockfd = -1;
        }
    }

    bool TCPExporter::isConnected() const
    {
        return sockfd != -1;
    }

    void TCPExporter::WriteToServer()
    {
        while (running)
        {
            if (!queue.empty())
            {
                std::string message = queue.pop();
                int l = send(sockfd, message.c_str(), message.size(), 0);
                std::cout << l << std::endl;
                if (l < 0)
                {
                    std::cerr << "Failed to send message" << std::endl;
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

} // namespace logger
