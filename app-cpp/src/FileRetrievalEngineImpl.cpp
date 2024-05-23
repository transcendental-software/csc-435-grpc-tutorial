#include "FileRetrievalEngineImpl.hpp"

grpc::Status FileRetrievalEngineImpl::IndexFile(
        grpc::ServerContext* context,
        const fre::IndexReq* request,
        fre::IndexRep* reply)
{
    std::cout << std::endl;
    std::cout << "indexing " << request->document_path() << " from client " << request->client_id() << std::endl;
    
    for (auto iter = request->word_frequencies().begin(); iter != request->word_frequencies().end(); ++iter) {
        std::cout << iter->first << " " << iter->second << std::endl;
    }

    std::cout << "completed!" << std::endl;
    std::cout << "> ";

    reply->set_ack("OK");
    return grpc::Status::OK;
}

grpc::Status FileRetrievalEngineImpl::SearchFiles(
        grpc::ServerContext* context,
        const fre::SearchReq* request,
        fre::SearchRep* reply) 
{
    std::cout << std::endl;
    std::cout << "searching for " << request->term() << std::endl;
    std::cout << "> " << std::flush;

    (*reply->mutable_document_frequencies())["DOC1"] = 10;
    (*reply->mutable_document_frequencies())["DOC10"] = 20;
    (*reply->mutable_document_frequencies())["DOC100"] = 30;

    return grpc::Status::OK;
}