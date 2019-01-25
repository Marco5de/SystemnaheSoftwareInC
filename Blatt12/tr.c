#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define stdin 0
#define stdout 1
#define stderr 2

struct swititching_pair{
	char current;
	char new;
	struct swititching_pair *next;
};	

//forward declaration
int itoa(int d, char* out);

//todo usage with formatting
//todo currently not checking if write is really writing all the content its been given
//todo alle spezifikationen die im pdf zu commandline args gegegebn sind erfüllen!
//prüfen dass nur gültige char eingegeben werden
int main(int argc, const char **argv){

	struct stat file_stat;
	int fd = 0;
	void *mem;
	struct swititching_pair *list;

	if(argc!=4){
		char *out = "Usage: ./tr [replace][replace with] [File]\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}	
	
	if(strlen(argv[1]) != strlen(argv[2])){
		char *out = "Error: replacement commands have to be the same length!\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}

	list = calloc(1,sizeof(struct swititching_pair));
	if(!list){char *err="Err: allocating memory\n";write(2,err,strlen(err));}
	list->current = *argv[1];
	list->new = *argv[2];
	list->next = NULL;

	struct swititching_pair *tmp = list;
	for(int i=1; i<strlen(argv[1]);i++){
		tmp->next = calloc(1,sizeof(struct swititching_pair));
		if(!tmp){char *err="Err: allocating memory\n";write(2,err,strlen(err));}
		tmp->current = *(argv[1]+i);
	   	tmp->new = *(argv[2]+i);
		tmp->next->next = NULL;
		tmp = tmp->next;	
	}
	char *list_init = "~~~Init der liste ist beendet~~~\n";
	write(stdout,list_init,strlen(list_init));

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




