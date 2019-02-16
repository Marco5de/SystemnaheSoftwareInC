#include <stdio.h>
#include <stdlib.h>

int *create_arr(unsigned int len, int start, int incr){
	printf("Inside create arr\n");
	int *ip = malloc(sizeof(int)*len);
	if(ip){
		int val = start;
		for(int i=0; i<len;++i, val +=incr){
			ip[i]=val;
		}
	}return ip;
}

void print_array(int *ip, unsigned int len){
	printf("inside print_arr\n");
	for(int i=0; i<len;++i){
		printf(" %d",ip[i]);
	}printf("\n");
}

void add_array(int *a, int *b, unsigned int len){
	printf("inside add_array\n");
	for(int i=0; i<len; ++i)a[i]+=b[i];
}

int main(){
	int *ip1 = create_arr(6,1,1);
	int *ip2 = create_arr(3,10,10);
	if(ip1 && ip2){
		print_array(ip1+3,3);
		add_array(ip1+3,ip2,3);print_array(ip1+3,3);
		add_array(ip2+1,ip2,2);print_array(ip2,3);
	}
}
