#include "/home/jiaweiq/Myinclude/apue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        err_sys("fork() error");
    else if (pid != 0) {
        printf("parent\n");
        sleep(2);
        abort();
    }

    if ((pid = fork()) < 0)
        err_sys("fork() error");
    else if (pid != 0) {
        printf("first child\n");
        sleep(4);
        abort();
    }

    if ((pid= fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {
        printf("second child\n");
        execl("/home/jiaweiq/c_project/proc/testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0);
        exit(7);
    }

    if ((pid = fork()) < 0)
        err_sys("fork() error");
    else if (pid != 0) {
        printf("third child\n");
        sleep(8);
        exit(0);
    }

    sleep(6);
    kill(getpid(), SIGKILL);
    exit(6);

}