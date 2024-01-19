#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

// writer app
int main(int argc, char *argv[]){

	openlog(argv[0], LOG_PID, LOG_USER);
	
	if(argc != 3){
		syslog(LOG_ERR, "wrong number of args:%d", argc);
		exit(1);
	}
	
	FILE *file = fopen(argv[1], "w");
	
	if(file == NULL){
		syslog(LOG_DEBUG, "file:<%s> could not be opened", argv[1]);
		return -1;
	}else{
	
		fputs(argv[2], file);
		syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);
	}
	fclose(file);
	return 0;
	
	
}
