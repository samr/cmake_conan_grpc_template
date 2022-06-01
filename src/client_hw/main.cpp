#include <iostream>
#include <optional>
#include <string>

#include <CLI/CLI.hpp>
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

#include <google/protobuf/port_def.inc>

// TODO: Remove this, but it all seems to be strangeness related to -isystem vs -I includes.
// #ifdef PROTOBUF_VERSION
// #error PROTOBUF_VERSION was previously defined
// #endif
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
//#define PROTOBUF_VERSION 3020000
//#if (PROTOBUF_VERSION < 3020000 || PROTOBUF_VERSION >= 3020000)
//#error This file was generated by a newer version of protoc which is
//#error incompatible with your Protocol Buffer headers. Please update
//#error your headers.
//#endif
// #ifndef PROTOBUF_VERSION
// #error The PROTOBUF_VERSION should be defined.
// #endif

#include <google/protobuf/port_undef.inc>

#include "helloworld.pb.h"
#include "helloworld.grpc.pb.h"
#include "internal_use_only/config.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char** argv) {
    CLI::App app{fmt::format("{} version {}", cmake_conan_grpc_template::cmake::project_name,
                             cmake_conan_grpc_template::cmake::project_version)};

    std::optional<std::string> server;
    app.add_option("-s,--server", server, "Server to contact")->required();
    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");

    CLI11_PARSE(app, argc, argv);

    if (show_version) {
        fmt::print("{}\n", cmake_conan_grpc_template::cmake::project_version);
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
