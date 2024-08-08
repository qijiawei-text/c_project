#include "/home/jiaweiq/Myinclude/apue.h"
#include <sys/wait.h>

char* env_init[] = {"USER = unmknown", "PATH = /temp", NULL};

int main(void)
{
    pid_t  pid;

    if ((pid = fork()) < 0) {
        err_sys("fork erroe");
    } else if (pid == 0) {
        if (execle("/home/jiaweiq/c_project/proc/echoall", "echoall", "myarg1", "MY ARG2", (char*)0, env_init) < 0){
            err_sys("execle error");
        }
    }

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");
    if ((pid = fork()) < 0){
        err_sys("fork error");
    } else if (pid == 0){
        if (execlp("./echoall", "echoall", "only 1 arg", (char*)0) < 0){
            err_sys("execlp error");
        }
    }
    exit(0);
}