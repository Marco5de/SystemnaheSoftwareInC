#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "a"
//schließt stdout und eröffnet neue date, da der kleinste mögliche fd gewählt wird 
//wird der strom nun in die spezifizierte Datei umgelenkt
int main(){
	close(1);
	int fd = open(FILENAME,O_RDWR|O_CREAT);

	printf("Ich printe etwas\n");
}
