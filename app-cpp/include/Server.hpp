#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <mutex>

#include <grpc/grpc.h>
#include <grpcpp/server.h>

class Server
{
    std::string address;
    std::string port;

    std::unique_ptr<grpc::Server> RPCserver;
    
    public:
        Server(std::string address, std::string port) : address(address), port(port) { }
        virtual ~Server() = default;

        void run();
        void waitForQuit();
};

#endif