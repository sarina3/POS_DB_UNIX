CC=g++
CFLAGS=-I.
DEPS = Server.h Client.h SharedData.h Commands.h Logins.h table.h Database.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sempos: Server.o Client.o main.o SharedData.o Logins.o table.o Database.o
	g++ -o sempos main.o Server.o Client.o SharedData.o Logins.o table.o Database.o -I -g -l pthread 
