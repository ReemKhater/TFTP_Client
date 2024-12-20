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
