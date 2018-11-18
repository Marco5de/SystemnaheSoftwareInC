#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N_MAX 8
#define BUFFLEN 255
#define MAX_NAME_LENGTH 255

/*Methods*/
int pickRand();
struct mountain convertToMountain();
void printArr(int len, struct mountain *ptr);

/*Structs*/
struct mountain{
	char name[MAX_NAME_LENGTH];
	int height;
}mount;

/*global Vars*/
struct mountain arr[8];
int t;



int main(){
	pickRand();

}




/***************************************************************************
	everything below this is for init of the game
	all methods above this are for game logic
***************************************************************************/	

/* 	returning 0 if succes
	anything != 0 singaling failure
*/		
int pickRand(){
	FILE *file;
	file = fopen("berge.txt","r");
	if(file == NULL){
		printf("Fehler beim öffnen der Datei!\n");
		return 1;
	}
	/*init setup for the array, starting to randomly add new mountains to it*/
	char buff[BUFFLEN];
	for(int i=0; i<N_MAX; i++){
		fgets(buff,BUFFLEN,file);
		arr[i] = convertToMountain(buff);
	}
	t = 8;
	srand(time(0)^getpid());	
	printArr(N_MAX,arr);
	while(fgets(buff,BUFFLEN,file)!=NULL){
		int M = rand()%t;
		t++;
		if(M < N_MAX){
			arr[M] = convertToMountain(buff);	

		}
	}
	printf("\n\n\n");
	printArr(N_MAX,arr);
	fclose(file);
	return 0;
}

/*	returning struct to init array
*/
struct mountain convertToMountain(char *buf){
	char *ptr;
	struct mountain out = {"",0}; 
	ptr = strtok(buf,":");	
//	printf("Name: %s",ptr);
	strncpy(out.name,ptr,MAX_NAME_LENGTH);

	ptr = strtok(NULL,":");
//	printf("Height: %d\n\n", atoi(ptr));
	out.height = atoi(ptr);
	
	return out;

}
void printArr(int len, struct mountain *ptr){
	for(int i=0; i<len;i++){
		printf("Name: %s   Height: %d\n",(ptr+i)->name,(ptr+i)->height);

	}
}	
