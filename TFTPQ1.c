#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 69
#define ZERO 0

char getaddr(const char *domain, char *addr_str){
	struct addrinfo hints;
	struct addrinfo *res;
    struct addrinfo *rp;
	
	size_t sizeHint=sizeof(hints);
	
	memset(&hints, ZERO, sizeHint);
	
	hints.ai_family = AF_UNSPEC; //Allowing IPv4 and IPv6
	
	
	int ecode= getaddrinfo(domain,NULL,&hints,&res);

	if(ecode!=0){
		printf("Error: %s \n",gai_strerror(ecode));
		exit(EXIT_FAILURE);
	}
	
	void *addr;
	
	for (rp = res; rp != NULL; rp = rp->ai_next) {
		struct sockaddr_in *new_addr=(struct sockaddr_in *)rp->ai_addr;
		addr=&(new_addr->sin_addr);
			
		inet_ntop(rp->ai_family, addr, addr_str, rp->ai_addrlen); //Convert the struct into a string
		//printf("IP address is %s \n",addr_str);
	 }
	 freeaddrinfo(res);
	 return *addr_str;
}


int main(){
	const char *domain = "srvtpinfo1.ensea.fr";
    char addr_str[INET6_ADDRSTRLEN];

    getaddr(domain, addr_str); //recovering of the IP address of the DNS in addr_str
    printf("The IP address is %s \n", addr_str);
    
    
}
