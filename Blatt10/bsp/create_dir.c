#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	char pathname[200];
	printf("Name des neuen Ordners eingeben: ");
	scanf("%199s", pathname);
	if(mkdir(pathname, 0755) == -1)
		printf("Konnte Verzeichnis nicht erstellen!!\n");
	else
		printf("Neues Verzeichnis mit dem Namen %s erstellt\n", pathname);

	return 0;
}
