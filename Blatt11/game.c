#include <stdio.h>
#include <stdbool.h>
#include <stralloc.h>
#include <math.h>
#include "hashing.h"
#include <stdlib.h>
#include <string.h>

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
void game_loop();
double distance(stralloc *c1,stralloc *c2);


int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr,"Usage: %s [city1] [city2]\n",argv[0]);
		exit(1);
	}
	
	FILE *file = fopen(F_NAME,"r");
	if(!file){fprintf(stderr,"err: opening file\n");exit(1);}
	if(init_game(file)){
		fprintf(stderr,"Err: init_game\n");
	}
#ifdef DEBUG
	printf("counter: %d\n",counter);
#endif	
	char *city1 = argv[1];
	char *city2 = argv[2];
	stralloc c1 = {0};
	c1.s = city1;
	c1.len = strlen(city1);
	stralloc c2 = {0};
	c2.s = city2;
	c2.len = strlen(city2);

	double d = distance(&c1,&c2);
	printf("distance between your selected cities: %f\n",d);
	game_loop();

	return 0;

}

void game_loop(){
	printf("Jetzt im Main Loop: Geben sie zwei Städte ein um die Distanz zwischen ihnen auszurechen\n"); 
	char buffer[256];
	while(1){
		while(!fgets(buffer,256,stdin));
		printf("Entered string %s",buffer);


	}
}

double distance(stralloc *c1, stralloc *c2){
	unsigned int index1 = compute_hash(c1)%HASH_SIZE;	
	unsigned int index2 = compute_hash(c2)%HASH_SIZE;

	#ifdef DEBUG
	printf("Hash: %u\n",index1);
	#endif
	struct city *city1 = hash[index1];
	struct city *city2 = hash[index2];


	if(!city1 || !city2){fprintf(stderr,"Err: hashing error\n");exit(1);}
	while(city1!=NULL && strcmp(c1->s,city1->name.s)){
		printf("Current city: %s Next: %p\n",city1->name.s,city1->next);
		city1 = city1->next;
	}	
	while(city2!=NULL && strcmp(c2->s,city2->name.s)){
		printf("Current city: %s\n",city2->name.s);
		city2 = city2->next;
	}	
	if(!city1 || !city2){fprintf(stderr,"Err: wrong cities\n");exit(1);}	
	
	#ifdef DEBUG
	printf("Distance between %s and %s\n",city1->name.s,city2->name.s);
	#endif

	double latitude_city1 = city1->latitude;
	double latitude_city2 = city2->latitude;
	double longitude_city1 = city1->longitude;
	double longitude_city2 = city2->longitude;
	//formulas off of wikipedia
	const double f = 1/298.257223563;
	const double a = 6378.137;

	const double F = 2*pi/360*(latitude_city1 + latitude_city2)/2;
	const double G = 2*pi/360*(latitude_city1 - latitude_city2)/2;
	const double l = 2*pi/360*(longitude_city1 - longitude_city2)/2;

	const double S = pow(sin(G),2) * pow(cos(l),2) + pow(cos(F),2) * pow(sin(l),2);
	const double C = pow(cos(G),2) * pow(cos(l),2) + pow(sin(F),2) * pow(sin(l),2);
	const double w = atan(sqrt(S/C));
	const double D = 2 * w * a;

	const double T = sqrt(S*C) / w;
	const double H1 = 3*T-1/(2*C);
	const double H2 = 3*T+1/(2*S);

	return D*(1 + f*H1*pow(sin(F),2)*pow(cos(G),2) - f*H2*pow(cos(F),2)*pow(sin(G),2));
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
			printf("Hash %s: %u \n",name.s,hashnum);
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



