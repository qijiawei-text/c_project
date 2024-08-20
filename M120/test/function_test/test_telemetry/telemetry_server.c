/**********************************************************************************
   Copyright(C), 2019, OPLINK Tech.Co.,Ltd
   FileName    : telemetry_server.c
   Description : telemetry_server main process
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2019/12/12   V1.0.0       Initial file.
**********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>


#include "telemetry_data.h"


#define IPV6_SERVER 1
#define MAXSIZE 4096


static int m_iExitServerLoop = 0;



static void
__TELE_InterruptHandler(int iSignum)
{
    if ((SIGINT == iSignum) ||
        (SIGKILL == iSignum) ||
        (SIGTERM == iSignum)) {
        m_iExitServerLoop = 1;
    }

    return;
}


int
main(int argc, char *argv[])
{
    struct rlimit rlim;
    int rc = 0;
    int sock;
#ifdef IPV6_SERVER
    struct sockaddr_in6 s_addr;
    struct sockaddr_in6 c_addr;
#else
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
#endif
    unsigned int cliAddrLen;
    uint8_t buffer[MAXSIZE];
    unsigned short servPort;
    int recvMsgSize;
    servPort = 1500;
    int count = 0;
    int iSockopt = 1;

    if (argc != 2) {
        printf("Usage: <port> \n");
        exit(1);
    }

    printf("telemetry server init ok\n");

    signal(SIGINT, __TELE_InterruptHandler);
    signal(SIGTERM, __TELE_InterruptHandler);

    rlim.rlim_cur = rlim.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &rlim);

    servPort = atoi(argv[1]);

    /* Create socket for sending/receiving datagrams */
#ifdef IPV6_SERVER
    if ((sock = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
#else
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
#endif
        printf("socket() failed.\n");
        m_iExitServerLoop = 1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &iSockopt, sizeof(iSockopt)) == -1) {
        printf("setsockopt() failed. \n");
        m_iExitServerLoop = 1;
    }

#ifdef IPV6_SERVER
    memset(&s_addr, 0, sizeof(struct sockaddr_in6));
    s_addr.sin6_family = AF_INET6;
    s_addr.sin6_addr = in6addr_any;
    s_addr.sin6_port = htons(servPort);

    /* Bind to the local address */
    if ((bind(sock, (struct sockaddr *)&s_addr, sizeof(s_addr))) < 0) {
        printf("bind() failed.\n");
        m_iExitServerLoop = 1;
    }
#else
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    /* Bind to the local address */
    if ((bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr))) < 0) {
        printf("bind() failed.\n");
        m_iExitServerLoop = 1;
    }
#endif

    while (!m_iExitServerLoop) {
        /* Set the size of the in-out parameter */
#ifdef IPV6_SERVER
        cliAddrLen = sizeof(c_addr);
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, buffer, MAXSIZE, 0, (struct sockaddr *)&c_addr, &cliAddrLen)) < 0) {
            printf("recvfrom() failed.\n");
        }
#else
        cliAddrLen = sizeof(clntAddr);
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, buffer, MAXSIZE, 0, (struct sockaddr *)&clntAddr, &cliAddrLen)) < 0) {
            printf("recvfrom() failed.\n");
        }
#endif
        TELEMETRY_UnpackOpticalStream(buffer, recvMsgSize);
        printf("Message: %d, length: %d\n", count, recvMsgSize);
        count++;
    }

    printf("Telemetry server process terminated!");

    exit((0 == rc) ? 0 : 1);
}

