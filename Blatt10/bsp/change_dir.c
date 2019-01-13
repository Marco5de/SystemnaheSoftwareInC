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
		printf("Konnte Verzeichnis nicht erstellen\n");
	else
		printf("Neues Verzeichnis mit dem Namen %s erstellt\n", pathname);

	if(chdir(pathname) == -1)
		printf("Konnte nicht in das Verzeichnis %s wechseln!\n", pathname);
	else
		printf("Erfolgreich nach %s gewechselt!\n", pathname);

	FILE* fp = fopen("testfile", "w");
	fclose(fp);

	return 0;
}
