//example by geeks4geeks: socket programming in c
//client side
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define PORT 8080

int main(int argc, const char **argv){
	struct sockaddr_in address;
	int sock = 0;
	int valread;

	struct sockaddr_in serv_addr;
	char *hello = "Hello from Client\n";
	char buffer[1024] = {0};

	//see serverside implementation for comments on funcionts or the manpage
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr,"Err: fehler beim socket/file descriptor zuweise\n");
		exit(EXIT_FAILURE);
	}

	//füllt mem konstant mit 0 für sizeof bytes
	memset(&serv_addr,'0',sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	//convert IPv4 /IPv6 address from text to binary form
	if(inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr) <= 0){ //using localhost!
		fprintf(stderr,"Err: fehler beim konvertieren der IP addresse in eine binäre form\n");
		exit(EXIT_FAILURE);
	}	
	//connect to server
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		fprintf(stderr,"Err: fehler beim verbinden mit dem Server\n");
		exit(EXIT_FAILURE);
	}

	send(sock, hello, strlen(hello), 0);
	printf("Hallo message sent\n");

	valread = read(sock, buffer, 1024);
	printf("%s\n",buffer);

	return 0;	

}
