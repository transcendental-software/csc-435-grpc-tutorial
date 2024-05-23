package csc435.app;

import java.io.IOException;
import java.util.Scanner;

import io.grpc.Grpc;
import io.grpc.InsecureServerCredentials;
import io.grpc.ServerBuilder;

public class Server {
    private Integer port;
    private io.grpc.Server server;

    public Server(String port) {
        this.port = Integer.parseInt(port);
    }

    public void run() {
        ServerBuilder<?> serverBuilder = Grpc.newServerBuilderForPort(port, InsecureServerCredentials.create());
        server = serverBuilder.addService(new FileRetrievalEngineService()).build();
        
        try {
            server.start();
        } catch (IOException e) {
            return;
        }
        
        Thread thread = new Thread(new WaitForQuit());
        thread.start();

        try {
            server.awaitTermination();
        } catch (InterruptedException e) { }
    }

    private class WaitForQuit implements Runnable {

        @Override
        public void run() {
            Scanner sc = new Scanner(System.in);

            String command;

            while (true) {
                System.out.print("> ");
                
                command = sc.nextLine();
                
                if (command.compareTo("quit") == 0) {
                    server.shutdownNow();
                    System.out.println("Server terminated!");
                    break;
                }
            }

            sc.close();
        }
        
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("USE: java Server <port>");
            System.exit(1);
        }

        Server server = new Server(args[0]);
        server.run();
    }
}