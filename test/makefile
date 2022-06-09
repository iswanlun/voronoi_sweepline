# link with -lm

test: *.c ../src/*.o
	gcc -ggdb -Wall $^ -lm

%.o:
	make o -C ../src/