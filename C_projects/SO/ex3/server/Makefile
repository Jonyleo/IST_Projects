# Makefile
# Sistemas Operativos

CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -pthread --save-temps -O3 -Wall -Wextra -pedantic
LDFLAGS=-lm

.PHONY: all clean run

all: tecnicofs



# NOSYNC
tecnicofs: lib/hash.o lib/bst.o fs.o lib/lock.o lib/inodes.o main.o 
	$(LD) $(CFLAGS) $(LDFLAGS) -o tecnicofs lib/hash.o lib/bst.o fs.o lib/lock.o lib/inodes.o main.o 


lib/bst.o: lib/bst.c lib/bst.h
	$(CC) $(CFLAGS) -o lib/bst.o -c lib/bst.c

lib/hash.o: lib/hash.c lib/hash.h
	$(CC) $(CFLAGS) -o lib/hash.o -c lib/hash.c

lib/lock.o: lib/lock.c lib/lock.h
	$(CC) $(CFLAGS) -o lib/lock.o -c lib/lock.c

lib/inodes.o: lib/inodes.c lib/inodes.h
	$(CC) $(CFLAGS) -o lib/inodes.o -c lib/inodes.c

fs.o: fs.c fs.h lib/bst.h
	$(CC) $(CFLAGS) -o fs.o -c fs.c

main.o: main.c fs.h lib/bst.h
	$(CC) $(CFLAGS) -o main.o -c main.c

clean:
	@echo Cleaning...
	rm -f lib/*.o *.o tecnicofs

