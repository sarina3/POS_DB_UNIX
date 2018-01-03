CC=g++
CFLAGS=-I.
DEPS = Server.h Client.h SharedData.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sempos: Server.o Client.o main.o
	g++ -o sempos main.o Server.o Client.cpp SharedData.cpp -I -g -l pthread
