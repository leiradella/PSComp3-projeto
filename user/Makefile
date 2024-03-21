#compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -g -pthread

#source files
SRC = intuti.c client_socket.c

#header files
HDR = client_socket.h

#object files
OBJ = $(SRC:.c=.o)

#exe
EXEC = intuti

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

