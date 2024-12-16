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
#define GET "./gettftp"
#define PUT "./puttftp"

void PrintPrompt(const char *prompt,int size){
	write(STDOUT_FILENO,prompt,size); //Write in the shell the right prompt
	}

int ConnectToServer(const char *domain, const char *port, int *sockfd){
	struct addrinfo hints;
	struct addrinfo *res;
    struct addrinfo *rp;
	
	size_t sizeHint=sizeof(hints);
	
	memset(&hints, ZERO, sizeHint);
	
	hints.ai_family = AF_UNSPEC; //Allowing IPv4 and IPv6
	hints.ai_socktype = SOCK_DGRAM; // Only UDP addresses
	
	
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
		
		*sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); //Create a socket
			
		inet_ntop(rp->ai_family, addr, addr_str, rp->ai_addrlen); //Convert the struct into a string
		
		if (connect(*sockfd, rp->ai_addr, rp->ai_addrlen) == 0) { //Trying to connect to the server
            const char *answer="Connected to the server. \n";
			int sizeAnswer=strlen(answer);
		
			PrintPrompt(answer,sizeAnswer);
		}
	 }
	 
	 freeaddrinfo(res);
	 return *sockfd;
}

void gettftp(const char *domain, char file,const char *port){
	int sockfd=-1;
	const char *prompt="Connection to the server...\n";
	int sizePrompt=strlen(prompt);
	
	PrintPrompt(prompt,sizePrompt);
	
	ConnectToServer(domain,port,&sockfd);
	printf("%d \n",sockfd);
	
	const char *downloading="download of the file...\n";
	int sizeDownloading= strlen(downloading);
	
	PrintPrompt(downloading,sizeDownloading);
	
	//Here the code to download the file
	printf("%d \n",file);

	const char *done="file download. \n";
	int sizeDone= strlen(done);
	
	PrintPrompt(done,sizeDone);	
	
	close(sockfd); //Closing of the socket
	const char *closing="Socket closed \n";
	int sizeClosing= strlen(closing);
	
	PrintPrompt(closing,sizeClosing);	
	
	
}

void puttftp(const char *domain, char file, const char *port){
	int sockfd=-1;
	const char *prompt="Connection to the server...\n";
	int sizePrompt=strlen(prompt);
	
	PrintPrompt(prompt,sizePrompt);
	
	ConnectToServer(domain,port,&sockfd);
	
	const char *uploading="upload of the file...\n";
	int sizeUploading= strlen(uploading);
	
	PrintPrompt(uploading,sizeUploading);
	
	//Here the code to upload the file
	printf("%d \n",file);

	const char *done="file upload. \n";
	int sizeDone= strlen(done);
	
	PrintPrompt(done,sizeDone);	
	
	close(sockfd); //Closing of the socket
	const char *closing="Socket closed \n";
	int sizeClosing= strlen(closing);
	
	PrintPrompt(closing,sizeClosing);	
	
	
}

int main(int argc, char ** argv){
	if(argc<3){
		const char *prompt="not enough argument \n";
		int sizePrompt=strlen(prompt);
	
		PrintPrompt(prompt,sizePrompt);
		exit(EXIT_FAILURE);
	}
	const char *port = "69";
	const char *domain = "srvtpinfo1.ensea.fr";
	
	if(strcmp(argv[0],GET)==0){ //If using gettftp
		gettftp(domain, *argv[2],port);
	}else if(strcmp(argv[0],PUT)==0){ //If using puttftp
		puttftp(domain, *argv[2],port);
    }else{
		const char *prompt="Function doesn't exist \n";
		int sizePrompt=strlen(prompt);
	
		PrintPrompt(prompt,sizePrompt);
		exit(EXIT_FAILURE);
	}		
}
