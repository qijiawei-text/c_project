#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void daemonize()
{
    // 执行一个 fork()，之后父进程退出，子进程继续执行。
    pid_t pid = fork();
    if (pid > 0)
        exit(0);
    else if (pid < 0)
    {
        perror("fork");
        exit(0);
    }
    // 子进程调用 setsid() 开启一个新会话。
    if (setsid() == -1)
    {
        perror("setsid");
        exit(0);
    }

    // 清除进程的 umask 以确保当守护进程创建文件和目录时拥有所需的权限。
    umask(0);

    // 修改进程的当前工作目录，通常会改为根目录（/）。
    chdir("/home/jiaweiq/c_project/mysocket");

    // 关闭守护进程从其父进程继承而来的所有打开着的文件描述符。
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 在关闭了文件描述符0、1、2之后，守护进程通常会打开/dev/null 并使用dup2() 使所有这些描述符指向这个设备。
}

// 代码主体逻辑部分
int main()
{
    daemonize();
    FILE *file = fopen("/home/jiaweiq/c_project/mysocket/time.txt", "a+");
    if (file == NULL)
    {
        perror("fopen");
        exit(0);
    }


    while (1)
    {
        time_t tt = time(NULL);
        struct tm t = *localtime(&tt);

        fprintf(file, "现在是北京时间: %d-%d-%d %d:%d:%d\n", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
        fflush(file); // 保证数据立即写入文件
        sleep(2);
    }

    return 0;
}
