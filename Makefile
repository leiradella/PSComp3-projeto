main: sismon.o


sismon.o: sismon.c
	cc -c sismon.c

clean: rm main.o sismon.o