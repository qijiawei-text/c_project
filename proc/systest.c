#include "/home/jiaweiq/Myinclude/apue.h"
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int sysytem(const char *cmdstring)
{
    pid_t pid;
    int status;

    if (cmdsring == NULL)
        return(1);
    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid ==0) {
        execl("/bin/sh", "sh", "-c", cmdstring, (char*) 0);
        exit(127);
    } else {
        while (waitpid (pid , &status, 0) < 0){
            if (errno != EINTR){
                status = -1;
                break;
            }
        }
    }

    return(status);

}


