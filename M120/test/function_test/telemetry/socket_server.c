#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sysrepo.h"
#include "sysrepo/values.h"
#include "gpb_telemetry.pb-c.h"
#include "DSC_inc.h"
#define MAXSIZE 4096

int
main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    unsigned int cliAddrLen;
    uint8_t buffer[MAXSIZE];
    unsigned short servPort;
    int recvMsgSize;
    GpbPmMessage *msg;
    servPort = 1500;

    int count = 0;
    FILE *fp;

    if (argc != 3) {
        /* run as a RPC handler */
        printf("Usage: <port> <file-name>\n");
        exit(1);
    }

    servPort = atoi(argv[1]);
    fp = fopen(argv[2], "w+");

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket() failed.\n");
    }

    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    /* Bind to the local address */
    if ((bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr))) < 0) {
        printf("bind() failed.\n");
    }

    for (;; ) {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(clntAddr);
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, buffer, MAXSIZE, 0, (struct sockaddr *)&clntAddr, &cliAddrLen)) < 0) {
            printf("recvfrom() failed.\n");
        }

        size_t msg_len = (size_t)recvMsgSize;
        msg = gpb_pm_message__unpack(NULL, msg_len, buffer);
        if (msg == NULL) {
            fprintf(stderr, "error unpacking incoming message\n");
            continue;
        }

        printf("Message: %d\n", count);
        fprintf(fp, "Message: %d\n", count);
        for (int i = 0; i < msg->n_entry; i++) {
            printf("Entry: %d\n", i);
            fprintf(fp, "Entry: %d\n", i);
            printf("Received: entity = %s\n", msg->entry[i]->entity);
            fprintf(fp, "Received: entity = %s\n", msg->entry[i]->entity);
            printf("Received: pm = %s\n", msg->entry[i]->pm);
            fprintf(fp, "Received: pm = %s\n", msg->entry[i]->pm);
            printf("Received: min = %s\n", msg->entry[i]->min);
            fprintf(fp, "Received: min = %s\n", msg->entry[i]->min);
            printf("Received: max = %s\n", msg->entry[i]->max);
            fprintf(fp, "Received: max = %s\n", msg->entry[i]->max);
            printf("Received: avg = %s\n", msg->entry[i]->avg);
            fprintf(fp, "Received: avg = %s\n", msg->entry[i]->avg);
            printf("Received: timestamp = %s\n", msg->entry[i]->timestamp);
            if (msg->granularity == GPB_PM_MESSAGE__GRANULARITY__ONE_DAY) {
                printf("Received: granularity = %s\n", "1Day");
                fprintf(fp, "Received: granularity = %s\n", "1Day");
            } else if (msg->granularity == GPB_PM_MESSAGE__GRANULARITY__FIFTEEN_MIN) {
                printf("Received: granularity = %s\n", "15Min");
                fprintf(fp, "Received: granularity = %s\n", "15Min");
            } else if (msg->granularity == GPB_PM_MESSAGE__GRANULARITY__ONE_MIN) {
                printf("Received: granularity = %s\n", "1Min");
                fprintf(fp, "Received: granularity = %s\n", "1Min");
            } else if (msg->granularity == GPB_PM_MESSAGE__GRANULARITY__UNTIMED) {
                printf("Received: granularity = %s\n", "unTimed");
                fprintf(fp, "Received: granularity = %s\n", "unTimed");
            }
            if (msg->entry[i]->validity == GPB_PM_COUNTER__VALIDITY_TYPE__NOT_AVAILABLE) {
                printf("Received: validity = %s\n", "notAvailable");
                fprintf(fp, "Received: validity = %s\n", "notAvailable");
            } else if (msg->entry[i]->validity == GPB_PM_COUNTER__VALIDITY_TYPE__PARTIAL) {
                printf("Received: validity = %s\n", "partial");
                fprintf(fp, "Received: validity = %s\n", "partial");
            } else if (msg->entry[i]->validity == GPB_PM_COUNTER__VALIDITY_TYPE__COMPLETE) {
                printf("Received: validity = %s\n", "complete");
                fprintf(fp, "Received: validity = %s\n", "complete");
            }
            printf("\n");
            fprintf(fp, "\n");
        }
        count++;
    }

    fclose(fp);
}
