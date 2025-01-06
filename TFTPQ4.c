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
#define SIZE_OF_PACKET 516 // 512 + the length of opcode

void PrintPrompt(const char *prompt,int size){
	write(STDOUT_FILENO,prompt,size); //Write in the shell the right prompt
	}
	
int ConvertToASCII(const char *file, unsigned char *ASCIIfile){
    for(unsigned int i = 0; i < strlen(file); ++i)
    {
        ASCIIfile[i]=(unsigned char) file[i];
    }
    return *ASCIIfile;
}

unsigned int ConfigureRRQPacket(const char *file, const char *mode, unsigned char *rrq_packet) {
    unsigned char ASCIIfile[strlen(file)];
    
    //Initialization of the first two bits of rrq
	rrq_packet[0]=ZERO;
	rrq_packet[1]=1;
	
	ConvertToASCII(file,ASCIIfile); //Convert file in ASCII 
	
	unsigned int lengthOfRRQ=2;
	for(unsigned int i=0; i<strlen(file);i++){ //Add the n bytes of the filename
		rrq_packet[lengthOfRRQ++]=ASCIIfile[i];
	}
	
	rrq_packet[lengthOfRRQ++]=ZERO; //byte seperator between the filename and the mode
	

    for (unsigned int i = 0; i < strlen(mode); i++) { //Add the mode "octet" to the packet
        rrq_packet[lengthOfRRQ++] = (unsigned char)mode[i];
    }
    
    rrq_packet[lengthOfRRQ++] = ZERO; // Byte separator to terminate the mode
    
    return lengthOfRRQ;
    
}

void SendRRQ(int sockfd, unsigned char *rrq_packet, int lengthOfRRQ){
	//Send of RRQ
	 if (send(sockfd, rrq_packet, lengthOfRRQ, 0) == -1) { 
        perror("RRQ packet not understand");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void RecvDATA(int sockfd, unsigned char *data_packet, int byte_recv, unsigned char *ack_packet){
	struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr); // Length of the address structure
    
	do {
        byte_recv = recvfrom(sockfd, data_packet, SIZE_OF_PACKET, 0, (struct sockaddr *)&sender_addr, &addr_len);
        if (byte_recv < 0) {
            perror("Error in receiving a byte");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
		
		if (data_packet[0] !=0 || data_packet[1] != 3){
			perror("DATA packet not received \n");
			close(sockfd);
			exit(EXIT_FAILURE);
		}

		//Configure ACK
		ack_packet[0]=ZERO;
		ack_packet[1]=4;
		ack_packet[2]=data_packet[2];
		ack_packet[3]=data_packet[3];
		
	
		//Send ACK
		if (send(sockfd, ack_packet, sizeof(ack_packet), 0) == -1) {
			perror("Failed to send ACK");
			close(sockfd);
			exit(EXIT_FAILURE);
		}
    
	}while(byte_recv == SIZE_OF_PACKET); //4 is the length of the opcode
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
		if (*sockfd == -1) {
			perror("Socket creation failed");
			close(*sockfd);
			exit(EXIT_FAILURE);
		}
			
		inet_ntop(rp->ai_family, addr, addr_str, rp->ai_addrlen); //Convert the struct into a string
		
		if(connect(*sockfd, rp->ai_addr, rp->ai_addrlen) == 0) { //Trying to connect to the server
            const char *answer="Connected to the server. \n";
			int sizeAnswer=strlen(answer);
		
			PrintPrompt(answer,sizeAnswer);
		}
	 }
	 
	 freeaddrinfo(res);
	 return *sockfd;
}

void gettftp(const char *domain, const char *file,const char *port){
	int sockfd=-1;
	const char *prompt="Connection to the server...\n";
	int sizePrompt=strlen(prompt);
	unsigned char rrq_packet[SIZE_OF_PACKET];
	unsigned char ack_packet[4];
	unsigned char data_packet[SIZE_OF_PACKET];
	int byte_recv=0; 
	const char *mode="octet";
	
	
	PrintPrompt(prompt,sizePrompt);
	
	unsigned int lengthOfRRQ= ConfigureRRQPacket(file, mode,rrq_packet);
	

    for (unsigned int i = 0; i < lengthOfRRQ; i++) {
        printf("%02x ", rrq_packet[i]);
    }
    printf("\n");
    
    ConnectToServer(domain,port,&sockfd); //Connection to the server
    
    const char *downloading="download of the file...\n";
	int sizeDownloading= strlen(downloading);
	
	PrintPrompt(downloading,sizeDownloading);
    

	SendRRQ(sockfd, rrq_packet, lengthOfRRQ);
    
    RecvDATA(sockfd, data_packet, byte_recv, ack_packet);
    

	const char *done="file download. \n";
	int sizeDone= strlen(done);
	
	PrintPrompt(done,sizeDone);	
	
	close(sockfd); //Closing of the socket
	const char *closing="Socket closed. \n";
	int sizeClosing= strlen(closing);
	
	PrintPrompt(closing,sizeClosing);	
	
	
}

void puttftp(const char *domain, const char *file, const char *port){
	int sockfd=-1;
	const char *prompt="Connection to the server...\n";
	int sizePrompt=strlen(prompt);
	
	PrintPrompt(prompt,sizePrompt);
	
	ConnectToServer(domain,port,&sockfd);
	
	const char *uploading="upload of the file...\n";
	int sizeUploading= strlen(uploading);
	
	PrintPrompt(uploading,sizeUploading);
	
	//Here the code to upload the file
	printf("%s \n",file);

	const char *done="file upload. \n";
	int sizeDone= strlen(done);
	
	PrintPrompt(done,sizeDone);	
	
	close(sockfd); //Closing of the socket
	const char *closing="Socket closed \n";
	int sizeClosing= strlen(closing);
	
	PrintPrompt(closing,sizeClosing);	
	
	
}

int main(int argc, const char ** argv){
	if(argc<3){
		const char *prompt="not enough argument \n";
		int sizePrompt=strlen(prompt);
	
		PrintPrompt(prompt,sizePrompt);
		exit(EXIT_FAILURE);
	}
	const char *port = "69";
	
	if(strcmp(argv[0],GET)==0){ //If using gettftp
		gettftp(argv[1], argv[2],port);
	}else if(strcmp(argv[0],PUT)==0){ //If using puttftp
		puttftp(argv[1], argv[2],port);
    }else{
		const char *prompt="Function doesn't exist \n";
		int sizePrompt=strlen(prompt);
	
		PrintPrompt(prompt,sizePrompt);
		exit(EXIT_FAILURE);
	}		
}
