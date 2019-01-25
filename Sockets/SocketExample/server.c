//using a example from geeks4geeks
//server side implementation
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const **argv){

	int server_fd, new_socket, valread;
	struct sockaddr_in address;

	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hallo from Server\n";

	//creating socket file descriptor (socket desciptor funktioniert im grunde genauso wie eine normale Datei --> gute Abstraktion)
	//gibt filedescoptor zurück, AF_INET für IPv4, SOCK_STREAM für TCP und 0 für Protocol number im IP
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		fprintf(stderr,"Socket: failed to open socket\n");
		exit(EXIT_FAILURE);
	}

	//attach socket to port (ToDo was machen die einzelnen Parameter?!)
	if(setsockopt(server_fd,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		fprintf(stderr, "Err: attaching socket to port\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //localhost
	address.sin_port = htons( PORT );

	//attaching socket to port (8080)
	if(bind(server_fd,(struct sockaddr *) &address, sizeof(address))<0){
		fprintf(stderr,"Err: attaching socket to port 2\n");
		exit(EXIT_FAILURE);
	}

	//put server in listen mode
	//3 is maximum queue of pending location (see manpage listen))
	if(listen(server_fd,3) < 0){
		fprintf(stderr,"Err: putting socket in listen mode \n");
		exit(EXIT_FAILURE);
	}
	//client connected to Server, give connection a Socket (just as you would do it java)
	if((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen)) < 0){
		fprintf(stderr,"Err: accepting client: creating new socket descriptor\n");
		exit(EXIT_FAILURE);
	}

	//read val from socket
	//see syscall! should be buffered to be efficient!
	//check also the manpages of read and send! 
	valread = read(new_socket,buffer,1024);
	printf("result from syscall read %d\n",valread);
	printf("%s\n",buffer);

	send(new_socket,hello,strlen(hello),0);
	printf("Hello message sent\n");

	return 0;	

}
