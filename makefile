CC = gcc
CFLAGS = -g -c -Wall
LDLIBS = -lpthread
.c.o:
	$(CC) $(CFLAGS) $< -o $@

all:
	make printNums
	make semSync
	make sumArray
	make produceConsume

printNums: printNums.o wrappers.o

sumArray: sumArray.o wrappers.o

semSync: semSync.o wrappers.o

produceConsume: produceConsume.o wrappers.o

wrappers.o: wrappers.h

printNums.o: wrappers.h

semSync.o: wrappers.h

sumArray.o: wrappers.h

produceConsume.o: wrappers.h

clean:
	-rm printNums semSync sumArray produceConsume *.o
