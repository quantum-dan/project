CC=gcc
CFLAGS=-I.
DEPS=server.h db.h
OBJ=main.o server.o db.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
