#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


void die(){
	exit(1);
}



int main(int argc, char **argv){
	if(argc!=4)
		die();
	char *filename = argv[1];
	off_t offset;
	if((offset=atoi(argv[2]))<0)
		die();
	char *replace = argv[3];

	int fd;
	if((fd=open(filename,O_RDWR))<0)
		die();
	if(lseek(fd,offset,SEEK_SET)<0)
		die();

	int ret;
	int nbyte = strlen(replace);
	const char *cbuf = replace;

	do{
		ret=write(fd,cbuf,nbyte);
		cbuf+=ret;
		nbyte-=ret;
	}while(ret>0 && nbyte>0);

	close(fd);

}

