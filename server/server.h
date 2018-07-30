#ifndef _SERVER_
#define _SERVER_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<errno.h>       // For error()

//For the sockets
#include<unistd.h>      // close() system call
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>   // inet_ntop() call

//For the threads
#include<pthread.h>

//Defines
#define TRUE 1
#define BACKLOG 5
#define MAX_BUFFER_SIZE 1024
#define PORT "9000"

pthread_mutex_t mutex;
int serv_sock;


//Declarations
void *sender_func(void *);
void *receiver_func(void *);
void error(const char *msg);
void set_addr_struct(struct addrinfo *hints);
void binder(struct addrinfo *p, struct addrinfo *servinfo, struct addrinfo *temp);
void *get_in_addr(struct sockaddr *sa);
void get_client_ip(struct sockaddr_storage *client);

#endif
