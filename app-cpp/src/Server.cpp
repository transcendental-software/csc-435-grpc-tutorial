#include <memory>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpc/support/time.h>

#include "FileRetrievalEngineImpl.hpp"

class Server
{
    std::string address;
    std::string port;

    std::unique_ptr<grpc::Server> RPCserver;
    
    public:
        Server(std::string address, std::string port) : address(address), port(port) { }
        virtual ~Server() = default;

        void run()
        {
            FileRetrievalEngineImpl service;    
            grpc::ServerBuilder builder;
            builder.AddListeningPort(address + ":" + port, grpc::InsecureServerCredentials());
            builder.RegisterService((grpc::Service*) &service);
            RPCserver = builder.BuildAndStart();
            RPCserver->Wait();
        }

        void shutdown()
        {
            RPCserver->Shutdown();
        }
};

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "USE: ./server <port>" << std::endl;
        return 1;
    }

    std::string port(argv[1]);

    Server server("0.0.0.0", port);
    std::thread serverThread(&Server::run, &server);

    std::string command;
    
    std::cout << "gRPC Server started!" << std::endl;
    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command.compare("quit") == 0) {
            server.shutdown();
            serverThread.join();
            std::cout << "gRPC Server terminated!" << std::endl;
            break;
        }
    }
    
    return 0;
}