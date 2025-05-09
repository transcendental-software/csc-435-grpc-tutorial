package csc435.app;

import java.io.IOException;
import java.util.Scanner;

import io.grpc.Grpc;
import io.grpc.InsecureServerCredentials;
import io.grpc.ServerBuilder;

public class Server implements Runnable {
    private Integer port;
    private io.grpc.Server server;

    public Server(String port) {
        this.port = Integer.parseInt(port);
    }

    @Override
    public void run() {
        ServerBuilder<?> serverBuilder = Grpc.newServerBuilderForPort(port, InsecureServerCredentials.create());
        server = serverBuilder.addService(new FileRetrievalEngineService()).build();
        
        try {
            server.start();
        } catch (IOException e) {
            System.err.println("Failed to start server!");
            return;
        }

        try {
            server.awaitTermination();
        } catch (InterruptedException e) { }
    }

    public void shutdown() {
        if (server != null) {
            server.shutdownNow();
        }
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("USE: java Server <port>");
            System.exit(1);
        }

        Server server = new Server(args[0]);
        Thread serverThread = new Thread(server);
        serverThread.start();
        
        Scanner sc = new Scanner(System.in);
        String command;

        while (true) {
            System.out.println("gRPC Server started!");
            System.out.print("> ");
            
            command = sc.nextLine();
            
            if (command.compareTo("quit") == 0) {
                server.shutdown();
                try {
                    serverThread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("gRPC Server terminated!");
                break;
            }
        }

        sc.close();
    }
}