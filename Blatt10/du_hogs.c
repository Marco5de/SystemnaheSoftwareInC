#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define COUNT 10

typedef struct file{
	char *path;
	off_t size;
	struct file *next;
}file;

file list[COUNT] = {0}; 

int main(int argc, char *argv[]){
	struct stat attribut;
	DIR *dir;
	struct dirent *dirzeiger;

	if(argc != 2){
		printf("Error, wrong number of arguments\n\n");
		exit(1);
	}

	if((dir=opendir(argv[1]))==NULL){
		printf("Error opening dir\n\n");
		exit(1);
	}
	printf("Succesfully opend dir \n\n");

	while((dirzeiger = readdir(dir)) != NULL){
		if(lstat((*dirzeiger).d_name, &attribut) == -1){
			printf("Error in lstat function \n\n");
			exit(1);
		}
		
		char *timestr = ctime(&attribut.st_mtime);


		if(attribut.st_mode & S_IFREG){
			off_t size = attribut.st_size;
			printf("Regular file: %s Size %jdBytes Last changed: %s",(*dirzeiger).d_name,size,timestr);
		}else if(attribut.st_mode & S_IFDIR)
			printf("Dir: %s Last changed:%s ",(*dirzeiger).d_name,timestr);
		else if(attribut.st_mode & S_IFCHR)
			printf("Devicefile: %s Last changed: %s",(*dirzeiger).d_name,timestr);
		else
			printf("Unknown file\n");
	}
	if(closedir(dir) == -1)
		printf("Error closing dir\n\n");
	


	return 0;

}
