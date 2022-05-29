#include <iostream>
#include <optional>
#include <string>

#include <CLI/CLI.hpp>
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

#include "helloworld.grpc.pb.h"
#include "internal_use_only/config.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char** argv) {
    CLI::App app{fmt::format("{} version {}", myproject::cmake::project_name, myproject::cmake::project_version)};

    std::optional<std::string> server;
    app.add_option("-s,--server", server, "Server to contact")->required();
    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");

    CLI11_PARSE(app, argc, argv);

    if (show_version) {
        fmt::print("{}\n", myproject::cmake::project_version);
        return EXIT_SUCCESS;
    }

    const auto channel = grpc::CreateChannel(*server, grpc::InsecureChannelCredentials());
    const auto stub = helloworld::Greeter::NewStub(channel);

    grpc::ClientContext context;
    helloworld::HelloRequest request;
    request.set_name("client_hw");
    helloworld::HelloReply reply;
    grpc::Status status = stub->SayHello(&context, request, &reply);

    if (status.ok()) {
        std::cout << "Status was ok: " << reply.message() << '\n';
    } else {
        std::cout << "Status was not ok: " << status.error_message() << '\n';
    }

    return 0;
}
