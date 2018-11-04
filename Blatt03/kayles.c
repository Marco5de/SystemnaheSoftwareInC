#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
//TODO REMOVE bufferClean and Exit(1)

void printGame(unsigned long int cones,int nc);
int userInput(int nc);
void cleanBuffer();
bool handleInput(int input, unsigned long int cones);
void gameAction(unsigned long int *cones, int pos, int *gameState, int nc);

int main(){
	/* START: initial game setup*/
	unsigned long int cones = 0;
	int gameState;	

	srand(time(0));
	int number_cones;
	while((number_cones = rand()%32) < 4); 
	gameState = number_cones;

	//setting the first nc bits to 1 for init setup
	for(int i=0; i < number_cones; i++){
		cones |= (1<<i);
	}
	
	printf("*** Dudeny's Game of Kayles *** \n");
	printf("Number of cones: %d\n",number_cones);
	printGame(cones,number_cones);
	/* END: initial game setup */

	/* START: game loop */
	while(gameState > 0){
		int input = userInput(number_cones);
		printf("Input: %d\n",input);
		
		if(handleInput(input,cones)){
			gameAction(&cones,input,&gameState,number_cones);
		 }else
			printf("invalid input. selected cone already fallen. try again\n");
		
		if(gameState < 1)
			exit(1);
		//opponent selecting random number (plays randomly, no logic involved)
		int selec;
		while(!(handleInput((selec = (rand()%(2*number_cones))-number_cones),cones)));
		printf("your opponent chose: %d\n",selec);
		gameAction(&cones,selec,&gameState,number_cones);
		}
}
	/* end: game loop */

	
/* 	check wether the selected cone (or a neighbor) is already fallen 
*	return: bool ~ valid entry or not
*/
bool handleInput(int input,unsigned long int cones){
	bool valid = false;
	if(input > 0){
		valid = (cones & (1 << (input-1)));
	}else if(input < 0){
		valid = (cones & (1 << (abs(input)-1)));
		valid = valid && (cones & (1 << (abs(input))));
	}else if(input == 0)
		printf("cone does not exist.\n");

	return valid;

}
/*	print current game state to stdout */
void printGame(unsigned long int cones, int nc){
	for(int i = 0; i < nc; i++){
		if(cones & (1 << i))
			printf(" /\\");
		else
			printf("   ");
	}
	printf("\n");
	for(int i = 0; i < nc; i++){
		printf("%3d",i+1);
	}
	printf("\n");
}

int userInput(int nc){
	int input;
	printf("your move: ");
	do{
		scanf("%d",&input);
		cleanBuffer();
	}while(abs(input) > nc);
	return input;
}

void cleanBuffer(){
	int x;
	while((x=getchar()) != EOF && x != '\n');
}


void gameAction(unsigned long int* cones, int pos, int *gameState, int nc){
	int gs = *gameState;
	if(pos > 0){
		*cones &= ~(1 << (pos-1));
		--gs;
	}
	else if(pos < 0){
		*cones &= ~(1 <<(abs(pos)-1));
		*cones &= ~(1 <<(abs(pos)));
		gs -= 2;
	}
	*gameState = gs;
	printGame(*cones, nc);
}
