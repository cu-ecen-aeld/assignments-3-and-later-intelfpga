#
#ifeq ($(CROSS_COMPILE),aarch64-none-linux-gnu-)
#
#	CC = aarch64-none-linux-gnu-gcc
#else
#	CC = gcc
#endif

all: writer

writer: writer.o
	$(CC) $(LDFLAGS) -o writer writer.o

writer.o: writer.c
	$(CC) $(CCFLAGS) -c writer.c

clean: 
	rm -f *.o 
	rm -f writer
