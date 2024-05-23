#include <memory>
#include <chrono>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpc/support/time.h>

#include "Server.hpp"
#include "FileRetrievalEngineImpl.hpp"

void Server::run()
{
    std::thread thread(&Server::waitForQuit, this);

    FileRetrievalEngineImpl service;
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address + ":" + port, grpc::InsecureServerCredentials());
    builder.RegisterService((grpc::Service*) &service);
    RPCserver = builder.BuildAndStart();
    RPCserver->Wait();
    
    thread.join();
}

void Server::waitForQuit()
{
    std::string command;
    
    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command.compare("quit") == 0) {
            RPCserver->Shutdown();
            std::cout << "Server terminated!" << std::endl;
            break;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "USE: ./server <port>" << std::endl;
        return 1;
    }

    std::string port(argv[1]);

    Server server("0.0.0.0", port);
    server.run();
    
    return 0;
}