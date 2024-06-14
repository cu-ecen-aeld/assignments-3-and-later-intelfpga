#include "aesdsocket.h"
 
struct  addrinfo *servinfo;
int sock_fd, con_fd;
//char buf[BUF_SIZE];
List thread_list;
pthread_mutex_t write_mutex;
struct timespec start_time_val, current_time_val,
		current_time_res;
double start_realtime, current_realtime,
		current_realtime_res;
struct itimerval tv;
struct sigaction sa;
struct itimerspec it_spec;
timer_t thread_timer;

int main (int argc, char *argv[]){
	
	pthread_mutex_init(&write_mutex, NULL);
	
	if(argc >1 && strcmp(argv[1],"-d") == 0)
		daemon(1,1);
	
	// timer related
		
	setup_timer();
	
	//setitimer(CLOCKID, &tv, NULL);
		
	// socket realted 
	struct addrinfo hints;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	
	
	
	char str_addr[INET6_ADDRSTRLEN];
	int rv;
	int peek = 1;
	
		
	//memset(buf, 0, BUF_SIZE);
	
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
	
	
		
	listen(sock_fd, 10);
	
	int fd_w, fd_r;
	
	lst_init(&thread_list);
	
	while(1){
	
		addr_size = sizeof (their_addr);
		con_fd = accept(sock_fd, 
				(struct sockaddr *) &their_addr,
				&addr_size);
		
		//fd_w = open(SERVER_LOG, O_WRONLY | O_CREAT | O_APPEND, 0644);
		
		inet_ntop (their_addr.ss_family,
			   &(((struct sockaddr_in *) &their_addr)->sin_addr),
			   str_addr,
			   sizeof (str_addr));
		
		syslog(LOG_ERR, "Accepted connection from %s \n", str_addr);
		
		Thread_Hander_Node *temp_node = (Thread_Hander_Node*) malloc(sizeof (Thread_Hander_Node));
		
		temp_node->con_fd = con_fd;
		
		//temp_node->str_addr = str_addr;
		
		temp_node->complete=false;
		
		memset(temp_node->buf, 0, BUF_SIZE);
		
		temp_node->write_mutex_ptr = &write_mutex;
		
		lst_append(&thread_list, (List_Node *) temp_node);
		
		pthread_create( &(temp_node->thread),
				(void*) NULL,
				thread_handler,
				(void*) temp_node);	
				
		/**
		receive_line(con_fd, buf, BUF_SIZE);
		
		sem_wait(&write_sem);
		write_line(fd_w, buf);
		sem_post(&write_sem);
		
		close(fd_w);
		
		fd_r = open(SERVER_LOG, O_CREAT, 0644);
		
		while(read_line(fd_r, buf, BUF_SIZE))
			
			send_line(con_fd, buf);
			
		close(fd_r);
		close(con_fd);
		syslog(LOG_ERR, "Closed connection from %s \n", str_addr);
		
		**/ 
		 
		lst_init_it(&thread_list);
		Thread_Hander_Node *node;
		int thread_id=0;
		
		while(lst_has_next(&thread_list)){
			node = (Thread_Hander_Node*) lst_next(&thread_list);
			
			thread_id++;
			if(node->complete){
				pthread_join(node->thread, NULL);
				close(node->con_fd);
				//syslog(LOG_ERR, "Closed connection from %s \n",node->str_addr);
				lst_remove(&thread_list, (List_Node *) node);
				free(node);	
			}
		}
		
	}
	
	//freeaddrinfo(servinfo);
	//close(sock_fd);
	//return 0;
	
}

void setup_timer(){

	struct itimerval tv;
	struct sigaction sa;
	struct itimerspec it_spec;
	struct sigevent sev;
	
	memset(&sev, 0, sizeof(struct sigevent));
	memset(&it_spec, 0, sizeof(struct itimerspec));
	
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = (void *) timer_handler; 
	
	timer_create(CLOCKID, &sev, &thread_timer);

	it_spec.it_value.tv_sec = 1;
        it_spec.it_value.tv_nsec = 0;
        it_spec.it_interval.tv_sec = 5;
        it_spec.it_interval.tv_nsec = 0;
           
	timer_settime(thread_timer, TIMER_ABSTIME, &it_spec, NULL);
	
}

static void timer_handler(int signo){

	time_t t;
	struct tm *tmp;
	char outstr[350],formatted[350];
	
	t = time(NULL);
        tmp = localtime(&t);
        
        strftime(outstr, sizeof(outstr), "%a, %d %b %Y %T %z", tmp);
        
        //printf("timestamp:%s \n",outstr);
        
         
        sprintf(formatted,"timestamp:%s \n",outstr);
        
        pthread_mutex_lock(&write_mutex);
        int fd_w = open(SERVER_LOG, O_WRONLY | O_CREAT | O_APPEND, 0644); 
	write_line(fd_w, formatted);
		
	
	fsync(fd_w);
	close(fd_w);
	pthread_mutex_unlock(&write_mutex);
	
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
	signal(SIGALRM, timer_handler);

}

static void *thread_handler(void *threadp){

	int fd_r, fd_w;
	
	Thread_Hander_Node* list_node = (Thread_Hander_Node*) threadp;
	

	receive_line(list_node->con_fd, list_node->buf, BUF_SIZE);
	
	pthread_mutex_lock(list_node->write_mutex_ptr);
	
	fd_w = open(SERVER_LOG, O_WRONLY | O_CREAT | O_APPEND, 0644);
	write_line(fd_w, list_node->buf);
	close(fd_w);	
	pthread_mutex_unlock(list_node->write_mutex_ptr);
	
	fd_r = open(SERVER_LOG, O_CREAT, 0644);
		
	while(read_line(fd_r, list_node->buf, BUF_SIZE))
			
		send_line(list_node->con_fd, list_node->buf);
	
			
	close(fd_r);
	
	list_node->complete=true;
	

}
