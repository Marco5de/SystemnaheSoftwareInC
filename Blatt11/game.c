#include <stdio.h>
#include <stdbool.h>
#include <stralloc.h>
#include <math.h>
#include <unistd.h>
#include "hashing.h"
#include <stdlib.h>

#define DEBUG
#define pi 3.14159265358979323846
#define HASH_SIZE 20000 
#define F_NAME "gemeinden.txt"


struct city{
	struct city *next;
	stralloc name;
	double longitude;
	double latitude;
};

//global variables
struct city *hash[HASH_SIZE];
int counter;
//forward declarations
int init_game(FILE *file);
int read_line(FILE *file, stralloc *name, stralloc *longi, stralloc *lati);
void gameLoop();


void cleanHashptr(){
	for(int i=0; i<HASH_SIZE; i++){
		if(hash[i])
			printf("Unsafe\n");
	}
}

int main(){
	FILE *file = fopen(F_NAME,"r");
	if(!file){fprintf(stderr,"err: opening file\n");exit(1);}
	if(init_game(file)){
		fprintf(stderr,"Err: init_game\n");
	}
#ifdef DEBUG
	printf("counter: %d\n",counter);
#endif	
	if(gameLoop())


	return 0;

}


int read_line(FILE* fp, stralloc* name, stralloc *longi, stralloc *lati) {
	int current = 0;

	for(;;) {
		stralloc sa = {0};
		//check that there is space for one more byte
		if(!stralloc_readyplus(&sa, 1)) return 0;
		//read next byte
		if(fread(sa.s + sa.len, sizeof(char), 1 , fp) <=0) return 0;
		++sa.len;
		//check if the two strallocs are equal
		if(stralloc_diffs(&sa,":") == 0){current++;continue;}

		if(!current){
			stralloc_cat(name,&sa);
			continue;
		}else if(current == 1){
			stralloc_cat(longi,&sa);
			continue;
		}else if(current == 2){
			stralloc_cat(lati,&sa);
			if(lati->s[lati->len-1]=='\n')
				break;
		}	
	}
	return 1;
}


int init_game(FILE *file){
		#ifdef DEBUG
		printf("---inside init---\n");
		#endif
		stralloc name = {0};
		stralloc longi = {0};
		stralloc lati = {0};

		#ifdef DEBUG
		printf("--enter readline---\n");
		#endif
		while(read_line(file,&name,&longi,&lati)){
			unsigned int hashnum = compute_hash(&name);
			#ifdef DEBUG
			printf("Hash calculated: %u \n",hashnum);
			#endif
			int index = hashnum % HASH_SIZE;
			struct city *new_city = calloc(1,sizeof(struct city));
			if(!new_city){fprintf(stderr,"err: allocating mem\n");exit(1);}
			stralloc_copys(&new_city->name,name.s);
			new_city->longitude = atof(longi.s);
			new_city->latitude = atof(lati.s);
			new_city->next = NULL;
			if(!hash[index]){
				#ifdef DEBUG
				printf("First in list\n");
				#endif
				hash[index] = new_city;
			}else{
				#ifdef DEBUG
				printf("---Already ele in list---\n");
				counter++;
				#endif
				struct city *tmp = hash[index];
				while(tmp->next!=NULL){
					tmp = tmp->next;
				}
				tmp->next = new_city;
			}
			stralloc_init(&name);
			stralloc_init(&longi);
			stralloc_init(&lati);
		}

	return 0;
}



