#include <iostream>
#include <memory>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "proto/file_retrieval_engine.grpc.pb.h"

class Client
{
    int clientID;
    std::string address;
    std::string port;

    public:
        Client(int clientID, std::string address, std::string port) : 
            clientID(clientID), address(address), port(port) { }
        virtual ~Client() = default;

        virtual void run()
        {
            std::shared_ptr<grpc::Channel> channel;
            std::unique_ptr<fre::FileRetrievalEngine::Stub> stub;
            std::unique_ptr<grpc::ClientContext> context;
            grpc::Status status;
            fre::IndexReq indexRequest;
            fre::IndexRep indexReply;
            fre::SearchReq searchRequest;
            fre::SearchRep searchReply;

            channel = grpc::CreateChannel(address + ":" + port, grpc::InsecureChannelCredentials());
            stub = fre::FileRetrievalEngine::NewStub(channel);

            // run indexing by accessing the IndexFile service
            context = std::make_unique<grpc::ClientContext>();
            indexRequest.set_client_id(clientID);
            indexRequest.set_document_path("DOC11");
            auto temporary_index = indexRequest.mutable_word_frequencies();
            (*temporary_index)["cat"] = 10;
            (*temporary_index)["dog"] = 20;
            (*temporary_index)["tiger"] = 100;
            status = stub->IndexFile(context.get(), indexRequest, &indexReply);
            if (status.ok()) {
                std::cout << "Indexing " << indexReply.ack() << std::endl;
            }
            
            // run search by accessing the SearchFiles service
            context = std::make_unique<grpc::ClientContext>();
            searchRequest.set_term("cat");
            status = stub->SearchFiles(context.get(), searchRequest, &searchReply);
            if (status.ok()) {
                auto results = searchReply.document_frequencies();
                std::cout << "Searching for " << searchRequest.term() << std::endl;
                for (auto iter = results.begin(); iter != results.end(); ++iter) {
                    std::cout << iter->first << " " << iter->second << std::endl;
                }
            }
        }
};

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "USE: ./client <client ID> <IP address> <port>" << std::endl;
        return 1;
    }

    std::string clientID(argv[1]);
    std::string address(argv[2]);
    std::string port(argv[3]);

    Client client(std::stoi(clientID), address, port);
    client.run();

    return 0;
}