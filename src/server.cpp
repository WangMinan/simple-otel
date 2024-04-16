#include <memory>
#include <ostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include "protocol/message.h"
#include "trace/trace.h"
#include "span_exporter.h"

int main(int argc, char const *argv[])
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(serverSocket, (sockaddr *)&addr, sizeof(addr));
    listen(serverSocket, 5);
    std::cout << "start" << std::endl;
    while (true)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
        char buf[1024];

        int len = recv(clientSocket, buf, sizeof(buf), 0);
        std::string str(buf);
        protocol::Message msg = protocol::Message::Deserialize(str);
        std::string trace_id = msg.GetHeader("trace_id");
        trace::Trace trace(trace_id);
        trace::SpanContext parent_context(trace_id, msg.GetHeader("span_id"));
        auto span = trace.StartSpan("server", "server", parent_context);

        write(clientSocket, buf, len);

        span->SetStatus(trace::StatusCode::kOk);
        span->End();
        close(clientSocket);
        trace::OstreamExporter exporter(span);
        exporter.Export();
    }

    close(serverSocket);
    return 0;
}
