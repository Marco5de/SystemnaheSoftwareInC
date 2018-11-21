#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define N_MAX 8
#define BUFFLEN 255
#define MAX_NAME_LENGTH 255

/* setting the bth bit of a to 1*/
#define SETBIT(a,b) ((a) |= (1 << b))

/*Methods*/
int pickRand();
struct mountain convertToMountain();
void printArr(int len, struct mountain *ptr);
void displayGameState();
bool checkValid(int pos,int pick);
void initSrt();
void updateSrt(int pick, int pos);
int cmpMTN(const void *a, const void *b);

/*Structs*/
struct mountain{
	char name[MAX_NAME_LENGTH];
	int height;
}mount;

/*global Vars*/
struct mountain arr[N_MAX];
struct mountain srt[N_MAX];
int t;
int points=0;
unsigned char map = 1;

	
int main(){
	pickRand();
	initSrt();
	while(true){
		if(((points+1)%8)==0){
			pickRand();
			initSrt();
			map = 1;
		}
		displayGameState();
		printf("What is to be inserted where?(Enter \"PICK POSITION\")");
		int pick,pos;
		if(scanf("%d %d",&pick,&pos)!=2)
			exit(1);
		pick--;pos--;	
		if(checkValid(pos,pick)){
			SETBIT(map,pick);
			points++;
			updateSrt(pick,pos);
		}else{
			printf("Spiel beendet. Die erreichte Punktzahl beträgt %d Pkt \n\n",points);
		exit(0);
		}


	}
}
void updateSrt(int pick, int pos){
	int sortet = points%8;
	srt[sortet] = arr[pick];
	qsort(srt, N_MAX, sizeof(struct mountain), cmpMTN);
	printArr(N_MAX,srt);
}

bool checkValid(int pos,int pick){
	int currLast = points%8;
	if(map & (1 << pick)){
		printf("Fehlerhafte Eingabe. Ausgewähltes Feld bereits belegt\nExit!\n\n”");
		exit(1);
	}	
	if(pos > currLast+1){
		return (srt[currLast].height >= arr[pick].height);
	}else{
		if((pos != 0) && (pos!=N_MAX-1)){
			return ((srt[pos-1].height >= arr[pick].height) && (srt[pos+1].height <= arr[pick].height));
		}else if(pos == 0){
			return (arr[pick].height >= srt[0].height);
		}
	}
	return false;
}
/*	returns 0 if mnt have equal height
	returns >0 if param 2 is taller
	returns <0 if param 1 is taller
*/	
int cmpMTN(const void *a, const void *b){
	const struct mountain *m1 = (struct mountain *)a;
	const struct mountain *m2 = (struct mountain *)b;

	return (m2->height - m1->height);
}

void displayGameState(){
	printf("CurrentState:\n");
	for(int i=0;i<N_MAX;i++){
		printf("%d : \n",(i+1));
		printf("     %s\n",srt[i].name);
	}
	printf("Still to be sorted:\n");
	for(int i=0;i<N_MAX;i++){
		if(!(map & (1<<i))){
			printf("%d: %s with height %d\n",(i+1),arr[i].name,arr[i].height);
		}
	}	
	printf("\n\n");
}


void initSrt(){
	srt[0] = arr[0];
	for(int i = 0; i<N_MAX; i++){
	}
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
	while(fgets(buff,BUFFLEN,file)!=NULL){
		int M = rand()%t;
		t++;
		if(M < N_MAX){
			arr[M] = convertToMountain(buff);	

		}
	}
	fclose(file);
	return 0;
}

/*	returning struct to init array
*/
struct mountain convertToMountain(char *buf){
	char *ptr;
	struct mountain out = {"",0}; 
	ptr = strtok(buf,":");	
	strncpy(out.name,ptr,MAX_NAME_LENGTH);
	
	//call with NULL uses prev entered input (in this case buf)
	ptr = strtok(NULL,":");
	out.height = atoi(ptr);
	
	return out;

}

/* DEBUG AUSGABE */
void printArr(int len, struct mountain *ptr){
	for(int i=0; i<len;i++){
		printf("Name: %s   Height: %d\n",(ptr+i)->name,(ptr+i)->height);

	}
}	
