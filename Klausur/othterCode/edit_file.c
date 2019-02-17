#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int buff_write(int fd,const void *buf,size_t bytes){
	int ret=0;
	int nbytes = bytes;
	const char *cbuf = buf;

	do{
		ret = write(fd,cbuf,nbytes);
		cbuf+=ret;
		nbytes-=ret;
	}while(ret>0 && nbytes>0);

	return ret<0?ret:nbytes;
}

void die(){
	char *err="An error occured exiting main\n";
	buff_write(1,err,strlen(err));
	exit(1);
}

ssize_t readline(int fd,char *buf,size_t bufflen){
	int ret=1;
	int len=0;
	while(ret && len<bufflen){
		ret=read(fd,buf+len,bufflen);
		if(ret<0)
			die();

		len+=ret;
	}

	return len;
}


int main(int argc, char **argv){
	if(argc!=4){
		char *err = "Wrong amount of cmd args!\nUsage: filename offset replacelit\n";
		if((buff_write(1,err,strlen(err)))<0)
			die();
	}
	char *filename = argv[1];
	off_t offset;
	if((offset=atoi(argv[2]))<0)
		die();
	char *replace = argv[3];
	
	int fd;
	if((fd=open(filename,O_RDWR))<0)
		die();
	char *buf = calloc(1024,sizeof(char));
	if(!buf)
		die();
	if((readline(fd,buf,1024))<0)
		die();

	buff_write(1,buf,strlen(buf));

	/*stralloc sa = {};
	sa.s = "Some string\n";

	if((write(1,sa.s,strlen(sa.s)))<0)
		exit(1);
	*/		
}
