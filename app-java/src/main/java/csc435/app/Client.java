package csc435.app;

import java.util.HashMap;
import java.util.Map;

import csc435.app.FileRetrievalEngineGrpc.FileRetrievalEngineBlockingStub;

import io.grpc.Grpc;
import io.grpc.InsecureChannelCredentials;
import io.grpc.ManagedChannel;

public class Client {
    private int clientID;
    private String address;
    private String port;

    public Client(int clientID, String address, String port) {
        this.clientID = clientID;
        this.address = address;
        this.port = port;
    }

    public void run() {
        String target = address + ":" + port;
        ManagedChannel channel = Grpc.newChannelBuilder(target, InsecureChannelCredentials.create()).build();
        FileRetrievalEngineBlockingStub stub = FileRetrievalEngineGrpc.newBlockingStub(channel);
        IndexReq indexRequest;
        IndexRep indexReply;
        SearchReq searchRequest;
        SearchRep searchReply;

        // run indexing by accessing the IndexFile service
        Map<String, Long> temporary_index = new HashMap<>();
        temporary_index.put("cat", 10l);
        temporary_index.put("dog", 20l);
        temporary_index.put("tiger", 100l);

        indexRequest = IndexReq.newBuilder()
            .setClientId(clientID)
            .setDocumentPath("DOC11")
            .putAllWordFrequencies(temporary_index)
            .build();
        indexReply = stub.indexFile(indexRequest);
        System.out.println("Indexing " + indexReply.getAck());

        // run search by accessing the SearchFiles service
        searchRequest = SearchReq.newBuilder().setTerm("cat").build();
        searchReply = stub.searchFiles(searchRequest);
        System.out.println("Searching for " + searchRequest.getTerm());
        for (Map.Entry<String, Long> entry : searchReply.getDocumentFrequenciesMap().entrySet()) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
    }

    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("USE: java Client <client ID> <IP address> <port>");
            System.exit(1);
        }

        Client client = new Client(Integer.parseInt(args[0]), args[1], args[2]);
        client.run();
    }
}