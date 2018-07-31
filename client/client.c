#include"client.h"

int lock_limit = 0;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void set_addr_struct(struct addrinfo *hints){
    hints->ai_family = AF_UNSPEC;    // Can be IPv4 or IPv6
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;    // Tells getaddrinfo() to assign the address of the local host to the socket structure
                                    // Do not set to passive if connecting to a specific ip. Set ip in getaddrinfo
}

void connector(struct addrinfo *servinfo, struct addrinfo *p){
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((temp_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            printf("client: socket");
            continue;
        }

        if (connect(temp_sock, p->ai_addr, p->ai_addrlen) == -1) {
            //close(temp_sock);
            printf("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        printf("client: failed to connect\n");
    }
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *sender_func(void *sender){
	char sending_buffer[MAX_BUFFER_SIZE];
	while(TRUE){
        if(lock_limit<1){   // Checks if client has sent more than once already before showing client's message {allows for client message to printed}
            lock_limit+=1;
            pthread_mutex_lock(&mutex);
            printf("Client: ");
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
        int bytes_received = 0;
        bytes_received = recv(*(int *)receiver, receiving_buffer, MAX_BUFFER_SIZE, 0);
        if(bytes_received == -1){
            if(lock_limit != 0){    // Checks if the client has already sent a message but has not received any message yet.
                lock_limit = 0;     // This allows the client to send more messages if there no messages to view. 
            }
            printf("Could not receive message: ");
        }
        else{
            pthread_mutex_lock(&mutex);
            printf("Message from server: %s\n",receiving_buffer);

            //Release the lock
            pthread_mutex_unlock(&mutex);
           
            //Clear the buffer
            memset(receiving_buffer, 0, strlen(receiving_buffer));
            
            lock_limit=0;   // Resets client sent count
        }
    }
    return 0;
}
