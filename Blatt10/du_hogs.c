#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>
#include <errno.h>

#define COUNT 10
//#define DEBUG

//Todo format output
//todo commandline arguments
//todo free memory when done using

typedef struct file{
	char *path;
	off_t size;
}file;

//forward declarations
void compareSize(off_t size, char *path, char *fname);
int cmpfunc(const void *a, const void *b);
void debug_printList();
char* assemblePathstring(char *path, char *fname);
char* stringConcat(const char* a, const char* b);

file list[COUNT] = {0}; 

void du_hog_readdir(char *path){
	//declaring needed struct to get any information about directorys or files
	struct stat attribut;
	DIR *dir;
	struct dirent *dirptr;
	//tries to open directory. Exit if not succesfull
	if((dir=opendir(path))==NULL){
		printf("Error opening directory \n\n");
		exit(1);
	}	
	printf("Succesfully opend dir: %s\n",path);

	while((dirptr = readdir(dir)) != NULL){
		if(dirptr->d_name[0] == '.'){
			continue;
		}
		//tries to get information about file (can also be a directory)
		if(lstat(assemblePathstring(path,assemblePathstring("/",dirptr->d_name)), &attribut) == -1){
			printf("Error in lstat function \n\n");
			exit(1);
		}	
		char *timestr = ctime(&attribut.st_mtime);

		//checks wether the current file is a reg. file, directory, character device or an unknown file
		if(attribut.st_mode & S_IFREG){
			off_t size = attribut.st_size;
			compareSize(size,path,(*dirptr).d_name);
			printf("Regular file: %s Size %jdBytes Last changed: %s",(*dirptr).d_name,size,timestr);
		}
		else if(attribut.st_mode & S_IFDIR){
			printf("Dir: %s Last changed:%s ",(*dirptr).d_name,timestr);
			//recursive function call
			du_hog_readdir(assemblePathstring(path,assemblePathstring("/",(*dirptr).d_name)));					
		}
		else if(attribut.st_mode & S_IFCHR){
			printf("Devicefile: %s Last changed: %s",(*dirptr).d_name,timestr);
		}else
			printf("Unknown file\n");
	}
	//tires to close current directory
	if(closedir(dir) == -1)
		printf("Error closing dir\n\n");


}

int main(int argc, char *argv[]){
	int count = 10;
	char *path = ".";

	if(argc >= 2){
		if(argv[1][0]=='-' && argv[1][1]=='?'){
			printf("Usage: %s [-N] [dir]\n\n",argv[0]);	
			exit(0);
		}
		else if(argv[1][0]=='-'){
			count = atoi(argv[1]+1);
			printf("Listing the %d biggest files\n\n",count); 
		}
		else{
			path = argv[1];
		}
	}
	if(argc>=3){
		path = argv[2];
	}

	du_hog_readdir(path);
	debug_printList();	

	return 0;
}

//helper functions
void compareSize(off_t size, char* path, char *fname){
	if(size < list[COUNT-1].size){
		//nothing todo in this case file is not large enough to be considered top COUNT
		#ifdef DEBUG
		printf("---Smaller FILE found: %s SIZE: %jdBytes---\n",fname,size);
		#endif
		return;
	}
	#ifdef DEBUG
	printf("---FILE for list found---\n");
	#endif 

	char *fpath = assemblePathstring(path,fname);

	file newFile = {fpath,size};
	list[COUNT-1] = newFile;
	qsort(list,COUNT,sizeof(file),cmpfunc);
	#ifdef DEBUG
	debug_printList();
	#endif
}

int cmpfunc(const void *a, const void *b){
	file *a1 = (file*)a;
	file *b1 = (file*)b;
	return (b1->size - a1->size);
}

//generate fpath for struct, path + '/' + fname + \0
char* assemblePathstring(char *path, char *fname){
	char *fpath = calloc(1,strlen(path)+strlen(fname)+2);
	if(fpath == NULL){printf("ERROR allocating memory\n");exit(1);}
	memcpy(fpath,path,strlen(path));
	memcpy(fpath+strlen(path)+1, fname,strlen(fname)+1);//+1 to copy nullbyte \0
   	*(fpath+strlen(path)) = '/';	

	return fpath;
}

void debug_printList(){
	printf("\n\n");
	for(int i=0; i<COUNT; i++){
		printf("Name: %s Size: %jd \n",list[i].path,list[i].size);
	}
	printf("\n\n");
}
