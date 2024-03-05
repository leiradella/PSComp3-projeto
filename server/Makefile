#compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -g -pthread

#source files
SRC = sismon.c naosei.c server_socket.c

#header files
HDR = naosei.h server_socket.h

#object files
OBJ = $(SRC:.c=.o)

#exe
EXEC = projeto

#build
all: $(EXEC)

#linking
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

#compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#clean
clean:
	rm -f $(OBJ)

