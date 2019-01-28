#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define stdin 0
#define stdout 1
#define stderr 2

#define DEBUG

struct swititching_pair{
	char current;
	char new;
	struct swititching_pair *next;
};	

//forward declaration
int itoa(int d, char* out);
int replacing_chars(struct swititching_pair *list, void *mem, size_t size,unsigned int rsize);

//todo usage with formatting
//todo currently not checking if write is really writing all the content its been given
//todo alle spezifikationen die im pdf zu commandline args gegegebn sind erfüllen!
//prüfen dass nur gültige char eingegeben werden
//todo fix hässliche functin zum ersetzen
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
		tmp->next->current = *(argv[1]+i);
	   	tmp->next->new = *(argv[2]+i);
		tmp->next->next = NULL;
		tmp = tmp->next;	
	}
#ifdef DEBUG
	char *list_init = "~~~Init der liste ist beendet~~~\n";
	write(stdout,list_init,strlen(list_init));
#endif
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

	mem = mmap(0,file_stat.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(mem == MAP_FAILED){
		char *out = "Err: mapping to memory did not work\n";
		write(stderr,out,strlen(out));
		exit(EXIT_FAILURE);
	}	
#ifdef DEBUG
	char *map_opend = "~~~Map successfully opend~~~\n";
	write(stdout,map_opend,strlen(map_opend));
	write(stdout,mem,file_stat.st_size);
#endif
		
	if(replacing_chars(list,mem,(size_t)file_stat.st_size,strlen(argv[1]))){
		char *error = "~~~Error in replacing_chars function. Exiting main~~~\n";
		write(stdout,error,strlen(error));
	}


	close(fd);
#ifdef DEBUG
	char *exit = "~~~Closed all connection to files. exiting main now~~~\n";
	write(stdout,exit,strlen(exit));
#endif
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

int replacing_chars(struct swititching_pair *list, void *mem,size_t size,unsigned int rsize){
	char *curr_ptr = (char*)mem;
	
	struct swititching_pair *tmp;

	for(int i=0; i<size; i++){
		tmp = list;
		int j = 0;
		while(j++ < rsize){
			if(curr_ptr[i] == tmp->current){
#ifdef DEBUG
				char *replaced_char = "~~~Succesfully replaced_char ~~~ \n";
				write(stdout, replaced_char, strlen(replaced_char));
#endif 
				curr_ptr[i] = tmp->new;	
			}
			tmp=tmp->next; 
		}
	}
	return 0;
}
