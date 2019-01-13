#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {
	struct stat attribut;

	if(argc==1) {
		printf("Fehler, zu wenig Argumente!!\n");
		exit(1);
	}

	while(*++argv) {
		printf("%s  ", *argv);
		if(stat(*argv, & attribut) == -1) {
			printf("Fehler beim aufrufen von stat!\n");
			exit(1);
		}
		char *timestr = ctime(&attribut.st_mtime);
		printf("%s", timestr);
	}

}
