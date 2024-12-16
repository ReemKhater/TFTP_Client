#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define ZERO 0

void ConnectToServer(const char *domain, const char *port){
	struct addrinfo hints;
	struct addrinfo *res;
    struct addrinfo *rp;
    int sockfd;
	
	size_t sizeHint=sizeof(hints);
	
	memset(&hints, ZERO, sizeHint);
	
	hints.ai_family = AF_UNSPEC; //Allowing IPv4 and IPv6
	
	
	int ecode= getaddrinfo(domain,port,&hints,&res);

	if(ecode!=0){
		printf("Error: %s \n",gai_strerror(ecode));
		exit(EXIT_FAILURE);
	}
	
	void *addr;
	char addr_str[INET6_ADDRSTRLEN];
	
	for (rp = res; rp != NULL; rp = rp->ai_next) {
		struct sockaddr_in *new_addr=(struct sockaddr_in *)rp->ai_addr;
		addr=&(new_addr->sin_addr);
		
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); //Create a socket
			
		inet_ntop(rp->ai_family, addr, addr_str, rp->ai_addrlen); //Convert the struct into a string
		
		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) { //Trying to connect to the server
            printf("Connected to the server. \n");
		}
	 }
	 
	 freeaddrinfo(res);
}


int main(){
	const char *domain = "srvtpinfo1.ensea.fr";
	const char *port = "69";

    ConnectToServer(domain, port); //Connection to the server
    
    
}
