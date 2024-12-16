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

void gettftp(const char *domain, char file){
	const char *prompt="Connection to the server...\n";
	int sizePrompt=strlen(prompt);
	
	PrintPrompt(prompt,sizePrompt);
	
	//Here the code to connect to the server
	printf("%s \n",domain);
	
	const char *downloading="download of the file...\n";
	int sizeDownloading= strlen(downloading);
	
	PrintPrompt(downloading,sizeDownloading);
	
	//Here the code to download the file
	printf("%d \n",file);

	const char *done="file download. \n";
	int sizeDone= strlen(done);
	
	PrintPrompt(done,sizeDone);	
	
	
}

void puttftp(const char *domain, char file){
	const char *prompt="Connection to the server...\n";
	int sizePrompt=strlen(prompt);
	
	PrintPrompt(prompt,sizePrompt);
	
	//Here the code to connect to the server
	printf("%s \n",domain);
	
	const char *downloading="upload of the file...\n";
	int sizeDownloading= strlen(downloading);
	
	PrintPrompt(downloading,sizeDownloading);
	
	//Here the code to download the file
	printf("%d \n",file);

	const char *done="file upload. \n";
	int sizeDone= strlen(done);
	
	PrintPrompt(done,sizeDone);	
	
	
}

int main(int argc, char ** argv){
	if(argc<3){
		const char *prompt="not enough argument \n";
		int sizePrompt=strlen(prompt);
	
		PrintPrompt(prompt,sizePrompt);
		exit(EXIT_FAILURE);
	}
	const char *domain = "srvtpinfo1.ensea.fr";
	char addr_str[INET6_ADDRSTRLEN];
	
	getaddr(domain, addr_str); //recovering of the IP address of the DNS in addr_str
    printf("The IP address is %s \n", addr_str);
    

	if(strcmp(argv[0],GET)==0){ //If using gettftp
		gettftp(domain, *argv[2]);
	}else if(strcmp(argv[0],PUT)==0){ //If using puttftp
		puttftp(domain, *argv[2]);
    }else{
		const char *prompt="Function doesn't exist \n";
		int sizePrompt=strlen(prompt);
	
		PrintPrompt(prompt,sizePrompt);
		exit(EXIT_FAILURE);
	}		
}
