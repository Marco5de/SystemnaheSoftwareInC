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
void printList();
void available();
void cleanBuffer();

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
	struct Word *fnext;
	struct Word *enext; 
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
	
	game();

	return 0;
}

void game(){
	printf("Geben sie das erste Element der Liste ein:\n");
	char *s = calloc(128,sizeof(char));
	fgets(s,sizeof(s),stdin);
	if(*s == '\n'){
		available();
		free(s);
		cleanBuffer();
	}else{
		printf("Falsche eingabe\n");
		exit(1);
	}	

	while(true){
		if(!counter){
			printf("Sie haben gewonnen!!\n\n");
			break;
		}	
		printf("%2d Words remaining:\n Current queue of words: %s,...,%s\n",counter,first.name,last.name);  
		printf("Next word:    ");		
		char *str = calloc(128,sizeof(char));
		
		fgets(str,sizeof(str),stdin);
		if(*str == '\n'){
			available();
			free(str);
			cleanBuffer();
			continue;
		}

		cleanBuffer();
		printf("stuck after bufferclean \n");
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
	for(struct word *i = list.head; i->bucketNext!=NULL; i=i->bucketNext){
		if(!i->set)
			printf("%s  ",i->name); 
	}
	printf("\n\n");
	printf("Press enter to continue\n");
}
struct word* checkValid(char *str){
	for(struct word *i = list.head; i->bucketNext !=NULL; i = i->bucketNext){
		if(strncmp(str,i->name,strlen(str)-1)==0)
			return i;
		else
			continue;
	}
	return NULL;
}	

int hashB(char *str){
	/* map uppercase char ascii val 0 to 25 */
	return ((int)*str-0x41);
}

int hashE(char *str){
	/* map lowercase char ascii val 0 to 25*/
	return ((int)*str-0x61);
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
		//printf("Stadt: %s mit HASH: %d\n",buf,hashB(buf));
		struct word *newWord;
		newWord = (struct word*) calloc(1,sizeof(*newWord));
		if(!newWord){exit(1);} //exiting if mem wasn't allocated
		buf[strlen(buf)-1] = 0; //cutting off newLine

		newWord->set = false; //set properties for new entry
		newWord->name = (char *)malloc(strlen(buf));
		if(!(newWord->name)){exit(1);}//exiting if mem wasn't allocated
		strcpy(newWord->name,buf);
		newWord->next = NULL;
		newWord->prev = NULL;
		newWord->bucketNext = NULL;
	
		int fchar = hashB(newWord->name); //add new entry to the two hashtables
		int nchar = hashE(newWord->name);
		if(fset.bucket[fchar] == 0){ //adding entry to bucketlist for first char
			fset.bucket[fchar] = newWord;
		}else{
			struct word *tmp = fset.bucket[fchar];
			while(tmp->fnext != 0){
				tmp = tmp->fnext;
			}tmp->fnext = newWord;	
		}

		if(eset.bucket[nchar] == 0){ //adding entry to bucketlist for last char
			eset.bucket[nchar] = newWord;
		}else{
			struct word *tmp = eset.bucket[nchar];
			while(tmp->enext != 0){
				tmp = tmp->enext;
			}tmp->enext = newWord;	
		}
	}


	fclose(file);

	return 0;

}

/*
* Also checking for blankspace stops ideling in this function. Could cause some bugs with cites that have spaces in their name
*/

void cleanBuffer(){
	int x;
	while((x=getchar()) != EOF && x != '\n' && x!=32);
}
