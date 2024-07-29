
main: hello.o main.o
	gcc hello.o main.o -o main
mian.o: main.c hello.h
	gcc -c main.c -o main.o
hello.o: hello.h hello.c
	gcc -c hello.c