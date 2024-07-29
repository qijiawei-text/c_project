#include "/home/jiaweiq/Myinclude/apue.h"

 int main(int argc, char *argv[]){
    int i;
    for(i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s \n", i, argv[i]);
    }
    printf("argc: %d", argc);
    exit(0);
 }