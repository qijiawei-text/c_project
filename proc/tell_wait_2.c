#include "/home/jiaweiq/Myinclude/apue.h"
#include <sys/wait.h>

static void charatatime (char *);

int main(void)
{
    pid_t pid;

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        WAIT_PARENT();
        charatatime("output from child\n");
    } else {
        charatatime("output from parent\n");
        TELL_CHILD(pid);
    }
    exit(0);
}

static void charatatime (char* str)
{
    char* ptr;
    int c;

    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0;){
        //每一次都会调用write，使内核尽可能的多次在两个进程之间切换
        putc(c, stdout);
    }
}



