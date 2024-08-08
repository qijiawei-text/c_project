#include "/home/jiaweiq/Myinclude/apue.h"
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <syslog.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include<sys/resource.h>


#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

// 面向连接的服务器

/*初始化服务器*/
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;
    int err = 0;

    if ((fd = socket(addr->sa_family, type, 0)) < 0){
        return (-1);
    }
    if (bind(fd, addr, alen) < 0){
        goto errout;
    }
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
        if (listen(fd, qlen) < 0)
        goto errout;
    }
    return(fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return(-1);
}


int set_cloexec(int fd)
{
    int val;
    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
    {
        return -1;
    }
    val |= FD_CLOEXEC;
    return fcntl(fd, F_SETFD, val);
}

void serve(int sockfd)
{
    int clfd;
    FILE *fp;
    char buf[BUFLEN];

    set_cloexec(sockfd);
    while (1)
    {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0)
        {
            syslog(LOG_ERR, "ruptimeed: accept error:%s", strerror(errno));
            exit(0);
        }
        syslog(LOG_INFO, "connect success");
        set_cloexec(clfd);
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            sprintf(buf, "error:%s\n", strerror(errno));
            syslog(LOG_INFO, "send %s", buf);
            send(clfd, buf, strlen(buf), 0);
        }
        else
        {
            syslog(LOG_INFO, "open uptime succeed");
            while (fgets(buf, 20, fp) != NULL)
            {
                syslog(LOG_INFO, "send %s", buf);
                send(clfd, buf, strlen(buf), 0);
            }
            pclose(fp);
        }
        close(clfd);
    }
}


void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    // clear file creation mask
    umask(0);

    printf("++++++++++++\n");
    //get maximun number of file description
    if(getrlimit(RLIMIT_NOFILE, &rl)<0)
    {
        err_quit("%s, can't get file limit", cmd);
    }

    // become a session leader to lose controlling TTY
    if((pid = fork())<0)
    {
        err_quit("%s, can't fork", cmd);
    }
    else if(pid != 0)
    {
        exit(0);
    }
    setsid();
    printf("11111111111\n");
    // ensure future opens won't allocate controlling TTYs
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGHUP, &sa, NULL)<0)
    {
        err_quit("%s, can't ignore sighup", cmd);
    }
    if((pid = fork())<0)
    {
        err_quit("%s, can't fork", cmd);
    }
    else if(pid!=0)
    {
        exit(0);
    }
    printf("22222222222\n");
    // change the current working directory to the root
    if(chdir("/home/jiaweiq/c_project/mysocket")<0)
    {
        err_quit("%s, can't change directory to /", cmd);
    }
    char path[500];
    getcwd(path,sizeof(path));
	printf("调用chdir函数更改当前工作目录后：\n");
    printf("当前工作目录为：%s\n",path);
    printf("3333333333333333\n");
    //close all open file descriptors
    if(rl.rlim_max == RLIM_INFINITY)
    {
        rl.rlim_max = 1024;
    }
    printf("rl.rlim_max: %ld\n", rl.rlim_max);
    for(i=0;i<rl.rlim_max;i++)
    {
        printf("555555555555\n");
        close(i);
    }
    printf("11111111111\n");
    // attach file descriptors 0,1,2 to /dev/null
    fd0 =open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    // initialize the log file
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if(fd0 !=0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpection file descriptors %d,%d,%d", fd0, fd1, fd2);
        exit(1);
    }
}




char buf1[20];
int main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;

    if (argc != 1)
    {
        err_quit("usage: ruptimed");
    }
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
    {
        n = HOST_NAME_MAX;
    }

    if ((host = (char *)malloc(n)) == NULL)
    {
        err_sys("malloc error");
    }
    if (gethostname(host, n) < 0)
    {
        err_sys("gethostname error");
    }
    printf("hostname is %s\n", host);
    printf("============\n");

    daemonize("ruptimed"); //将进程转换为守护进程
    memset(&hint, 0, sizeof(hint));

    hint.ai_flags = AI_PASSIVE;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;

    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    {
        syslog(LOG_ERR, "ruptimed: getaddrinfo error:%s", gai_strerror(err));
        exit(1);
    }

    // struct sockaddr *aip;

    // for (aip = ailist; aip != NULL; aip = aip->ai_next)
    // {
    //     struct sockaddr_in *skin = (struct sockaddr_in *)ailist->ai_addr;
    //     if (inet_ntop(AF_INET, &(skin->sin_addr), buf1, 20) < 0)
    //     {
    //         syslog(LOG_INFO, "inet_ntop error\n");
    //     }
    //     else
    //     {
    //         syslog(LOG_INFO, "addr is %s, port is %d", buf1, ntohs(skin->sin_port));
    //     }
    // }

    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
        {
            serve(sockfd);
            exit(0);
        }
    }
    exit(0);
}











