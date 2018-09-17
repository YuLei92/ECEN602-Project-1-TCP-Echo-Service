# Enable gcc compiler 
CC = gcc
# Set -Wall as per requirement and -g for debug.
all: server client

ServerForMy.o: server.c
	gcc -c server.c

server: server.o
	gcc -o server server.o

client: client.o
	gcc -o client client.o

ClientForMy.o: client.c
	gcc -c client.c

clean:
	rm -rf *.o client server