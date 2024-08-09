CC:=C:\Users\jiaweq1\Downloads\mingw64\bin\gcc.exe


main : mongoose.o main.o
	-$(CC) mongoose.o main.o -lwsock32 -o main

main.o : main.c
	-$(CC) -c main.c -lwsock32
mongoose.o : mongoose.c 
	-$(CC) -c mongoose.c -lwsock32
clean :	
	del main.o mongoose.o 