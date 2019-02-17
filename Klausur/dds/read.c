#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void die(){
	exit(1);
}
//start reading at offset and print to stdout
int main(int argc, char **argv){
	if(argc!=3)
		die();
	char *filename = argv[1];
	off_t offset;
	if((offset = atoi(argv[2]))<0)
		die();

	int fd;
	if((fd = open(filename,O_RDWR))<0)
		die();
	if(lseek(fd,offset,SEEK_SET)<0)
		die();

	int ret=1;
	int len=0;
	char *buf = calloc(1,sizeof(char)*8192);

	while(ret && len < 8192){
		ret = read(fd,buf+len,8192);
		if(ret<0)
			die();
		len+=ret;
	}
	int ret_w;
	int nbyte=8192;
	
	do{
		ret_w=write(1,buf,nbyte);
		buf+=ret_w;
		nbyte-=ret_w;
	}while(ret_w>0 && nbyte>0);

	close(fd);
}
