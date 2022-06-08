# link with -lm
CC = gcc

libfortune.so: beach_line.o events.o faces.o fortune.o
	$(CC) -shared -o $@ $^

libfortune.a: beach_line.o events.o faces.o fortune.o
	ar rc $@ $^
	ranlib $@

%.o: %.c
	$(CC) -c -Wall -Werror -fpic $^ -O3 -lm 

clean:
	rm *.o || \
	rm *.so || \
	rm *.a
