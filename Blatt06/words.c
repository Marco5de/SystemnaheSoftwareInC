#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

/*
 * functions
*/ 
int hashB(char *str);
int hashE(char *str);
int init();
void game();
struct word* checkValid(char *str);
void available();
void cleanBuffer();
// macros
#define BUFFLEN 255
#define WORDSET_MAX 26
#define FILENAME "cities.txt"
/*
 * structs
 *
*/ 
struct word{
	char *name;
	bool set;
	struct word *fnext;
	struct word *enext; 
};
struct wordSet{
	struct word *bucket[WORDSET_MAX];
};	

/*
 * global vars
*/ 
struct wordSet fSet;
struct wordSet eSet;
int counter = 0;
struct word first;
struct word last;

int main(){
	init();

	srand(time(0)^getpid());
	int rng = rand()%counter;
	printf("random: %d\n",rng);
	int curr = 0;
	bool exit = false;
	for(int i=0; i<WORDSET_MAX; i++){
		if(exit)
			break;
		if(fSet.bucket[i]==0)
			continue;
		struct word *t = fSet.bucket[i];
		while(t != 0){
			if(rng == ++curr){
				first = *t;
				last = *t;
				exit = true;
				break;
			}else{
				t = t->fnext;
				continue;
			}	
		}
	}

	game();

	return 0;
}

void game(){
	while(true){
		if(!counter){
			printf("Sie haben gewonnen!!\n\n");
			break;
		}	
		printf("%2d Words remaining:\n Current queue of words: %s,...,%s\n",counter,first.name,last.name);  
		printf("Next word:    ");		
		char *str = calloc(128,sizeof(char));
		
		fgets(str,sizeof(str),stdin);
		if(str == 0){
			free(str);
			continue;
		}	
		if(*str == '\n'){
			available();
			free(str);
			cleanBuffer();
			continue;
		}

		cleanBuffer();
		struct word *selec;
		if((selec=checkValid(str)))
			printf("Gültige Eingabe. Sie haben %s gewaehlt!\n",selec->name);
		else{
			printf("Falsche Eingabe. Versuche es nocheinmal!\n");
			free(str);
			continue;
		}
		if(selec->set){
			printf("Stadt wurde bereits verwendet! Wählen sie eine andere\n\n");
			free(str);
			continue;
		}	
		if(*(first.name)+32 ==(*(str+strlen(str)-1))){
			printf("Ihre Auswahl war gültig. Füge vorne in Liste ein.\n\n");
			first = *selec;
			selec->set = true;
		}else if((*str)+32 == *(last.name + strlen(last.name)-1)){
			printf("Ihre Auswahl war gültig. Füge hinten in die Liste ein. \n\n");
			selec->set = true;
			last = *selec;
		}else{
			printf("Falsche Wahl versuchen sie es erneut\n\n");
			free(str);
			continue;
		}
		counter--;
		free(str);
	}
}

void available(){
	printf("\n\nVerfügbare Städte: \n");
	for(int i=0; i<26; i++){
		if(fSet.bucket[i] == 0)
			continue;
		struct word *t = fSet.bucket[i];
		while(t != 0){
			if(!t->set)
				printf("%s  ",t->name); 
			t = t->fnext;
		}	
	}
	printf("\n\n");
	printf("Press enter to continue\n");
}
struct word* checkValid(char *str){
	for(int j=0; j<26; j++){
		if(fSet.bucket[j]==0)
			continue;
		struct word *i = fSet.bucket[j];
		while(i != 0){
			if(strncmp(str,i->name,strlen(str)-1)==0)
				return i;
			else{
				i = i->fnext;
				continue;
			}
		}
	}
	return NULL;
}	

int hashB(char *str){
	/* map uppercase char ascii val 0 to 25 */
	return ((int)*str-0x41);
}

int hashE(char *str){
	/* map lowercase char ascii val 0 to 25*/
	return ((int)*str-0x61+32);
}

int init(){
	FILE *file;
	file = fopen(FILENAME,"r");
	if(file==NULL){
		printf("Error reading file\n\n");
		exit(1);
	}
	char buf[BUFFLEN];
	while(fgets(buf,BUFFLEN,file)){
		counter++;
		struct word *newWord;
		newWord = (struct word*) calloc(1,sizeof(*newWord));
		if(!newWord){exit(1);} //exiting if mem wasn't allocated
		buf[strlen(buf)-1] = 0; //cutting off newLine
	
		newWord->set = false; //set properties for new entry
		newWord->name = (char *)malloc(strlen(buf));
		if(!(newWord->name)){exit(1);}//exiting if mem wasn't allocated
		strcpy(newWord->name,buf);
		newWord->fnext = NULL;
		newWord->enext = NULL;
	
		int fchar = hashB(newWord->name); //add new entry to the two hashtables
		int nchar = hashE(newWord->name);
		if(fSet.bucket[fchar] == 0){ //adding entry to bucketlist for first char
			fSet.bucket[fchar] = newWord;
		}else{
			struct word *tmp = fSet.bucket[fchar];
			while(tmp->fnext != 0){
				tmp = tmp->fnext;
			}tmp->fnext = newWord;	
		}

		if(eSet.bucket[nchar] == 0){ //adding entry to bucketlist for last char
			eSet.bucket[nchar] = newWord;
		}else{
			struct word *tmp = eSet.bucket[nchar];
			while(tmp->enext != 0){
				tmp = tmp->enext;
			}tmp->enext = newWord;	
		}
	}


	fclose(file);

	return 0;

}
void cleanBuffer(){
	int x;
	while((x=getchar()) != EOF && x != '\n' && x!=32);
}
