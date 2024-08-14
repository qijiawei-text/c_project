#include "/home/jiaweiq/Myinclude/apue.h"
#include<error.h>

//接受到SIGHUP的处理函数，如果没有该函数，默认是终端进程
static void sig_hup(int signo)
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void pr_ids(char *name)
{
    printf("%s: pid =%ld, ppid=%ld, tgprg = %ld\n", name, (long)getpid(), (long)getppid(),
    (long)tcgetpgrp(STDIN_FILENO)); //通过标准输入的fd获取会话首进程
    fflush(stdout);
}

int main(void)
{
    char c;
    pid_t pid;

    pr_ids("parent");

    if((pid = fork())<0)
    {
        err_sys("fork error");
    }
    else{
        if(pid>0)
        {
            sleep(5); // 暂停5秒，使子进程暂停
        }
        else{
            pr_ids("child");
            signal(SIGHUP, sig_hup); //绑定信号与处理函数
            kill(getpid(), SIGTSTP); //自己给自己发送信号，使自己暂停
            pr_ids("child"); //当接受到内核的SIGHUP和SIGCONT后继续执行。
            if(read(STDIN_FILENO, &c, 1)!=1)
            {
                printf("read error %d on controlling TTY\n",errno);
            }
        }

    }
    exit(0);
}