#include "/home/jiaweiq/Myinclude/apue.h"
/*vfork与fork的区别： 1.vfork并不将父进程的地址空间完全复制到子进程中。
                     2.vfork保证紫禁城先运行，在他调用exec或exit之后父进程才可能被调度
                     执行。*/
int globvar = 6;

int main(void)
{
    int var;
    pid_t pid;
    var = 88;
    printf("before vfork\n");
    if ((pid = vfork()) < 0){
        err_sys("vfork error");
    } else if (pid == 0) {
        globvar++;
        var++;
        _exit(0);
    }

    printf("pid = %ld, glob =%d, var = %d\n", (long)getpid(), globvar, var);

    exit(0);
}