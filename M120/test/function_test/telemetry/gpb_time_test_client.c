#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "time.h"
#include "pbc.h"
#include "readfile.h"

int
main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in fromAddr;
    unsigned short echoServPort;
    unsigned int dataSize;
    unsigned int fromSize;
    char *servIP;
    char echoBuffer[10240];
    int respStringLen;
    struct timeval start, send, recv, stop;

    if (argc != 4) {
        printf("Usage: %s <Server IP> <Echo Port> <Data Size>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1]; /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Use given port, if any */

    if (strcmp(argv[3], "100b") == 0) {
        dataSize = 100 / 4;
    } else if (strcmp(argv[3], "1k") == 0) {
        dataSize = 1000 / 4;
    } else if (strcmp(argv[3], "10k") == 0) {
        dataSize = 10000 / 4;
    } else {
        printf("    100b : 100 byte data\n");
        printf("    1k : 1k byte data\n");
        printf("    10k : 10k byte data\n");
        exit(1);
    }

    gettimeofday(&start, NULL);
    struct pbc_env *env = pbc_new();
    struct pbc_slice slice;
    read_file("performance.pb", &slice);
    int rc = pbc_register(env, &slice);
    free(slice.buffer);
    if (rc != 0) {
        printf("pbc_register error:%d \n", rc);
        exit(1);
    }

    struct pbc_wmessage *w_msg = pbc_wmessage_new(env, "Performance");
    slice.buffer = echoBuffer, slice.len = 1024;
    for (int i = 0; i < dataSize; i++) {
        pbc_wmessage_integer(w_msg, "data", 1, 0);
    }
    pbc_wmessage_buffer(w_msg, &slice);

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("socket() failed.\n");
    }
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);

    gettimeofday(&send, NULL);
    /* Send the string to the server */
    if ((sendto(sock, slice.buffer, slice.len, 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) != slice.len) {
        printf("sendto() sent a different number of bytes than expected.\n");
    }

    /* Recv a response */
    fromSize = sizeof(fromAddr);
    if ((respStringLen = recvfrom(sock, echoBuffer, 1024, 0, (struct sockaddr *)&fromAddr, &fromSize)) != slice.len) {
        printf("recvfrom() failed\n");
    }
    if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
        printf("Error: received a packet from unknown source.\n");
        exit(1);
    }

    gettimeofday(&recv, NULL);
    /* null-terminate the received data */
    echoBuffer[respStringLen] = '\0';

    struct pbc_rmessage *r_msg = NULL;
    slice.buffer = echoBuffer;
    slice.len = respStringLen;
    r_msg = pbc_rmessage_new(env, "Performance", &slice);

/*    int data_n = pbc_rmessage_size(r_msg, "data"); */
/*    for (int i = 0; i < data_n; i++) { */
/*        printf("data = %d\n", pbc_rmessage_integer(r_msg , "data" , 0 , NULL)); */
/*    } */

    gettimeofday(&stop, NULL);

    printf("start to stop took %lu microsecond\n", stop.tv_usec - start.tv_usec);
    printf("send to stop took %lu microsecond\n", stop.tv_usec - send.tv_usec);
    printf("send to recv took %lu microsecond\n", recv.tv_usec - send.tv_usec);

    pbc_rmessage_delete(r_msg);
    pbc_wmessage_delete(w_msg);
    pbc_delete(env);
    close(sock);
    exit(0);
}