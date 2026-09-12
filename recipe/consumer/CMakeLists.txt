cmake_minimum_required(VERSION 3.20)
project(grpc_consumer LANGUAGES CXX)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
add_custom_command(
  OUTPUT sample.pb.cc sample.pb.h sample.grpc.pb.cc sample.grpc.pb.h
  COMMAND protobuf::protoc
  ARGS --proto_path=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
       --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
       --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
       ${CMAKE_CURRENT_SOURCE_DIR}/sample.proto
  DEPENDS sample.proto protobuf::protoc gRPC::grpc_cpp_plugin)
add_executable(grpc_consumer main.cpp sample.pb.cc sample.grpc.pb.cc)
target_include_directories(grpc_consumer PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(grpc_consumer PRIVATE cxx_std_17)
target_link_libraries(grpc_consumer PRIVATE gRPC::grpc++ protobuf::libprotobuf)
