#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>
#include <errno.h>
#include <string.h>

//#define DEBUG

//todo DESC instead of ASC

typedef struct file{
    char *path;
    off_t size;
}file;


struct fileElement{
    file fileEle;
    struct fileElement *next;
};

//forward declarations
void compareSize(off_t size, char *path, char *fname);
int cmpfunc(const void *a, const void *b);
void debug_printList();
char* concatString(const char *path,const char *fname);
char* assemblePathstring(const char *path, const char *middle, const char *fname);

struct fileElement *flist;
bool isFull = false;



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
#ifdef DEBUG
    printf("Succesfully opend dir: %s\n",path);
#endif


    while((dirptr = readdir(dir)) != NULL){
        if(dirptr->d_name[0] == '.'){
            continue;
        }
        //tries to get information about file (can also be a directory)
        if(lstat(assemblePathstring(path,"/",(*dirptr).d_name), &attribut) == -1){
            fprintf(stderr,"Error in lstat function\n");
            exit(1);
        }

        //checks wether the current file is a reg. file, directory, character device or an unknown file
        //is reg File
        if(attribut.st_mode & S_IFREG){
            off_t size = attribut.st_size;
            compareSize(size,path,(*dirptr).d_name);
        }
            //is DIR
        else if(attribut.st_mode & S_IFDIR){
            //recursive function call
            du_hog_readdir(assemblePathstring(path,"/",(*dirptr).d_name));
            continue;
        }
            //is CHARACTER DEVICE
        else if(attribut.st_mode & S_IFCHR){
            //printf("Devicefile: %s Last changed: %s",(*dirptr).d_name,timestr);
        }else
            fprintf(stderr,"Unknown file\n");
    }
    //tires to close current directory
    if(closedir(dir) == -1)
        fprintf(stderr,"Error closing dir");
}


int count;


int main(int argc, char *argv[]){
    count = 10;
    char *path = ".";

    if(argc >= 2){
        if(argv[1][0]=='-' && argv[1][1]=='?'){
            fprintf(stderr,"Usage: %s [-N] [dir]\n\n",argv[0]);
            exit(1);
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
    file newFile;
    newFile.size = size;
    newFile.path = concatString(path,fname);

    //handle case that list is empty
    if(!flist){
        flist = calloc(1, sizeof(struct fileElement));
        if(!flist){fprintf(stderr,"ERROR allocating memory \n");exit(1);}
        flist->fileEle = newFile;
        flist->next = 0; //should be redundant as it is cleared by calloc
        //if list was empty, first element is now established so this function done its job
        return;
    }

    //go through list until the two elemets are found between which the newFile will be inserted
    struct fileElement *lowerEle = NULL;
    struct fileElement *higherEle = flist;
    //while there still is an higher element and the size of it is bigger go to next element in list
    while(higherEle && size > higherEle->fileEle.size){
        lowerEle = higherEle;
        higherEle = higherEle->next;
    }

    //postion where file will be inserted is now found
    //case that element will be inserted as head of the list
    if(!lowerEle){
        flist = calloc(1, sizeof(struct fileElement));
        if(!flist){fprintf(stderr,"ERROR allocating memory\n");exit(1);}
        flist->next=higherEle;
        flist->fileEle = newFile;
    }
    //case that element isn't the head of the list, an will be inserted in the middle
    else{
        lowerEle->next = calloc(1,sizeof(struct fileElement));
        lowerEle->next->fileEle = newFile;
        lowerEle->next->next = higherEle;
    }

    struct fileElement *tmp;
    //check list size and free lower elements
    if(!isFull){
        tmp = flist;
        int i = 0;
        while(tmp){
            tmp = tmp->next;
            i++;
        }
        if(i>10)
            isFull=true;
    }
    if(isFull){
        tmp = flist;
        flist = flist->next;
        free(tmp);
    }

    //in either case the File should be inserted by now, so this function goes returns to recursiv function from where it's been called
    return;

}

//generate fpath for struct, path + '/' + fname + \0
char* concatString(const char *path,const char *fname){
    char *fpath = calloc(1,strlen(path)+strlen(fname)+1);
    if(fpath == NULL){fprintf(stderr,"ERROR allocating memory\n");exit(1);}
    memcpy(fpath,path,strlen(path));
    memcpy(fpath+strlen(path), fname,strlen(fname)+1);//+1 to copy nullbyte \0

    return fpath;
}

char* assemblePathstring(const char *path, const char *middle, const char *fname){
    char *string = concatString(path,middle);
    char *pathString = concatString(string,fname);
    free(string);
    return pathString;
}
void debug_printList(){
#ifdef DEBUG
    printf("\n\nPrinting List with %d elements\n",count);
#endif
	struct fileElement **arr = calloc(10,sizeof(struct fileElement));
	if(!arr){fprintf(stderr,"ERROR allocating memory\n");exit(1);}
    struct fileElement *ele = flist;
	int i = count;
    while(ele && count) {
		arr[--i] = ele;	
        ele = ele->next;
    }
	for(int i=0; i<count; i++){
		if(arr[i])
			printf("%10jd %s\n", (intmax_t)arr[i]->fileEle.size,arr[i]->fileEle.path);
	}
}
