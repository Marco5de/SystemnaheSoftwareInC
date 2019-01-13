#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	DIR *dir;
	struct dirent *dirzeiger;

	if(argc != 2) {
		printf("Fehler, es ist genau 1 Argument noetig!\n");
		exit(1);
	}

	if((dir=opendir(argv[1])) == NULL) {
		printf("Fehler beim oeffnen des Verzeichnisses!\n");
		exit(1);
	}

	while((dirzeiger = readdir(dir)) !=NULL)
		printf("%s\n", (*dirzeiger).d_name);

	if(closedir(dir) == -1)
		printf("Fehler beim schliessen des Verzeichnisses!\n");
	
	return 0;
}
