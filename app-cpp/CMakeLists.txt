cmake_minimum_required(VERSION 3.22)

project(sockets
        VERSION 1.0
        DESCRIPTION "C++ Client-Server gRPC Example"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig)
pkg_search_module(GRPC REQUIRED grpc++)

find_package(Protobuf REQUIRED)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(FileRetrievalEngine proto/file_retrieval_engine.proto)
target_link_libraries(FileRetrievalEngine PUBLIC ${GRPC_LIBRARIES} ${Protobuf_LIBRARIES})
protobuf_generate(TARGET FileRetrievalEngine)
protobuf_generate(
    TARGET FileRetrievalEngine
    LANGUAGE grpc
    PLUGIN protoc-gen-grpc=/usr/bin/grpc_cpp_plugin
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
)

add_executable(server src/Server.cpp src/FileRetrievalEngineImpl.cpp)
target_include_directories(server PUBLIC include)
target_link_libraries(server FileRetrievalEngine)

add_executable(client src/Client.cpp)
target_link_libraries(client FileRetrievalEngine)