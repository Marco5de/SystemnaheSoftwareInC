#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define stdin 0
#define stdout 1
#define stderr 2


//forward declaration
int itoa(int d, char* out);

//todo usage with formatting
//todo currently not checking if write is really writing all the content its been given
int main(int argc, const char **argv){

	struct stat file_stat;
	int fd = 0;
	void *mem;

	if(argc!=4){
		char *out = "Usage: ./tr [replace][replace with] [File]\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}	

	if((fd = open(argv[3],O_RDWR)) < 0){
		char *out = "Error: opening file failed\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}
	if(fstat(fd,&file_stat) == -1){
		char *out = "Error: occured calling fstat\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}

	char *out = "~~~opend file with size: ";
	write(stdout,out,strlen(out));
	char buf[1024];
	itoa((int)file_stat.st_size,buf);
	write(stdout,buf,strlen(buf));
	write(stdout,"~~~\n",5);

	mem = mmap(0,file_stat.st_size,PROT_READ,MAP_SHARED,fd,0);
	if(mem == MAP_FAILED){
		char *out = "Err: mapping to memory did not work\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}	
	char *map_opend = "~~~Map successfully opend~~~\n";
	write(stdout,map_opend,strlen(map_opend));
	write(stdout,mem,file_stat.st_size);


	close(fd);
	char *exit = "~~~Closed all connection to files. exiting main now~~~\n";
	write(stdout,exit,strlen(exit));
	return 0;	

}

//converting a usigned int to ascii string
//todo error handling & return value
int itoa(int d, char *out){
	char *ptr = out;
	do{
		*ptr++ = '0' + d%10;
		d/=10;
	}while(d);

	while(out<ptr--){
		char tmp = *out;
		*out++ = *ptr;
		*ptr=tmp;

	}
	return 0;
}




