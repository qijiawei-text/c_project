#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pbc.h"
#include "readfile.h"

#define ECHOMAX 10240 /* Longest string to echo */
int
main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr;
    unsigned int cliAddrLen;
    char echoBuffer[ECHOMAX];
    unsigned short echoServPort;
    int recvMsgSize;

    if (argc != 2) {
        printf("Usage: %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    struct pbc_env *env = pbc_new();
    struct pbc_slice slice;
    struct pbc_rmessage *r_msg = NULL;
    read_file("performance.pb", &slice);
    int rc = pbc_register(env, &slice);
    if (rc != 0) {
        printf("pbc_register error:%d \n", rc);
        exit(1);
    }

    echoServPort = atoi(argv[1]);
    /* First arg: local port */
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket() failed.\n");
    }

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);
    /* Bind to the local address */
    if ((bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0) {
        printf("bind() failed.\n");
    }

    for (;; ) {
        cliAddrLen = sizeof(echoClntAddr);
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *)&echoClntAddr, &cliAddrLen)) < 0) {
            printf("recvfrom() failed.\n");
        }
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        slice.buffer = echoBuffer;
        slice.len = recvMsgSize;
        r_msg = pbc_rmessage_new(env, "Performance", &slice);

        int data_n = pbc_rmessage_size(r_msg, "data");
        for (int i = 0; i < data_n; i++) {
            printf("data = %d\n", pbc_rmessage_integer(r_msg, "data", 0, NULL));
        }

        pbc_rmessage_delete(r_msg);
        /* Send received datagram back to the client */
        if ((sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&echoClntAddr, sizeof(echoClntAddr))) != recvMsgSize) {
            printf("sendto() sent a different number of bytes than expected.\n");
        }
    }
    pbc_delete(env);
}
