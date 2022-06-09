# link with -lm
CC = gcc

so: beach_line.o events.o faces.o fortune.o
	$(CC) -shared -o libfortune.so $^

a: beach_line.o events.o faces.o fortune.o
	ar rc libfortune.a $^
	ranlib libfortune.a

o: beach_line.o events.o faces.o fortune.o

%.o: %.c
	$(CC) -c -Wall -Werror -fpic $^

clean:
	rm *.o ; rm *.so ; rm *.a
