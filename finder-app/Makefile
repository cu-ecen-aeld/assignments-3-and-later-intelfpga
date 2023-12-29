
ifeq ($(CROSS_COMPILE),aarch64-none-linux-gnu-)

	CC = aarch64-none-linux-gnu-gcc
else
	CC = gcc
endif

all: writer

writer: writer.o
	$(CC) -o writer writer.o

writer.o: writer.c
	$(CC) -c writer.c

clean: 
	rm *.o 
	rm writer
