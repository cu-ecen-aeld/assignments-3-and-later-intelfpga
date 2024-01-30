#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <fcntl.h>

#include "read_write.h"

#define PORT "9000"
#define SERVER_LOG "/var/tmp/aesdsocketdata"
#define BUF_SIZE 1000000

struct  addrinfo *servinfo;
int sock_fd, con_fd;
char buf[BUF_SIZE];

static void sigint_handler(int signo);
void register_signals();

int main (int argc, char *argv[]){

	
	struct addrinfo hints;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	
	char str_addr[INET6_ADDRSTRLEN];
	int rv;
	int peek = 1;
	
		
	memset(buf, 0, BUF_SIZE);
	
	memset(&hints, 0, sizeof (hints));
	
	openlog(argv[0], LOG_PID, LOG_USER); 
	register_signals();
	
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	rv = getaddrinfo( NULL, PORT, &hints, &servinfo);
	
	sock_fd = socket( servinfo->ai_family,
			   servinfo->ai_socktype,
			   servinfo->ai_protocol);
			   
	if(setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &peek, sizeof (peek))== -1)
		printf("setsocket failed \n");
			   
				   
	if (bind(sock_fd, servinfo->ai_addr, servinfo->ai_addrlen) != 0){
		shutdown(sock_fd, SHUT_RDWR);
		freeaddrinfo(servinfo);
		close(sock_fd);
		printf("socket could not bind to port \n");
		return -1;
	}
	
	if(argc >1 && strcmp(argv[1],"-d") == 0)
		daemon(1,1);
	
	
		
	listen(sock_fd, 10);
	
	int fd_w, fd_r;
	
	
	
	while(1){
	
		addr_size = sizeof (their_addr);
		con_fd = accept(sock_fd, 
				(struct sockaddr *) &their_addr,
				&addr_size);
		
		fd_w = open(SERVER_LOG, O_WRONLY | O_CREAT | O_APPEND, 0644);
		
		inet_ntop (their_addr.ss_family,
			   &(((struct sockaddr_in *) &their_addr)->sin_addr),
			   str_addr,
			   sizeof (str_addr));
		
		syslog(LOG_ERR, "Accepted connection from %s \n", str_addr);
						
		receive_line(con_fd, buf, BUF_SIZE);
		
		write_line(fd_w, buf);
		close(fd_w);
		
		fd_r = open(SERVER_LOG, O_CREAT, 0644);
		
		while(read_line(fd_r, buf, BUF_SIZE))
			
			send_line(con_fd, buf);
			
		close(fd_r);
		close(con_fd);
		syslog(LOG_ERR, "Closed connection from %s \n", str_addr);
		 
		
	}
	
	//freeaddrinfo(servinfo);
	//close(sock_fd);
	//return 0;
	
}

static void sigint_handler(int signo){

	switch (signo){
		
		case SIGINT:
			syslog(LOG_ERR, "Caught signal, exiting \n");
			break;
		case SIGHUP:
			syslog(LOG_ERR, "Caught signal, exiting \n");
			break;
		case SIGTERM:
			syslog(LOG_ERR, "Caught signal, exiting \n");
			break;
		
		default:
			syslog(LOG_ERR, "Caught unknown SIG \n");
					
	}

	close(con_fd);
	shutdown(sock_fd, SHUT_RDWR);
	close(sock_fd);
	freeaddrinfo(servinfo);
	remove(SERVER_LOG);
	//shutdown();
	exit(0);

}
void register_signals(){

	signal(SIGINT,  sigint_handler);
	signal(SIGTERM, sigint_handler);

}
