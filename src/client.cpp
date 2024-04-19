// C++ program to illustrate the client application in the
// socket programming
#include "ostream_exporter.h"
#include "protocol/message.h"
#include "span_context.h"
#include "trace/ostream_exporter.h"
#include "span_metadata.h"
#include <cstring>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "trace_provider.h"

void initTrace();
int main()
{
    initTrace();
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
    auto trace = trace::TraceProvider::GetTrace();
    trace::SpanContext context;
    auto span = trace->StartSpan("client", "client");
    msg.SetHeader("trace_id", trace->Id());
    msg.SetHeader("span_id", span->Id());
    msg.SetHeader("trace_flag", std::to_string(static_cast<int>(TraceFlag::kIsSampled)));
    msg.SetHeader("sampler", trace->Context()->GetSampler().Serialize());
    std::string message = msg.Serialize().c_str();

    send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);

    // closing socket
    close(clientSocket);
    span->SetStatus(trace::StatusCode::kOk);
    span->End();

    return 0;
}

void initTrace()
{
    auto exporter = std::make_unique<trace::OstreamSpanExporter>();
    auto processor = std::make_unique<trace::SimpleSpanProcessor>(std::move(exporter));
    trace::TraceProvider::InitProvider(std::move(processor));
}