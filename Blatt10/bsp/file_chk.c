#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	struct stat attribut;

	if(argc==1) {
		printf("Fehler, zu wenig Argumente!!\n");
		exit(1);
	}

	while(*++argv) {
		printf("%s = ", *argv);
		if(stat(*argv, & attribut) == -1) {
			printf("Fehler beim aufrufen von stat\n");
			exit(1);
		}
		if(attribut.st_mode & S_IFREG)
			printf("Regulaere Datei\n");
		else if(attribut.st_mode & S_IFDIR)
			printf("Directory\n");
		else if(attribut.st_mode & S_IFCHR)
			printf("Geraetedatei\n");
		else
			printf("Unbekannte Datei\n");

	}

}
