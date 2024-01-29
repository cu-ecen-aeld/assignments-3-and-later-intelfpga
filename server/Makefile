default: all

all: aesdsocket

aesdsocket : aesdsocket.o read_write.o
	$(CC) $(LDFLAGS) -o aesdsocket aesdsocket.o read_write.o

read_write.o: read_write.c
	$(CC) $(CCFLAGS) -c read_write.c

aesdsocket.o: aesdsocket.c
	$(CC) $(CCFLAGS) -c aesdsocket.c

clean: 
	rm -f *.o 
	rm -f aesdsocket
