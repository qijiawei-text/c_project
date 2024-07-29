#include <sys/wait.h>
#include "/home/jiaweiq/Myinclude/apue.h"
void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n",
               WTERMSIG(status),
#ifdef WCOREDUMP
               WCOREDUMP(status) ? " (core file generated)" : "");
#else
               "");
#endif
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}

int main()
{
    pid_t pid;
    int status;
    // case-1: childs exits with 7
    if ((pid = fork()) < 0)   err_sys("fork err\n");
    else if (pid == 0)        exit(7);
    if (wait(&status) != pid) err_sys("wait err\n");
    pr_exit(status);

    // case-2: child aborts
    if ((pid = fork()) < 0)   err_sys("fork err\n");
    else if (pid == 0)        abort();
    if (wait(&status) != pid) err_sys("wait err\n");
    pr_exit(status);

    // case-3: child divide 0
    if ((pid = fork()) < 0)   err_sys("fork err\n");
    else if (pid == 0)        status = status/0;
    if (wait(&status) != pid) err_sys("wait err\n");
    pr_exit(status);
    return 0;
}
