#include"server.h"

int main(){
    int communication_sock;
    int status;
    socklen_t client_size;
    struct addrinfo hints;
    struct sockaddr_storage client; // Will hold the clients information
    struct addrinfo *servinfo;      // Will point to the results
    struct addrinfo *p;             // Use it to access the list elements
    struct addrinfo *temp;          // Use it to store the valid element of the struct addrinfo

    memset(&hints,0,sizeof(hints)); // Clearing the struct it is not
    set_addr_struct(&hints);
    
    //Get ready to connect 
    status = getaddrinfo(NULL, PORT, &hints, &servinfo); // Null: use it for host ip address
    if(status != 0){
        error("getaddrinfo error: ");
    }

    //Get server IP and create the socket and bind it to the specified port
    binder(p,servinfo,temp);


    //Listen for incoming connections
    listen(serv_sock, BACKLOG);

    //Accept incoming connection{stores the clients info in sockaddr client}
    client_size = sizeof(client);
    communication_sock = accept(serv_sock, (struct sockaddr *)&client, &client_size);
    if(communication_sock == -1){
        error("Could not connect: ");
    }

    //Get client's IP address
    get_client_ip(&client);
      

    //Notify client of the succesful connection
    char notify[] = "Connection succesful\n";
    send(communication_sock, notify, sizeof(notify), 0);

    //Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    //Create two threads {one for sending; one for receiving}
    int thread_status;
    pthread_t sender,receiver;
    thread_status = pthread_create(&sender, NULL, sender_func, (void *)&communication_sock);
    if(thread_status != 0){
        error("Could not create sender_thread: ");
    }
    thread_status = pthread_create(&receiver,NULL, receiver_func, (void *)&communication_sock);
    if(thread_status != 0){
        error("Could not create receiver_thread: ");
    }

    //Wait for the threads to complete 
    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);
    return 0;
}



