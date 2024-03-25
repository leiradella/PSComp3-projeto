#compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -g -pthread

#queue flag
LDFLAGS= -lrt

#source files
SRC = reghist.c reghist_socket.c reghist_commands.c

#header files
HDR = reghist.h reghist_socket.h reghist_commands.h

#object files
OBJ = $(SRC:.c=.o)

#exe
EXEC = reghist

#build
all: $(EXEC)

#linking
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#clean
clean:
	rm -f $(OBJ) $(EXEC) HISTORICO.LOG

