#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>



int read_line(int fd, char *buf, int buf_size){

	int nr=0;
	char *temp = buf;
	
	do
		nr = read(fd, buf, sizeof(char));
	
	while (*buf++ != '\n' && 
		buf-temp < buf_size &&
		nr != 0);
	
	*buf = '\0';
	
	return nr;
}


int receive_line(int fd, char *buf, int buf_size){
	
	int nr=0;
	char *temp = buf;
	
	do
		nr = recv(fd, buf, sizeof(char), 0);
	
	while (*buf++ != '\n' && 
		buf-temp < buf_size &&
		nr != 0);
	
	*buf = '\0';
	
	return nr;
	
}

int write_line(int fd, char *buf){

	return write(fd, buf, strlen(buf));

}

int send_line(int fd, char *buf){

	return send(fd, buf, strlen(buf), 0);

}
