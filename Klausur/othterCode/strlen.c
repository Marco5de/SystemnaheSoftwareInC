#include <stdio.h>
#include <unistd.h>

size_t myown_strlen(const char *str){
	size_t len = 0;
	while(*str)
		str++,len++;
	return len;
}

int main(){
	printf("Page size = %d Byte\n",getpagesize());
	printf("%ld\n",myown_strlen("Hallo ich bin ein String mit einer Länge"));
}
