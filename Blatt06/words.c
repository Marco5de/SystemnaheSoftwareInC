#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

/*
 * currently using linked list instead of hash table, because of seg fault i am not able to fix
*/ 


/*
 * functions
*/ 
int hashB(char *str);
int hashE(char *str);
int init();
void game();
struct word* checkValid(char *str);
void printList();

#define BUFFLEN 255
/*
 * structs
 *
*/ 
struct word{
	char *name;
	bool set;
	struct word *next;
	struct word *prev;
	struct word *bucketNext;
};


struct wordSet{
	struct word *bucket[26];
};	


struct linkedList{
	struct word *head;
};	
/*
 * global vars
*/ 
struct wordSet bSet;
struct wordSet eSet;
struct linkedList list;
int counter = 0;
struct word first;
struct word last;

int main(){
	init();
	srand(time(0)^getpid());
	int rng = rand()%(counter-1);
	int i=0;
	struct word *tmp = list.head;
	while(i!=rng){
		i++;
		tmp = tmp->bucketNext;
	}
	first = *tmp;
	last = *tmp;
	
	game();

	return 0;
}

void game(){
	while(true){
		printf("%2d Words remaining:\n Current queue of words: %s,...,%s\n",counter,first.name,last.name);  
		printf("Next word:    ");		
		char *str = calloc(128,sizeof(char));;
		if(scanf("%s",str)!=1)
			exit(0);
	
		struct word *selec;
		if((selec=checkValid(str)))
			printf("Gültige Eingabe. Sie haben %s gewaehlt!\n",selec->name);
		free(str);

	}
}

/* function has seg fault if paramet str is not included in list, most likely error in for loop */
struct word* checkValid(char *str){
	for(struct word *i = list.head; i->bucketNext !=NULL; i = i->bucketNext){
		int len = strlen(str);//die aus der datei eingelesenen strings enden auf \n	
		if(strncmp(str,i->name,len-1)==0)
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
	file = fopen("states.txt","r");
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
		if(!newWord){exit(1);}
		char *str = malloc(128);
		str = buf;
		str[strlen(str)-1] = 0; //cutting off new line

		newWord->set = false; //set properties for new entry
		newWord->name = (char *)malloc(strlen(buf));
		strcpy(newWord->name,str);
		newWord->next = NULL;
		newWord->prev = NULL;
		newWord->bucketNext = NULL;
	
		free(str);

		if(list.head == NULL){
			//printf("Added first new word\n");
			list.head = newWord;
		}else{
			//printf("adding word in list\n");
			struct word *tmp = list.head;
			while(tmp->bucketNext != NULL)
				tmp = tmp->bucketNext;
			tmp->bucketNext = newWord;
		}	


	/*
		int bHash = hashB(newWord->name); //add new entry to the two hashtables
		int eHash = hashE(newWord->name);
		struct word *tmp = bSet.bucket[bHash]; //add entry to beginning Hash table
		if(tmp != NULL){
			printf("bucket already has element: %s\n",tmp->name);
			while(tmp->bucketNext != NULL){
				tmp = tmp->bucketNext;
			}
			tmp->bucketNext = newWord;//this line is causing a seg fault
		}else{
			bSet.bucket[bHash] = newWord;
			printf("new First word in bucket\n");
		}
		tmp = eSet.bucket[eHash]; //add new entry to end Hash table
		if(tmp!=NULL){
			while(tmp != NULL){
				tmp = tmp->bucketNext;
			}
			tmp->bucketNext = newWord;
		}else
			eSet.bucket[eHash] = newWord;
*/
	}


	fclose(file);

	return 0;

}











