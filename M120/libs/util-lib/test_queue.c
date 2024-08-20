#include "util_queue.h"

int
__QueueCb(ST_UTIL_QUEUE_MSG *pstQueueNode)
{
    printf("__QueueCb %d\n", pstQueueNode->ulMsgOid);
    syslog(1, "__QueueCb %d", pstQueueNode->ulMsgOid);
    return 0;
}


int
main()
{
    int i = 0;
    int iIndex = UTIL_InitQueue(0, 0);

    printf("init main\n");
    syslog(1, "init main");

    if (UTIL_RegQueueCb(__QueueCb)) {
        goto END_LABLE;
    }

    ST_UTIL_QUEUE_MSG stQueueMsg;
    memset(&stQueueMsg, 0, sizeof(stQueueMsg));

    for (i = 0; i < 30; i++) {
        stQueueMsg.ulMsgOid = i;
        UTIL_AddQueueNode(iIndex, (char *)&stQueueMsg, sizeof(stQueueMsg));
        UTIL_AddQueueNode(iIndex, (char *)&stQueueMsg, sizeof(stQueueMsg));

        printf("UTIL_AddQueueNode %d\n", i);
        syslog(1, "UTIL_AddQueueNode %d", i);
        sleep(1);
    }

    printf("end main ok\n");
    syslog(1, "end main ok");

    return 0;

END_LABLE:
    UTIL_CloseQueue(iIndex);
    UTIL_UnRegQueueCb();

    printf("end main error\n");
    syslog(1, "end main error");
    return 1;
}

