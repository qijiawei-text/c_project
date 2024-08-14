
#include "/home/jiaweiq/Myinclude/apue.h"

static void signal_handler_fun(int); 



int main(void)
{
    if (signal(SIGUSR1, signal_handler_fun) == SIG_ERR)
        err_sys("捕捉不到 SIGUSR1");
    if (signal(SIGUSR2, signal_handler_fun) == SIG_ERR)
        err_sys("捕捉不到 SIGUSR2");
    while(1)
        pause();
}


static void signal_handler_fun(int signum) 
{
    if (signum == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signum == SIGUSR2)
        printf("received SIGUSR2\n");
    else 
        err_sys("received signal ：%d\n", signum);
    
}

