package csc435.app;

import java.util.HashMap;
import java.util.Map;

import io.grpc.stub.StreamObserver;

public class FileRetrievalEngineService extends FileRetrievalEngineGrpc.FileRetrievalEngineImplBase {
    public FileRetrievalEngineService() { }

    @Override
    public void indexFile(IndexReq request, StreamObserver<IndexRep> responseObserver) {
        responseObserver.onNext(doIndexFile(request));
        responseObserver.onCompleted();
    }

    @Override
    public void searchFiles(SearchReq request, StreamObserver<SearchRep> responseObserver) {
        responseObserver.onNext(doSearchFiles(request));
        responseObserver.onCompleted();
    }

    private IndexRep doIndexFile(IndexReq request) {
        System.out.println("");
        System.out.println("indexing " + request.getDocumentPath() + " from client " + request.getClientId());
        
        for (Map.Entry<String, Long> entry : request.getWordFrequenciesMap().entrySet()) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
        
        System.out.println("completed!");
        System.out.print("> ");

        return IndexRep.newBuilder().setAck("OK").build();
    }

    private SearchRep doSearchFiles(SearchReq request) {
        System.out.println("");
        System.out.println("searching for " + request.getTerm());
        System.out.print("> ");

        Map<String, Long> results = new HashMap<>();
        results.put("DOC1", 10l);
        results.put("DOC10", 20l);
        results.put("DOC100", 30l);

        return SearchRep.newBuilder().putAllDocumentFrequencies(results).build();
    }

}
