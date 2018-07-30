#include"server.h"

//Global variables
int yes = 1;
pthread_mutex_t mutex;
int lock_limit = 0;
char client_ip[INET6_ADDRSTRLEN];
char *cli_ipver;
char ipstr[INET6_ADDRSTRLEN];   // Use it to store ip for printing
char cli_ip[INET6_ADDRSTRLEN];

//Function definitions

void error(const char *msg){
    perror(msg);
    exit(1);
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void set_addr_struct(struct addrinfo *hints){
    hints->ai_family = AF_UNSPEC;    // Can be IPv4 or IPv6
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;    // Tells getaddrinfo() to assign the address of the local host to the socket structure
                                    // Do not set to passive if connecting to a specific ip. Set ip in getaddrinfo
}

void binder(struct addrinfo *p, struct addrinfo *servinfo, struct addrinfo *temp){
    for(p = servinfo; p != NULL; p = p->ai_next){   // Loop through the list using p {ai_next points to next element}
        if ((serv_sock = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            error("server: socket");
            continue;
        }

        if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            error("setsockopt");
        }

        if (bind(serv_sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(serv_sock);
            error("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

}

void get_client_ip(struct sockaddr_storage *client){
    inet_ntop(client->ss_family,get_in_addr((struct sockaddr *)client),cli_ip, sizeof(cli_ip));
}

void *sender_func(void *sender){
    char sending_buffer[MAX_BUFFER_SIZE];
    while(TRUE){
        if(lock_limit<1){   // Checks if server has sent more than once already before showing client's message {allows for client message to printed}
            lock_limit+=1;
            pthread_mutex_lock(&mutex);
            printf("Server: ");
            fgets(sending_buffer, MAX_BUFFER_SIZE, stdin);
            
            //Send the message
            int len, bytes_sent;
            len = strlen(sending_buffer);
            bytes_sent = send(*(int *)sender, sending_buffer, len, 0);
            if(bytes_sent < len){
                int new_len = (len - bytes_sent);
                bytes_sent = send(*(int *)sender, (sending_buffer+bytes_sent), new_len, 0);
            }

            //Release the lock
            pthread_mutex_unlock(&mutex);

            //Clear the buffer
            memset(sending_buffer, 0, strlen(sending_buffer));
            }
    }
    return 0;
}

void *receiver_func(void *receiver){
    char receiving_buffer[MAX_BUFFER_SIZE];
    while(TRUE){
        int bytes_received;
        bytes_received = recv(*(int *)receiver, receiving_buffer, MAX_BUFFER_SIZE, 0);
        if(bytes_received == -1){
            if(lock_limit != 0){    // Checks if the server has already sent a message but has not received any message yet.
                lock_limit = 0;     // This allows the server to send more messages if there no messages to view. 
            }
            error("Could not receive message: ");
        }
        else{
            pthread_mutex_lock(&mutex);
            printf("Message from client [%s]: %s\n",cli_ip,receiving_buffer);

            //Release the lock
            pthread_mutex_unlock(&mutex);

            //Check if client wants to exit
            if(strcmp(receiving_buffer,"exit") == 0){
                printf("Client wants to exit\n.");
                printf("Exiting...");
                return 0;
            }            
            //Clear the buffer
            memset(receiving_buffer, 0, strlen(receiving_buffer));
            
            lock_limit=0;   // Resets server sent count
        }
    }
    return 0;
}
