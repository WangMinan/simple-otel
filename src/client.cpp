// C++ program to illustrate the client application in the
// socket programming
#include "protocol/message.h"
#include "span_context.h"
#include "span_exporter.h"
#include "span_metadata.h"
#include "trace.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "trace_provider.h"

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr *)&serverAddress,
            sizeof(serverAddress));

    // sending data
    protocol::Message msg;
    auto trace = trace::TraceProvider::StartTrace();
    trace::SpanContext context;
    auto span = trace->StartSpan("client", "client", context);
    msg.SetHeader("trace_id", trace->Id());
    msg.SetHeader("span_id", span->Id());
    msg.SetHeader("trace_flag", std::to_string(static_cast<int>(TraceFlag::kIsDiscarded)));
    std::string message = msg.Serialize().c_str();

    send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);

    // closing socket
    close(clientSocket);
    span->SetStatus(trace::StatusCode::kOk);
    span->End();
    trace::OstreamExporter exporter(span);
    exporter.Export();

    return 0;
}
