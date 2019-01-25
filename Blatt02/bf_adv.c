#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STD_KONTOSTAND 1000
#define WIN_ACES 62

void cleanBuffer();

int main(){
	int kontostand = STD_KONTOSTAND;
	srand(time(0));
	printf("*****Banca Francessa****** \n \
		In jeder Runde koennen Sie einen Teil Ihres Geldes auf eine der\
		folgenden Kombinationen setzen: \n\
		1: Aces, Augemsumme: 3 mit einer Gewinnchance von 1:61\n\
		2: Pequeno, Augensumme: 5 6 7 mit einer Gewinnquote von 1:1 \n\
		3: Grande, Augensumme 14 16 16 mit einer Gewinnquote von 1:1 \n\
		Kontostand: %d \n",kontostand);
	
	//gameLoop
	int count = 1;
	while(kontostand > 0){
		printf("\n\nDies ist die %d. Runde. \n Aktueller \
		Kontostand ist %d \n Geben sie Einsatz und Kombi ein:\
		\n",count++,kontostand);

		int einsatz=0, guess=0;

		do{
			printf("input: ");
			scanf("%d %d", &einsatz, &guess);
			cleanBuffer();
		}while(!(guess>0 && guess<4 && einsatz<=kontostand && einsatz > 0));	

		bool roundOver = false;
		kontostand -= einsatz;

		while(!roundOver){
		
			int dice1 = rand()%7+1;
			int dice2 = rand()%7+1;
			int dice3 = rand()%7+1;
			
			int sum = dice1 + dice2 + dice3;
			printf("Wuerfel: %d %d %d mit einer Augensumme von %d.\n",
				dice1, dice2,dice3,dice1+dice2+dice3);
			
			if(sum == 3){
				if(guess == 1){	
					kontostand += WIN_ACES * einsatz;
					printf("Sie gewinnen %d. Neuer Kontostand: %d \n",61*einsatz
					,kontostand);
				}else
					printf("Du verlierst\n");
				roundOver = true;
			}else if(sum == 5 || sum == 6 || sum == 7){
				if(guess == 2){	
					kontostand+= 2*einsatz;
					printf("Sie haben %d gewonnen. Neuer Kontostand %d.\n",
						einsatz,kontostand);
				}else
					printf("Du verlierst!\n");
				roundOver = true;
			}else if(sum == 14 || sum == 15 ||
					sum == 16){
				if(guess == 3){	
					kontostand += 2 * einsatz;
					printf("Sie haben %d gewonnen. Neuer Kontostand:\
					 %d.\n",einsatz,kontostand);
				}else
					printf("Du verlierst\n");
				roundOver = true;
			}else
				printf("Nichts passiert \n");

		}			
			
	}
	printf("Du hast kein Geld mehr. Das Spiel ist an dieser Stelle beenedet\
		du hast insgesamt %d Runden überleb\n\nt",count);
}
/* cleaning Buffer to remove invalid input */
void cleanBuffer(){
	int x;
	while((x = getchar()) != EOF && x != '\n');
}	
