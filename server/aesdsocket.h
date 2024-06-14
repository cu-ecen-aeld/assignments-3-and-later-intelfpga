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

#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

#include <stdbool.h>
#include "read_write.h"
#include "list.h"

#define PORT "9000"
#define SERVER_LOG "/var/tmp/aesdsocketdata"
#define BUF_SIZE 1000000
#define CLOCKID CLOCK_REALTIME


typedef struct t_handler_node {
	LIST_ITEMS;
	char buf[BUF_SIZE];
	bool complete;
	pthread_t thread;
	int con_fd;
	char str_addr[INET6_ADDRSTRLEN];
	pthread_mutex_t *write_mutex_ptr;	
}Thread_Hander_Node;

static void *thread_handler(void *threadp);
static void sigint_handler(int signo);
static void timer_handler(int signo);
void register_signals();
void setup_timer();
