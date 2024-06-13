default: all

all: aesdsocket

aesdsocket : aesdsocket.o read_write.o thread_handler.o list.o
	$(CC) $(LDFLAGS) -o aesdsocket aesdsocket.o read_write.o list.o thread_handler.o -pthread
	
list.o: list.c
	$(CC) $(CCFLAGS) -c list.c

read_write.o: read_write.c
	$(CC) $(CCFLAGS) -c read_write.c
	
thread_handler.o: thread_handler.c
	$(CC) $(CCFLAGS) -c thread_handler.c

aesdsocket.o: aesdsocket.c
	$(CC) $(CCFLAGS) -c aesdsocket.c -pthread

clean: 
	rm -f *.o 
	rm -f aesdsocket
