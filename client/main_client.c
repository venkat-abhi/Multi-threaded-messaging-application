#include"client.h"

int main(){
    int sockfd,rv;
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;

    //Clear the struct hints before use
    memset(&hints, 0, sizeof(hints));
    set_addr_struct(&hints);

    //Sets the server information
    status = getaddrinfo(NULL, SERV_PORT, &hints, &servinfo);    
    if(status != 0){
        error("getaddrinfo error: ");
    }

    //Loop through all the results and connect to the first we Can
    connector(servinfo, p);
    sockfd = temp_sock;

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
    printf("client: connecting to %s\n", s);

	
    freeaddrinfo(servinfo);    // Done with this structure

	//Initialize the mutex
	pthread_mutex_init(&mutex, NULL);

	//Create two threads {one for sending; one for receiving}
	int thread_status;
	pthread_t sender, receiver;
	thread_status = pthread_create(&sender, NULL, sender_func, (void *)&sockfd);
	if(thread_status != 0){
		error("Could not create sender_thread: ");
	}
	thread_status = pthread_create(&receiver, NULL, receiver_func, (void *)&sockfd);
	if(thread_status != 0){
		error("Could not create receiver_thread: ");
	}

	//Wait for the threads to complete
	pthread_join(sender, NULL);
	pthread_join(receiver, NULL);
	
	return 0;
		
}
