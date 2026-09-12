#include "sample.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <chrono>
#include <iostream>
#include <string>
class Echo final : public arm64test::Echo::Service {
    grpc::Status Call(grpc::ServerContext*, const arm64test::Payload* input,
                      arm64test::Payload* output) override {
        output->set_text("echo:" + input->text());
        return grpc::Status::OK;
    }
};
int main() {
    Echo service;
    int port = 0;
    grpc::ServerBuilder builder;
    builder.AddListeningPort("127.0.0.1:0", grpc::InsecureServerCredentials(), &port);
    builder.RegisterService(&service);
    auto server = builder.BuildAndStart();
    if (!server || port == 0) return 1;
    auto channel = grpc::CreateChannel("127.0.0.1:" + std::to_string(port), grpc::InsecureChannelCredentials());
    auto stub = arm64test::Echo::NewStub(channel);
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(10));
    arm64test::Payload input, output;
    input.set_text("native-arm64");
    const auto status = stub->Call(&context, input, &output);
    server->Shutdown();
    if (!status.ok() || output.text() != "echo:native-arm64") return 2;
    std::cout << "Installed protoc/plugin generation and loopback RPC passed\n";
}
