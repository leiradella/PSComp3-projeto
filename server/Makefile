#compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -g -pthread

#source files
SRC = sismon.c server_threads.c server_socket.c server_commands.c

#header files
HDR = server_threads.h server_socket.h server_commands.h

#object files
OBJ = $(SRC:.c=.o)

#exe
EXEC = sismon

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
	rm -f $(OBJ) $(EXEC)

