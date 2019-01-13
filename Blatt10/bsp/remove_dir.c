#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	char pathname[200];

	printf("Name des neuen Verzeichnises eingeben: ");
	scanf("%199s", pathname);
	if(mkdir(pathname, 0755) == -1) 
		printf("Konnte Verzeichnis nicht erstellen!\n");
	else
		printf("Neues Verzeichnis mit dem Namen %s erstellt\n", pathname);

	if(rmdir(pathname) == -1) 
		printf("Konnte Verzeichnis %s nicht loeschen\n", pathname);
	else
		printf("Verzeichnis %s geloescht\n",pathname);


	return 0;
}
