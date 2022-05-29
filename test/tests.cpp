#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

#include "helloworld.pb.h"
#include "helloworld.grpc.pb.h"

using Catch::Matchers::Equals;
using trompeloeil::_;

// NOLINT(misc-no-recursion)
unsigned int Factorial(unsigned int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

class MockGreeterService : public helloworld::Greeter::StubInterface {
    MAKE_MOCK3(SayHello,
               ::grpc::Status(::grpc::ClientContext*, const ::helloworld::HelloRequest&, ::helloworld::HelloReply*),
               override);
    MAKE_MOCK3(AsyncSayHelloRaw,
               ::grpc::ClientAsyncResponseReaderInterface<::helloworld::HelloReply>*(::grpc::ClientContext*,
                                                                                     const ::helloworld::HelloRequest&,
                                                                                     ::grpc::CompletionQueue*),
               override);
    MAKE_MOCK3(PrepareAsyncSayHelloRaw,
               ::grpc::ClientAsyncResponseReaderInterface<::helloworld::HelloReply>*(::grpc::ClientContext*,
                                                                                     const ::helloworld::HelloRequest&,
                                                                                     ::grpc::CompletionQueue*),
               override);
};

TEST_CASE("Can say hello", "[grpc]") {
    grpc::ClientContext context;
    helloworld::HelloRequest request;
    request.set_name("client_hw");
    helloworld::HelloReply reply;

    MockGreeterService stub;
    ALLOW_CALL(stub, SayHello(_, _, _))
            .LR_SIDE_EFFECT(reply.set_message("Hello"))
            .RETURN(grpc::Status::OK);
            //.TIMES(AT_LEAST(1));

    SECTION("making one call") {
        reply.clear_message();
        REQUIRE(reply.message().empty());
        REQUIRE(stub.SayHello(&context, request, &reply).error_code() == grpc::StatusCode::OK);
        REQUIRE_THAT(reply.message(), Catch::Matchers::Equals("Hello"));
    }
}
