#include "/home/jiaweiq/Myinclude/apue.h"
#include<fcntl.h>
static int globvar = 6;
char buf[] = "a write to stdout\n";

int main()
{
    int var;
    pid_t pid;

    
    var = 88;
    int fd;
    if( (fd = open("/home/jiaweiq/c_project/myforktxt.txt", O_RDWR)) == -1 )
    {
        err_sys("open error");
    }
    if(write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
    {
        err_sys("write error");
    }
    printf("before fork\n");  /*we don`t flush stdout*/

    if ((pid = fork()) < 0)
    {
        err_sys("frok error");
    } else if(pid == 0){
        // sleep(4);
        globvar++;
        var++;
        printf("子进程globar: %p\n",&globvar);
    } else {
        sleep(2);
        printf("父进程globar: %p\n",&globvar);
    }
    if(close(fd) == -1)
    {
        err_sys("close error");
    }
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    exit(0);
}