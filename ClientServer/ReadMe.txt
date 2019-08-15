Asylbek Dzhapashov
CS470
Lab3
02/17/2019

C++ program under Linux OS to Simulate an airfare selling system, where the server is in charge to keep the 
administration of the tickets, while the clients can connect to the server and ask to purchase the available tickets..

To run this program 
    type: 
        // to compile server.cpp file
        1. g++ server.cpp -o server -lpthread
        // to run server
        2. ./server <port> 
        // to compile client.cpp file
        3. g++ client.cpp -o client
        // to run client 
        4. ./client <config.ini>

    * <config.ini> includes IP address, Port, TimeOut respectively
    * program accepts multiple clients
    * reservation is updated on the server