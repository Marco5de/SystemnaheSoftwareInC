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
			if(input > 0){
				cones &= ~(1 << (input-1));
				gameState--;
			}
			else if(input < 0){
				cones &= ~(1 <<(abs(input)-1));
				cones &= ~(1 <<(abs(input)));
				gameState -= 2;
			}
			printGame(cones,number_cones);
		 }else
			printf("INVALID INPUT. Selected cone already fallen. TRY AGAIN\n");
		
		if(gameState < 1)
			exit(1);
		//AI selecting random number (plays randomly, no logic involved)
		int selec;
		while(!(handleInput((selec = (rand()%(2*number_cones))-number_cones),cones)));
		printf("Your opponent chose: %d\n",selec);
		if(selec > 0){
			cones &= ~(1 << (selec -1));
			gameState--;
		}else if(selec < 0){
			cones &= ~(1 << (abs(selec)-1));
			cones &= ~(1 << (abs(selec)));
			gameState -= 2;
		}
		printGame(cones,number_cones);
		}
}
	/* END: game loop */

	
/* 	check wether the selected Cone (or a neighbor) is already fallen 
*	RETURN: bool ~ valid entry or not
*/
bool handleInput(int input,unsigned long int cones){
	bool valid = false;
	if(input > 0){
		valid = (cones & (1 << (input-1)));
	}else if(input < 0){
		valid = (cones & (1 << (abs(input)-1)));
		valid = valid && (cones & (1 << (abs(input))));
	}else if(input == 0)
		printf("Cone does not exist.\n");

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
	bool hit = false;
	printf("Your move: ");
	do{
		scanf("%d",&input);
		cleanBuffer();
	}while(abs(input > nc) || hit);
	return input;
}

void cleanBuffer(){
	int x;
	while((x=getchar()) != EOF && x != '\n');
}
