CC=g++
CFLAGS=-I.
DEPS = Server.h Client.h SharedData.h Commands.h Logins.h table.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sempos: Server.o Client.o main.o
	g++ -o sempos main.cpp Server.cpp Client.cpp SharedData.cpp Logins.cpp table.cpp -I -g -l pthread
