#include "/home/jiaweiq/Myinclude/apue.h"
#include <sys/wait.h>
int main()
{
    pid_t pid;
    if ((pid = fork()) < 0)  err_sys("fork err");
    else if (pid == 0)
    {
        if ((pid = fork()) < 0) err_sys("fork err");
        else if (pid > 0)       exit(0);
        // child-2 continues when its parent exit
        // then child-2's parent will be init (pid=1)
        sleep(2);
        printf("second child, parent pid = %u\n", getppid());
        exit(0);
    }
    if (waitpid(pid, NULL, 0) != pid) err_sys("waitpid err");
    exit(0);
}
//second child, parent pid = 1
