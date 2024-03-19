#compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -g -pthread

#linker flags
LDFLAGS= -lrt

#source files
SRC = reghist.c

#header files
#HDR = server_threads.h server_socket.h server_commands.h #

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
	rm -f $(OBJ) $(EXEC)

