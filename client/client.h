#ifndef _CLIENT_
#define _CLIENT_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<errno.h>   // For error handling

//For the sockets
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>

//For thread management
#include<pthread.h>

//Defines
#define TRUE 1
#define MAX_BUFFER_SIZE 1024
#define SERV_PORT "9000"

char s[INET6_ADDRSTRLEN];   // For holding IP address 
pthread_mutex_t mutex;
int temp_sock;
struct addrinfo temp_struct;


//Function declarations
void error(const char *msg);
void set_addr_struct(struct addrinfo *hints);
void connector(struct addrinfo *servinfo, struct addrinfo *p);
void *get_in_addr(struct sockaddr *sa);
void *sender_func(void *sender);
void *receiver_func(void *receiver);

#endif
