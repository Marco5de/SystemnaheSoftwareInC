#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

bool checkInput(int i);
int classifyInput(int i);
void handleInput(int input, int p);	
void makeNumber();
int convertToInt(int i);

typedef enum {S0, S1, S2, S3, S4, S5, S6, S7} state;

state zustand = S0;
double curr = 0;
double expo = 0;
double dec = 0;
int input = 0;
double sumT = 0;
bool sign = false;
bool expSign = false;
int counterDec = 0;


double sumA = 0;
double c = 0;


int main(){
	while((input = getchar())!= EOF && input!='\n'){
		if(checkInput(input)){
			int x = classifyInput(input);
			switch(zustand){
				case S0: switch(x){
							case '+': handleInput(input,counterDec); zustand = S1; break;
							case '0': handleInput(input,counterDec); zustand = S2; break;
							case '.': handleInput(input,counterDec); zustand = S3; break;
							default: return 0;
							}break;
				case S1: switch(x){
							case '.': handleInput(input,counterDec); zustand = S3; break;
							case '0': handleInput(input,counterDec); zustand = S2; break;
							default: return 0;
						}break;
				case S2: switch(x){
							case '.': handleInput(input,counterDec); zustand = S4; break;
							case '0': handleInput(input,counterDec); zustand = S2; break;
							case 'e': handleInput(input,counterDec); zustand = S5; break;
							default:  makeNumber(); 
						}break;	
				case S3: switch(x){
							case '0': handleInput(input,++counterDec); zustand = S4; break;
							default: return 0;
						}break;	
				case S4: switch(x){
							case 'e': handleInput(input,counterDec); zustand = S5; break;
							case '0': handleInput(input,++counterDec); zustand = S4; break;
							default:  makeNumber(); 
						}break;
				case S5: switch(x){
							case '+': handleInput(input,counterDec);zustand = S6; break;
							case '0': handleInput(input,counterDec);zustand = S7; break;
							default: return 0;
						}break;	
				case S6: switch(x){
							case '0': handleInput(input,counterDec); zustand = S7; break;
							default: return 0;
						}break;	
				case S7: switch(x){
							case '0':handleInput(input,counterDec); zustand = S7; break;
							default: makeNumber(); 
						}break;
			}	
		}
	}
	makeNumber();
	printf("Regular sum: %.17lg\n",sumT);
	printf("Kahan sum:   %.17lg\n",sumA);
	printf("Difference:  %lg\n",fabs(sumT-sumA));
}	

int convertToInt(int i){
	switch(i){
		case 48: return 0;
		case 49: return 1;
		case 50: return 2;
		case 51: return 3;
		case 52: return 4;
		case 53: return 5;
		case 54: return 6;
		case 55: return 7;
		case 56: return 8;
		case 57: return 9;
		default: exit(1);
	}
	return 0;
}

void handleInput(int input, int c){
	if(classifyInput(input) == '0'){
		input = convertToInt(input);
		switch(zustand){
			case S0: curr = input; break;
			case S1: curr = curr*10 + input;break;
			case S2: curr = curr*10 + input;break;
			case S3: dec = input; break;
			case S4: if(dec==0)
						dec = input;
					else
						dec = dec *10 + input;
					break;
			case S5: expo = input; break;
			case S6: expo = input; break;
			case S7: if(expo == 0)
						expo = input;
					else 
						expo = expo*10 + input;
					break;	
			default: exit(0);
		}	
	}else if(classifyInput(input) == 'e'){
		switch(zustand){
			case S2: break;
		}	
	}else if(classifyInput(input) == '+'){
		switch(zustand){
			case S0: {if(input == '+'){sign=false;}
						else{sign=true;}}break;
			case S5: if(input == '+')
						expSign = false;
					else
					expSign = true;
					break;
			}		
	}
}
	
bool checkInput(int i){
	if(i==46 || i==43 || i==45 || i==69 || i==101 || (i<58 && i>47))
		return true;
	else
		return false;
}
int classifyInput(int i){
	if(i==69 || i==101)
		return 'e';
	else if(i==43 || i==45)
		return '+';
	else if(i<58 && i>47)
		return '0';
	else if(i==46)
		return '.';
	else 
		return -0;
}		

void makeNumber(){
	double output = curr;
	for(int i=0; i<counterDec; i++)
		dec*=0.1;		
	output += dec;	
	if(expo!=0){
		if(!expSign){
			for(int i=0; i<expo; i++)
				output *= 10;
		}else{
			for(int i=0; i<expo; i++){
				output*=0.1;
			}	
		}	
	}
	if(sign){
		output*=(-1);
		}
	//trivial sum
	sumT += output;
	//kahan sum
	double y = output - c;
	double t = sumA + y;
	c = (t - sumA) - y;
	sumA = t;

	//init
	curr = 0;
	expo = 0;
	dec = 0;
	input = 0;
	sign = false;
	expSign = false;
	counterDec = 0;
}	
