
all: 
	gcc -Wall -o LINUX LINUX.c 

run:
	./LINUX

clean:
	rm LINUX.o
