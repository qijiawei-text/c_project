#include "ops_common.h"
#include "DevOpsData.h"

int
OPS_GetNames(char (**names)[DEF_BUF_SIZE_64], size_t *count)
{
    int rc = SR_ERR_OK;
    static char (*pacKeys)[DEF_BUF_SIZE_64] = NULL;
    static char acKeys[OPS_DEV_NUM_MAX][DEF_BUF_SIZE_64];
    static size_t nCount = 0;

    if (NULL == pacKeys) {
        memset(acKeys, 0, sizeof(acKeys));
        rc = DSC_GetOpsNames(acKeys[0], sizeof(acKeys[0]), &nCount);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsNames");
            return rc;
        }

        if (nCount < 1) {
            LOG_WRITE(EN_LOG_ERROR, "DSC_GetOpsNames, OPS count is %lu < 1", nCount);
            rc = SR_ERR_NOT_FOUND;
            return rc;
        }

        pacKeys = acKeys;
        LOG_WRITE(EN_LOG_NOTICE, "OPS_GetNames, %s, %lu", pacKeys, nCount);
    }

    if (NULL != names) {
        *names = pacKeys;
    }
    if (NULL != count) {
        *count = nCount;
    }

    return rc;
}

uint32_t
OPS_GetNumber(uint32_t uiDevType, uint32_t uiPortId)
{
    if (uiDevType < DEV_TYPE_OPS4A) {
        return (uiDevType - DEV_TYPE_OPS) * OPS1_OPS_CNT_MAX + uiPortId;
    }

    return (DEV_TYPE_OPS4A - DEV_TYPE_OPS) * OPS1_OPS_CNT_MAX + (uiDevType - DEV_TYPE_OPS4A) * OPS4_OPS_CNT_MAX + uiPortId;
}

int
OPS_CreateUDPServer(uint16_t usPort)
{
    int iSockFd = -1;
    struct sockaddr_in stSockAddr;
    int rc = -1;

    memset(&stSockAddr, 0, sizeof(stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stSockAddr.sin_port = htons(usPort);

    iSockFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (iSockFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "socket failed");
        goto ERR_LABEL;
    }

    rc = bind(iSockFd, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
    if (rc < 0) {
        LOG_WRITE(EN_LOG_ERROR, "bind failed");
        goto ERR_LABEL;
    }

    return iSockFd;

ERR_LABEL:
    return -1;
}

void
OPS_SetRecvfromNonblocking(int iSockFd)
{
    struct timeval stTv;
    int rc = -1;

    memset(&stTv, 0, sizeof(stTv));
    stTv.tv_sec = 2;
    stTv.tv_usec = 0;

    rc = setsockopt(iSockFd, SOL_SOCKET, SO_RCVTIMEO, &stTv, sizeof(stTv));
    if (rc < 0) {
        /* return -1; */
        LOG_WRITE(EN_LOG_ERROR, "%s failed", __func__);
    }

    /* return 0; */
}

void
OPS_SetRunningThreadName(const char *pcThreadName)
{
    if (NULL == pcThreadName) {
        return;
    }

    prctl(PR_SET_NAME, pcThreadName);
}
