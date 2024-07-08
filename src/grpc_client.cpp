#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "model/message.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using message::Greeter;
using message::MessageRequest;
using message::MessageReply;

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

    std::string SayMessage(const std::string& user) {
        MessageRequest request;
        request.set_name(user);

        MessageReply reply;

        ClientContext context;

        Status status = stub_->SayMessage(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << "RPC failed: " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string reply = greeter.SayMessage(user);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}
