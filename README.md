# Multi-threaded-messaging-application (Utilizes Pthreads to send and receive messages in parallel)
I Wanted to learn how to use sockets and as the first step, I came up with this. 
## Server
The server listens on the port 9000 for incoming connections and then connects to them. It then spawns two threads (one receiver, and one 
sender). Both the threads utitilize the communication socket to send and recieve. 

Since both threads utilize the standard I/O, a mutex is utilized to keep them synchronized. There is also a counter to ensure that the server cannot send more than once without allowing the receiver to display the messages received from the client. 

### Note
To test it without a client program, run the following command (linux):


    1. compile the files (gcc main_server.c server.c -lpthread -o server)
    2. Run the server program (./server)
    3. In a different terminal, use telnet (telnet localhost 9000)

To view the all the servers running and their information (state, port, etc.), run the following command


    netstat -atp
    
## Client
The client connects to the server running on port 9000. It also spawns two threads (one receiver, and one 
sender). Both the threads utitilize the communication socket to send and recieve.

### Note


    The Server and client automatically assign the IP address of the system on which they are running since the ai_flags is set to
    AI_PASSIVE. To specify a particular IP address, modify this parameter and set the IP address in getaddrinfo call.
