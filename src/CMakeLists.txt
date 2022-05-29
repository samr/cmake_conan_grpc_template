find_package(absl CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(gRPC REQUIRED)

# Generic test that uses conan libs
add_executable(intro main.cpp)

target_link_libraries(
  intro
  PUBLIC project_options project_warnings
  PRIVATE absl::strings
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

# Client executable.
add_executable(client_hw)

message("Header: ${HW_GRPC_SRCS}")

set_target_properties(protobuf::libprotobuf PROPERTIES IMPORTED_GLOBAL TRUE)
set_target_properties(gRPC::grpc++ PROPERTIES IMPORTED_GLOBAL TRUE)

target_include_directories(client_hw PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../generated"
                                             "${CMAKE_BINARY_DIR}/configured_files/include")

target_sources(client_hw PRIVATE client_hw/main.cpp ${HW_PROTO_SRCS} ${HW_GRPC_SRCS})

target_link_libraries(
  client_hw
  PUBLIC project_options project_warnings
  PRIVATE CLI11::CLI11
          fmt::fmt
          gRPC::grpc++
          protobuf::libprotobuf
          spdlog::spdlog)

set_source_files_properties(client_hw PROPERTIES COMPILE_OPTIONS "-fPIC")

add_dependencies(client_hw generate_grpc_files)

# Server executable.
add_executable(server_hw)

target_include_directories(server_hw PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../generated"
                                             "${CMAKE_BINARY_DIR}/configured_files/include")

target_sources(server_hw PRIVATE server_hw/main.cpp ${HW_PROTO_SRCS} ${HW_GRPC_SRCS})

target_link_libraries(
  server_hw
  PUBLIC project_options project_warnings
  PRIVATE CLI11::CLI11
          fmt::fmt
          gRPC::grpc++
          protobuf::libprotobuf
          spdlog::spdlog)

add_dependencies(server_hw generate_grpc_files)
