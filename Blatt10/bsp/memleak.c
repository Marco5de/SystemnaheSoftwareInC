#include <stdio.h>
#include <stdlib.h>

int main(){

    void * ptr; 
    
    for (int i=0; i<10; i++){

	ptr = malloc(i);

	printf("Allocated %d bytes at %p\n", i,ptr);

    }

    free(ptr);
}
