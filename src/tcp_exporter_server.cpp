#include "exporter/ostream_span_exporter.h"
#include "processor/post_sample_processor.h"
#include "protocol/message.h"
#include "sampler/tail_sampler.h"
#include "span_context.h"
#include "trace_provider.h"
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <thread>

void initTrace()
{
  auto exporter = common::make_unique<trace::OstreamSpanExporter>();
  auto processor =
      common::make_unique<trace::SimpleSpanProcessor>(std::move(exporter));
  trace::TraceProvider::InitProvider(std::move(processor), "server");
}

void initPostTrace()
{
  auto exporter = common::make_unique<trace::OstreamSpanExporter>();
  auto sampler = common::make_unique<trace::TailSampler>(3);
  auto processor =
      common::make_unique<trace::PostSampleProcessor>(std::move(exporter));
  trace::TraceProvider::InitProvider(std::move(processor), "server",
                                     std::move(sampler));
}

void handleClient(int clientSocket) {
    char buf[2048];
    while (true) {
        memset(buf, 0, sizeof(buf));
        int len = recv(clientSocket, buf, sizeof(buf) - 1, 0);
        if (len <= 0) {
            // 客户端关闭连接或出现错误
            break;
        }
        std::string str(buf, len);
        std::cout << str << std::endl;
        // std::cout << "Length: " << len << std::endl;

        // Echo message back to client
        // write(clientSocket, str.c_str(), len);

        try {
            std::this_thread::sleep_for(std::chrono::seconds(3));
        } catch (const std::exception &e) {
            break;
        }

        // Echo message again to client after sleep
        // write(clientSocket, str.c_str(), len);
    }
    close(clientSocket);
}

int main(int argc, char const *argv[])
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8088);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server started, listening on port 8088" << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // 创建一个线程来处理客户端连接
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // 让线程在后台运行
    }

    close(serverSocket);
    return 0;
}
