find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Generic test that uses conan libs
add_executable(intro main.cpp)

target_link_libraries(
  intro
  PUBLIC project_options project_warnings
  PRIVATE CLI11::CLI11
          fmt::fmt
          protobuf::libprotobuf
          gRPC::grpc++
          spdlog::spdlog)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

# ------------------
# Client executable.

add_executable(client_hw client_hw/main.cpp)

# TODO: Remove all of this cruft. It is part of the journey, so I want it in the history, but it is unnecessary.
# Use -I instead of -isystem when including conan dependencies, otherwise protobuf compilation fails with obscure
# issues around not finding PROTOBUF_VERSION and other macro definitions.
# set_target_properties(client_hw
#    PROPERTIES
#    NO_SYSTEM_FROM_IMPORTED ON
#    #COMPILE_FLAGS "-fPIC"
#    )
#
# Protobufs require at least C++11, detected using __cplusplus, which appears to not be set correctly.
# target_compile_definitions(client_hw PUBLIC __GXX_EXPERIMENTAL_CXX0X__)
#
# set_target_properties(protobuf::libprotobuf IMPORTED_GLOBAL TRUE)
# set_target_properties(gRPC::grpc++ PROPERTIES IMPORTED_GLOBAL TRUE)
# add_library(protobuf ALIAS protobuf::libprotobuf)
# set_target_properties(protobuf
#    PROPERTIES
#    NO_SYSTEM_FROM_IMPORTED ON)
#
# add_library(grpc++ ALIAS gRPC::grpc++)
#
#target_sources(client_hw PRIVATE client_hw/main.cpp ${HW_PROTO_SRCS} ${HW_GRPC_SRCS})
#target_sources(client_hw PRIVATE client_hw/main.cpp ${HW_PROTO_SRCS})

target_link_libraries(client_hw
  PUBLIC project_options project_warnings
  PRIVATE proto_hw)

target_link_system_libraries(client_hw
  PRIVATE CLI11::CLI11
          fmt::fmt
          protobuf::libprotobuf
          gRPC::grpc++
          spdlog::spdlog)

target_include_directories(client_hw
  PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include"
)

add_dependencies(client_hw generate_grpc_files)

# ------------------
# Server executable.

add_executable(server_hw server_hw/main.cpp)

target_include_directories(server_hw PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(
  server_hw
  PUBLIC project_options project_warnings
  PRIVATE proto_hw
          CLI11::CLI11
          fmt::fmt
          protobuf::libprotobuf
          gRPC::grpc++
          gRPC::grpc++_reflection
          spdlog::spdlog)

add_dependencies(server_hw generate_grpc_files)
