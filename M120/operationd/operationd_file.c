/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_file.c
   Description : file upload and download.
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "sysrepo.h"
#include "util_inc.h"
#include "DSC_inc.h"
#include "oplkipc.h"
#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"

#include "oplk_file.h"
#include "oplkpersist.h"
#include "operationd_file.h"
#include "operationd_alm.h"
#include "operationd_queue.h"

#include "oplkupgd.h"

static char m_acDbRestoreFile[FILE_PATH_LEN_MAX + FILE_NAME_LEN_MAX] = {0};
static char m_acNodeId[NODE_ID_MAX_SIZE];

static int __RpcFileTransferCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                               sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcShowFileCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                           sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcDelFileCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                          sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcDbBackupCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                           sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcDbRestoreCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                            sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcDbActivateCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                             sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);
static int __RpcDbCancelCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                           sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx);



static ST_RPC_SUB m_astFileRpcSub[] = {
    {MODULE_NS_OOFT, __RpcFileTransferCb, RPC_XPATH_FILE_TRANSFER},
    {MODULE_NS_OOFT, __RpcShowFileCb, RPC_XPATH_SHOW_FILE},
    {MODULE_NS_OOFT, __RpcDelFileCb, RPC_XPATH_DEL_FILE},
    {MODULE_NS_OODS, __RpcDbBackupCb, DB_XPATH_BACKUP},
    {MODULE_NS_OODS, __RpcDbRestoreCb, DB_XPATH_RESTORE},
    {MODULE_NS_OODS, __RpcDbActivateCb, DB_XPATH_ACTIVATE},
    {MODULE_NS_OODS, __RpcDbCancelCb, DB_XPATH_CANCEL},
};

static int
__RemotePathParse(char *pcRemotePath, char *pcUserName, char *pcPassword, char *pcIp, char *pcDir)
{
    char *pcPos = NULL;
    char *pcTemp = NULL;
    int iErrCode = 0;
    char acUserName[OPLK_FILE_PATH_LEN] = {0};
    char acPassword[64] = {0};
    char acIP[OPLK_FILE_PATH_LEN] = {0};
    char acPort[8] = {0};
    char acRemoteDir[OPLK_FILE_PATH_LEN] = {0};
    int iLen = 0;

    pcPos = strchr(pcRemotePath, ':');
    if (NULL == pcPos) {
        iErrCode = 1;
        goto cleanup;
    }
    pcTemp = ++pcPos;
    /* parse username                                                           */
    pcPos = strchr(pcTemp, '@');
    if (NULL == pcPos) {
        iErrCode = 2;
        goto cleanup;
    }

    iLen = pcPos - pcTemp;
    if (iLen >= sizeof(acUserName)) {
        iErrCode = 3;
        goto cleanup;
    }

    *pcPos = '\0';
    snprintf(acUserName, sizeof(acUserName), "%s", pcTemp);

    /*parse host ip(ipv4/ipv6)                                                  */
    pcTemp = ++pcPos;
    if ('[' == *pcTemp) {
        pcPos = strchr(pcTemp, ']');
        if (NULL == pcPos) {
            iErrCode = 4;
            goto cleanup;
        }

        pcPos++;
        /* Fixed the bug of error logic, change && to || by MaoLu.              */
        if (NULL == pcPos || ':' != *pcPos) {
            iErrCode = 4;
            goto cleanup;
        }

        iLen = pcPos - pcTemp;
        if (iLen >= sizeof(acIP)) {
            iErrCode = 5;
            goto cleanup;
        }

        *pcPos = '\0';
        snprintf(acIP, sizeof(acIP), "%s", pcTemp);

    } else {
        pcPos = strchr(pcTemp, ':');
        if (NULL == pcPos) {
            iErrCode = 4;
            goto cleanup;
        }

        iLen = pcPos - pcTemp;
        if (iLen >= sizeof(acIP)) {
            iErrCode = 5;
            goto cleanup;
        }
        *pcPos = '\0';
        snprintf(acIP, sizeof(acIP), "%s", pcTemp);
    }

    /* If the port of host ip exists, parse it                                  */
    pcTemp = ++pcPos;
    if (pcTemp[0] >= '0' && pcTemp[0] <= '9') {
        pcPos = strchr(pcTemp, ':');
        if (NULL == pcPos) {
            iErrCode = 4;
            goto cleanup;
        }
        iLen = pcPos - pcTemp;
        if (iLen >= sizeof(acPort)) {
            iErrCode = 6;
            goto cleanup;
        }
        *pcPos = '\0';
        snprintf(acPort, sizeof(acPort), "%s", pcTemp);

        pcTemp = ++pcPos;

        if (iLen + strlen(acIP) + 1 >= sizeof(acIP)) {
            iErrCode = 6;
            goto cleanup;
        }
        strcat(acIP, ":");
        strcat(acIP, acPort);
    }

    /*parse remote path                                                         */
    /* pcTemp = ++pcPos;                                                        */
    pcPos = strchr(pcTemp, '&');
    if (NULL == pcPos) {
        iErrCode = 7;
        goto cleanup;
    }

    iLen = pcPos - pcTemp;
    if (iLen >= sizeof(acRemoteDir)) {
        iErrCode = 8;
        goto cleanup;
    }
    *pcPos = '\0';
    snprintf(acRemoteDir, sizeof(acRemoteDir), "%s", pcTemp);

    /*password at last                                                          */
    pcTemp = ++pcPos;
    iLen = strlen(pcTemp);
    if (iLen >= sizeof(acPassword)) {
        iErrCode = 9;
        goto cleanup;
    }
    snprintf(acPassword, sizeof(acPassword), "%s", pcTemp);

    /* check no empty                                                           */
    if ((0 == strlen(acUserName)) ||
        (0 == strlen(acPassword)) ||
        (0 == strlen(acRemoteDir)) ||
        (0 == strlen(acIP))) {
        iErrCode = 10;
        goto cleanup;
    }

    strcpy(pcUserName, acUserName);
    strcpy(pcPassword, acPassword);
    strcpy(pcIp, acIP);
    strcpy(pcDir, acRemoteDir);

    return SR_ERR_OK;

cleanup:
    LOG_WRITE(EN_LOG_ERROR, "parse scp remote path failed. iErrCode = %d", iErrCode);
    return SR_ERR_INVAL_ARG;
}

static int
__ExecScpTransfer(const char *pcIp, const char *pcUserName, const char *pcPwd, const char *pcRemotePath, const char *pcLocalPath, const char *pcAction, bool bIsImage)
{
    int iErrCode = 0;
    int rc = SR_ERR_OK;
    char acCmdLine[1024] = {0};
    char acPwdForExpect[128] = {0};
    ST_UTIL_QUEUE_MSG stQueueMsg;
    bool bIsAsynProc = false;

    rc = UTIL_TranslateStrForExpect(pcPwd, acPwdForExpect, sizeof(acPwdForExpect));
    if (rc != 0) {
        iErrCode = 17;
        goto parse_error;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "/usr/local/bin/expect /usr/local/bin/scpDownload.exp %s %s %s %s %s %s %s 1>/dev/null 2>&1",
             pcIp, pcUserName, acPwdForExpect, pcRemotePath, pcLocalPath,
             (0 == strcmp(pcAction, FILE_TRANSFER_ONEKEY_UPGRADING)) ? FILE_TRANSFER_DOWNLOAD : pcAction,
             (bIsImage == false) ? "false" : "true");

    LOG_WRITE(EN_LOG_DEBUG, "Executing command : %s", acCmdLine);

    memset(&stQueueMsg, 0, sizeof(stQueueMsg));
    if (0 == strcmp(FILE_TRANSFER_DOWNLOAD, pcAction)) {
        if (true == bIsImage) {
            bIsAsynProc = true;
            stQueueMsg.ulMsgOid = OID_DOWNLOAD_IMAGE;
            strcpy(stQueueMsg.acMsgBuf, acCmdLine);
            stQueueMsg.ulMsgLen = strlen(acCmdLine) + 1;
        } else {
            bIsAsynProc = false;
        }
    } else if (0 == strcmp(FILE_TRANSFER_UPLOAD, pcAction)) {
        bIsAsynProc = false;
    } else {
        bIsAsynProc = true;
        stQueueMsg.ulMsgOid = OID_ONEKEY_UPGD;
        strcpy(stQueueMsg.acMsgBuf, acCmdLine);
        stQueueMsg.ulMsgLen = strlen(acCmdLine) + 1;
    }

    if (bIsAsynProc) {
        if (0 != UTIL_AddQueueNode(OPERATIOND_GetQueueIdx(), (char *)&stQueueMsg, sizeof(stQueueMsg))) {
            iErrCode = 18;
            LOG_WRITE(EN_LOG_ERROR, "add node to queue failed");
            goto parse_error;
        }
    } else {
        rc = system(acCmdLine);
        if (rc != 0) {
            iErrCode = 19;
            goto parse_error;
        }
        LOG_WRITE(EN_LOG_NOTICE, "Executing  system rc: %d. WEXITSTATUS: %d", rc, WEXITSTATUS(rc));
    }

    return 0;

parse_error:

    return iErrCode;
}

static int
__SftpTransferFile(char *pcAction, char *pcLocalPath, char *pcRemotePath)
{
#define CHECK_IP_REACHABLE_CMD  "ping -c 1 %s | grep ttl >/dev/null; echo $?"

    int rc = SR_ERR_OK;
    char acCmdLine[2048] = {0};
    char acBufTmp[512] = {0};

    char acIP[OPLK_FILE_PATH_LEN] = {0};
    char acIpTmp[OPLK_FILE_PATH_LEN] = {0};
    char acRemoteDir[OPLK_FILE_PATH_LEN] = {0};
    char acUserName[OPLK_FILE_PATH_LEN] = {0};
    char acPassword[64] = {0};
    char acPasswordTmp[64] = {0};
    char acPwdForExpect[128] = {0};
    char acLocalPathAndName[OPLK_FILE_PATH_LEN + FILE_NAME_LEN_MAX] = {0};
    char acFileName[FILE_NAME_LEN_MAX] = {0};
    char *pcInnerPath = NULL;
    uint32_t ulFileNum = 0;
    char acAction[DEF_BUF_SIZE_1024] = {0};
    struct stat stFileStat;
    char acCmnIp[128] = {0};
    char acCmnPort[8] = "22";
    int iStrLen = 0, iIpLen = 0, iPortLen = 0;
    bool bFlag = false;
    char acUserLog[DEF_BUF_SIZE_1024] = {0};

    LOG_WRITE(EN_LOG_NOTICE, "pcAction : %s, pcLocalPath : %s", pcAction, pcLocalPath);
    /* LOG_WRITE(EN_LOG_NOTICE, "pcRemotePath : %s", pcRemotePath);             */

    memset(acUserName, 0, sizeof(acUserName));
    memset(acPasswordTmp, 0, sizeof(acPasswordTmp));
    memset(acIpTmp, 0, sizeof(acIpTmp));
    memset(acRemoteDir, 0, sizeof(acRemoteDir));
    rc = __RemotePathParse(pcRemotePath, acUserName, acPasswordTmp, acIpTmp, acRemoteDir);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __RemotePathParse pcRemotePath");
        goto cleanup;
    }

    /* @172.16.166.21:20001:/xxx/yyy
       @172.16.166.21:/xxx/yyy   (default port 22)
     */

    memset(acIP, 0, sizeof(acIP));
    strcpy(acIP, "@");
    strcat(acIP, acIpTmp);

    memset(acPassword, 0, sizeof(acPassword));
    strcpy(acPassword, ":");
    strcat(acPassword, acPasswordTmp);

    memset(acLocalPathAndName, 0, sizeof(acLocalPathAndName));
    memset(acFileName, 0, sizeof(acFileName));
    pcInnerPath = FILE_GetInnerPathAndFileName(pcLocalPath, acFileName);
    if (NULL == pcInnerPath) {
        LOG_WRITE(EN_LOG_ERROR, "error FILE_GetInnerPathAndFileName local-path : %s", pcLocalPath);
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    strcpy(acLocalPathAndName, pcInnerPath);
    strcat(acLocalPathAndName, acFileName);


    /* LOG_WRITE(EN_LOG_NOTICE, "sftp user : %s, ip : %s, remote-dir : %s", acUserName, acIP, acRemoteDir);*/

    iIpLen = iStrLen = strlen(acIP);                                        /* 15                                                      */

    while (iIpLen > 0) {
        if (']' == acIP[iIpLen]) {
            bFlag = true;
        }

        if ((false == bFlag) && (':' == acIP[iIpLen])) {
            break;
        }
        iIpLen--;
    }

    if (iIpLen <= 0) {
        iIpLen = iStrLen;
        iPortLen = 0;

        memset(acCmnIp, 0, sizeof(acCmnIp));
        memcpy(acCmnIp, acIP, iIpLen);
    } else {
        iPortLen = iStrLen - iIpLen - 1;
        memset(acCmnIp, 0, sizeof(acCmnIp));
        memset(acCmnPort, 0, sizeof(acCmnPort));

        memcpy(acCmnIp, acIP, iIpLen);
        memcpy(acCmnPort, acIP + iIpLen + 1, iPortLen);
    }

    /* check ip reachable                                                       */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    memset(acBufTmp, 0, sizeof(acBufTmp));
    snprintf(acCmdLine, sizeof(acCmdLine), CHECK_IP_REACHABLE_CMD, &acCmnIp[1]);

    rc = UTIL_ExecuteCmd(acCmdLine, acBufTmp, sizeof(acBufTmp), 2100);
    if ((OPLK_OK != rc) || (0 != strcmp("0", acBufTmp))) {
        OPERATIOND_SetSftpIpAlm(SFTP_IP_UNREACHABLE, true);
        LOG_WRITE(EN_LOG_ERROR, "error sftp ip : %s can not reachable.", &acCmnIp[1]);
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    /**user@[IP]*****************************************************************/
    strcat(acUserName, acCmnIp);
    LOG_WRITE(EN_LOG_NOTICE, "username: %s", acUserName);

    if (0 == strcmp(pcAction, FILE_TRANSFER_UPLOAD)) {
        if (lstat(acLocalPathAndName, &stFileStat) < 0) {
            LOG_WRITE(EN_LOG_ERROR, "error upload acLocalPathAndName : %s", acLocalPathAndName);
            rc = SR_ERR_OPERATION_FAILED;
            goto cleanup;
        }
        if (S_ISREG(stFileStat.st_mode)) {
            sprintf(acAction, "\"put %s %s\"", acLocalPathAndName, acRemoteDir);
        } else {
            LOG_WRITE(EN_LOG_ERROR, "error upload acRemoteDir : %s", acRemoteDir);
            rc = SR_ERR_OPERATION_FAILED;
            goto cleanup;
        }
    } else {
        if (NULL != strstr(acLocalPathAndName, IMAGE_INNER_PATH)) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            snprintf(acCmdLine, sizeof(acCmdLine), "rm -fr %s*", IMAGE_INNER_PATH);
            rc = UTIL_System(acCmdLine);
            if (0 != rc) {
                LOG_WRITE(EN_LOG_ERROR, "rm -fr %s fail", IMAGE_INNER_PATH);
                rc = SR_ERR_INTERNAL;
                goto cleanup;
            }
        } else if (NULL != strstr(acLocalPathAndName, LOG_INNER_PATH) ||
                   (NULL != strstr(acLocalPathAndName, OTDR_SOR_INNER_PATH))) { /* log dir and otdr not support download                */
            LOG_WRITE(EN_LOG_ERROR, "error %s not support download!", acLocalPathAndName);
            rc = SR_ERR_INTERNAL;
            goto cleanup;
        } else if (NULL != strstr(acLocalPathAndName, TMP_CERT_INNER_PATH)) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            snprintf(acCmdLine, sizeof(acCmdLine), "mkdir -p %s", TMP_CERT_INNER_PATH);
            rc = UTIL_System(acCmdLine);
            if (0 != rc) {
                LOG_WRITE(EN_LOG_ERROR, "mkdir or rm %s fail", TMP_CERT_INNER_PATH);
                rc = SR_ERR_INTERNAL;
                goto cleanup;
            }
        } else {
            rc = FILE_GetFileCount(pcInnerPath, &ulFileNum);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Error FILE_GetFileCount : %s", pcInnerPath);
                rc = SR_ERR_INTERNAL;
                goto cleanup;
            }
            if (ulFileNum >= FILE_NUM_MAX) {
                LOG_WRITE(EN_LOG_ERROR, "Error db file num is over max : %d", ulFileNum);
                rc = SR_ERR_INTERNAL;
                goto cleanup;
            }
        }

        sprintf(acAction, "\"get %s %s\"", acRemoteDir, acLocalPathAndName);
    }

    rc = UTIL_TranslateStrForExpect(acPassword + 1, acPwdForExpect, sizeof(acPwdForExpect));
    if (rc != 0) {
        LOG_WRITE(EN_LOG_ERROR, "Error UTIL_TranslateStrForExpect acPassword");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    /**+1 format:    :password                                                  */
    memset(acCmdLine, 0, sizeof(acCmdLine));
    sprintf(acCmdLine, "/usr/local/bin/expect /usr/local/bin/fileTransfer.exp %s %s %s %s 1>/dev/null 2>&1",
            acUserName, acPwdForExpect, acAction, acCmnPort);
    /* LOG_WRITE(EN_LOG_NOTICE, "Executing command : %s", acCmdLine);           */
    rc = system(acCmdLine);
    if (rc != 0) {
        LOG_WRITE(EN_LOG_ERROR, "Error exec failed. acCmdLine : %s", acCmdLine);
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "Executing  system rc: %d", rc);

    snprintf(acUserLog, sizeof(acUserLog), "sftp,%s,%s,%s,%s", pcAction, acUserName, acRemoteDir, acLocalPathAndName);
    DSC_WriteUserLog(DEF_OCP_COMPONENT_KEY_CHASSIS, STR_ID_FILE_TRANSFER, acUserLog);
    sleep(1);

cleanup:
    return rc;
}

static int
__ScpTransferFile(char *pcAction, char *pcLocalPath, char *pcRemotePath)
{
    int iErrCode = 0;
    int rc = SR_ERR_OK;
    char acCmdLine[2048] = {0};
    char acIP[OPLK_FILE_PATH_LEN] = {0};
    char acRemoteDir[OPLK_PATH_MAX_LEN] = {0};
    char acUserName[OPLK_FILE_PATH_LEN] = {0};
    char acPassword[64] = {0};
    char acLocalPathAndName[OPLK_FILE_PATH_LEN + FILE_NAME_LEN_MAX] = {0};
    char acFileName[FILE_NAME_LEN_MAX] = {0};
    char *pcInnerPath = NULL;
    uint32_t ulFileNum = 0;
    bool bIsImage = false;
    ST_UPGD_INFO stUpgdInfo;

    LOG_WRITE(EN_LOG_NOTICE, "pcAction : %s, pcLocalPath : %s", pcAction, pcLocalPath);
    /* LOG_WRITE(EN_LOG_NOTICE, "pcRemotePath : %s", pcRemotePath);             */

    /* pcRemotePath example:
         scp:guangjung@172.16.166.20:/home/guangjung/work_dir/shasta_devel_20180822/tools/backup/FirmwareBundle&userpasword */
    /*find begin position of user name                                          */

    memset(acUserName, 0, sizeof(acUserName));
    memset(acPassword, 0, sizeof(acPassword));
    memset(acIP, 0, sizeof(acIP));
    memset(acRemoteDir, 0, sizeof(acRemoteDir));
    rc = __RemotePathParse(pcRemotePath, acUserName, acPassword, acIP, acRemoteDir);
    if (rc != SR_ERR_OK) {
        iErrCode = 10;
        goto parse_error;
    }
    LOG_WRITE(EN_LOG_DEBUG, "scp user : %s, ip : %s, remote-dir : %s", acUserName, acIP, acRemoteDir);

    memset(acLocalPathAndName, 0, sizeof(acLocalPathAndName));
    memset(acFileName, 0, sizeof(acFileName));
    pcInnerPath = FILE_GetInnerPathAndFileName(pcLocalPath, acFileName);
    if (NULL == pcInnerPath) {
        iErrCode = 11;
        goto parse_error;
    }
    strcpy(acLocalPathAndName, pcInnerPath);
    strcat(acLocalPathAndName, acFileName);

    /* TBD: only support download by scp
           example:
               /usr/local/bin/expect /usr/local/bin/scpDownload.exp 172.16.166.20 guangjung password_xxx  /home/guangjung/work_dir/shasta_devel_20180822/tools/backup/FirmwareBundle /tmp/FirmwareBundle
     */

    if (0 == strcmp(FILE_TRANSFER_DOWNLOAD, pcAction)) {

        if (NULL != strstr(acLocalPathAndName, IMAGE_INNER_PATH)) {
            memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
            UPGD_GetInfo(&stUpgdInfo);
            if (EN_SYS_TRANSFERRING != stUpgdInfo.ucSysState) {
                memset(acCmdLine, 0, sizeof(acCmdLine));
                snprintf(acCmdLine, sizeof(acCmdLine), "rm -fr %s*", IMAGE_INNER_PATH);
                rc = UTIL_System(acCmdLine);
                if (0 != rc) {
                    LOG_WRITE(EN_LOG_ERROR, "rm -fr %s fail", IMAGE_INNER_PATH);
                    iErrCode = 12;
                    goto parse_error;
                }
            } else {
                LOG_WRITE(EN_LOG_ERROR, "transfer is going.");
                goto parse_error;
            }
            bIsImage = true;
        } else if (NULL != strstr(acLocalPathAndName, LOG_INNER_PATH) ||
                   (NULL != strstr(acLocalPathAndName, OTDR_SOR_INNER_PATH))) { /* log dir and otdr not support download                */
            LOG_WRITE(EN_LOG_ERROR, "error %s not support download!", acLocalPathAndName);
            iErrCode = 13;
            goto parse_error;
        } else if (NULL != strstr(acLocalPathAndName, TMP_CERT_INNER_PATH)) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            snprintf(acCmdLine, sizeof(acCmdLine), "mkdir -p %s", TMP_CERT_INNER_PATH);
            rc = UTIL_System(acCmdLine);
            if (0 != rc) {
                LOG_WRITE(EN_LOG_ERROR, "mkdir or rm %s fail", TMP_CERT_INNER_PATH);
                iErrCode = 14;
                goto parse_error;
            }
        } else if (NULL != strstr(acLocalPathAndName, TMP_GNMI_CERT_INNER_PATH)) {
            memset(acCmdLine, 0, sizeof(acCmdLine));
            snprintf(acCmdLine, sizeof(acCmdLine), "mkdir -p %s", TMP_GNMI_CERT_INNER_PATH);
            rc = UTIL_System(acCmdLine);
            if (0 != rc) {
                LOG_WRITE(EN_LOG_ERROR, "mkdir or rm %s fail", TMP_GNMI_CERT_INNER_PATH);
                iErrCode = 14;
                goto parse_error;
            }
        } else {
            rc = FILE_GetFileCount(pcInnerPath, &ulFileNum);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Error FILE_GetFileCount : %s", pcInnerPath);
                iErrCode = 15;
                goto parse_error;
            }
            if (ulFileNum >= FILE_NUM_MAX) {
                LOG_WRITE(EN_LOG_ERROR, "Error db file num is over max : %d", ulFileNum);
                iErrCode = 16;
                goto parse_error;
            }
        }

        iErrCode = __ExecScpTransfer((const char *)acIP, (const char *)acUserName, (const char *)acPassword, (const char *)acRemoteDir, (const char *)acLocalPathAndName, (const char *)pcAction, bIsImage);
        if (iErrCode != 0) {
            goto parse_error;
        }

    } else if (0 == strcmp(FILE_TRANSFER_UPLOAD, pcAction)) {
        iErrCode = __ExecScpTransfer((const char *)acIP, (const char *)acUserName, (const char *)acPassword, (const char *)acRemoteDir, (const char *)acLocalPathAndName, (const char *)pcAction, bIsImage);
        if (iErrCode != 0) {
            goto parse_error;
        }

    } else {
        memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
        UPGD_GetInfo(&stUpgdInfo);

        if (EN_SYS_TRANSFERRING == stUpgdInfo.ucSysState) {
            LOG_WRITE(EN_LOG_ERROR, "transfer is going.");
            goto parse_error;
        }

        if (EN_SYS_UPGRADING != stUpgdInfo.ucSysState) {                    /*check upgd is in running                                 */
            memset(acCmdLine, 0, sizeof(acCmdLine));
            snprintf(acCmdLine, sizeof(acCmdLine), "rm -fr %s*", IMAGE_INNER_PATH);
            rc = UTIL_System(acCmdLine);
            if (0 != rc) {
                LOG_WRITE(EN_LOG_ERROR, "rm -fr %s fail", IMAGE_INNER_PATH);
                iErrCode = 12;
                goto parse_error;
            }

            bIsImage = true;

            iErrCode = __ExecScpTransfer((const char *)acIP, (const char *)acUserName, (const char *)acPassword, (const char *)acRemoteDir, (const char *)acLocalPathAndName, (const char *)pcAction, bIsImage);
            if (iErrCode != 0) {
                goto parse_error;
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "upgd is running!");
            goto parse_error;
        }
    }

    return rc;

parse_error:
    LOG_WRITE(EN_LOG_ERROR, "parse scp remote path failed. iErrCode = %d", iErrCode);
    return SR_ERR_INVAL_ARG;
}

/**
 * @brief
 *
 * send file is rpc, only ok or fail, how to snd in progress notification
 * @param [in]
 * @param [in]
 * @return
 */
int
__SendFileTransferNotice(char *pcPath, FILE_TRANSFER_STATUS_E eStatus)
{
#define WR_CNT 2
    sr_session_ctx_t *pstSess = DSC_GetRunningSession();

    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

    if (NULL == pstSess || NULL == pcPath || eStatus >= FILE_TRANSFER_CNT) {
        LOG_WRITE(EN_LOG_ERROR, "in para wrong");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = FILE_XPANT_EVENT_LOLCAL_PATH;
    astValues[0].type = SR_STRING_T;
    astValues[0].data.string_val = pcPath;

    astValues[1].xpath = FILE_XPANT_EVENT_STATUS;
    astValues[1].type = SR_ENUM_T;
    /* see org-openroadm-common-types.yang line 143                             */
    if (FILE_TRANSFER_SUCCESS == eStatus) {
        astValues[1].data.enum_val = "Successful";
    } else if (FILE_TRANSFER_FAIL == eStatus) {
        astValues[1].data.enum_val = "Failed";
    } else {

    }

    rc = sr_notif_send(pstSess,
                       FILE_XPATH_TRANSFER_EVENT,
                       astValues,
                       WR_CNT,
                       0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s", sr_strerror(rc));
        goto cleanup;
    }
#undef WR_CNT
cleanup:
    return rc;
}

static int
__RpcFileTransferCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                    sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    char *pcTmp = NULL;
    char acProtocal[16] = {0};
    unsigned int uiLen = 0;

    LOG_WRITE(EN_LOG_NOTICE, "Executing  action: %s", input[0].data.string_val);
    pcTmp = strchr(input[2].data.string_val, ':');
    if (NULL == pcTmp) {
        rc = SR_ERR_INVAL_ARG;
        goto END_LABEL;
    }

    uiLen = (pcTmp - input[2].data.string_val);
    uiLen = uiLen < sizeof(acProtocal) ? uiLen + 1 : sizeof(acProtocal);

    snprintf(acProtocal, uiLen, "%s", input[2].data.string_val);

    if (0 == strcmp(acProtocal, "scp")) {
        rc = __ScpTransferFile(input[0].data.string_val,
                               input[1].data.string_val,
                               input[2].data.string_val);
    } else if (0 == strcmp(acProtocal, "sftp")) {
        rc = __SftpTransferFile(input[0].data.string_val,
                                input[1].data.string_val,
                                input[2].data.string_val);
    } else {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Invalid protocol: %s", acProtocal);
    }


END_LABEL:
    if (SR_ERR_OK == rc) {
        DSC_RpcCreateOutput(RPC_XPATH_FILE_TRANSFER, RPC_STATUS_SUCCESS, "File transfer ok", output, output_cnt);
        __SendFileTransferNotice(input[1].data.string_val, FILE_TRANSFER_SUCCESS);
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_FILE_TRANSFER, RPC_STATUS_FAILED, "File transfer error", output, output_cnt);
        __SendFileTransferNotice(input[1].data.string_val, FILE_TRANSFER_FAIL);
    }

    return SR_ERR_OK;
}

static int
__RpcShowFileCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
#define RPC_BASE_RETURN_NUM 2
#define RPC_MESSAGE_STR_SUCC "get file info successful!"
#define RPC_MESSAGE_STR_FAIL "get file info failed!"

    int i = 0, rc = SR_ERR_OK;
    char *pcInnerPath = NULL;
    FILE_ALL_INFO_ST stAllFileInfo;
    char acXpath[OPLK_PATH_MAX_LEN] = {'\0'};
    sr_val_t *pstSrValue = NULL;
    int iCount = 0;


    LOG_WRITE(EN_LOG_NOTICE, "Executing show_file: %s", input[0].data.string_val);

    pcInnerPath = FILE_GetInnerPath(input[0].data.string_val);
    if (NULL == pcInnerPath) {
        LOG_WRITE(EN_LOG_ERROR, "error get inner path failed : %s", input[0].data.string_val);
        goto cleanup;
    }

    memset(&stAllFileInfo, 0, sizeof(stAllFileInfo));
    rc = FILE_ReadFileList(pcInnerPath, &stAllFileInfo);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error read file failed : %s", pcInnerPath);
        goto cleanup;
    }

    rc = sr_new_values(RPC_BASE_RETURN_NUM + (stAllFileInfo.ulNum * SHOW_FILE_ATTR_NUM), &pstSrValue);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "sr_new_values error %s", sr_strerror(rc));
        goto cleanup;
    }

    /* set rpc status and message                                               */
    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, "%s/status", RPC_XPATH_SHOW_FILE);
    rc = sr_val_set_xpath(&pstSrValue[0], acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }

    rc = sr_val_set_str_data(&pstSrValue[0], SR_ENUM_T, RPC_STATUS_SUCCESS);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
        goto cleanup;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, "%s/status-message", RPC_XPATH_SHOW_FILE);
    rc = sr_val_set_xpath(&pstSrValue[1], acXpath);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acXpath, sr_strerror(rc));
        goto cleanup;
    }
    rc = sr_val_set_str_data(&pstSrValue[1], SR_STRING_T, RPC_MESSAGE_STR_SUCC);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
        goto cleanup;
    }

    iCount = SHOW_FILE_ATTR_START_INDEX;
    for (i = 0; i < stAllFileInfo.ulNum; i++) {
        /* set file name                                                        */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, RPC_XPATH_SHOW_OUTPUT_FILE_NAME, stAllFileInfo.astFileEntryInfo[i].acFileName);
        rc = sr_val_set_xpath(&pstSrValue[iCount], acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acXpath, sr_strerror(rc));
            goto cleanup;
        }
        pstSrValue[iCount].type = SR_STRING_T;
        rc = sr_val_set_str_data(&pstSrValue[iCount], SR_STRING_T, stAllFileInfo.astFileEntryInfo[i].acFileName);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
            goto cleanup;
        }
        iCount++;

        /* set file size                                                        */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, RPC_XPATH_SHOW_OUTPUT_FILE_SIZE, stAllFileInfo.astFileEntryInfo[i].acFileName);
        rc = sr_val_set_xpath(&pstSrValue[iCount], acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acXpath, sr_strerror(rc));
            goto cleanup;
        }
        pstSrValue[iCount].type = SR_UINT64_T;
        pstSrValue[iCount].data.uint64_val = stAllFileInfo.astFileEntryInfo[i].ullFileSize;
        iCount++;

        /* set modify date                                                      */
        memset(acXpath, 0, sizeof(acXpath));
        snprintf(acXpath, OPLK_PATH_MAX_LEN, RPC_XPATH_SHOW_OUTPUT_FILE_MODIFY_DATE, stAllFileInfo.astFileEntryInfo[i].acFileName);
        rc = sr_val_set_xpath(&pstSrValue[iCount], acXpath);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_xpath: %s, %s", acXpath, sr_strerror(rc));
            goto cleanup;
        }
        pstSrValue[iCount].type = SR_STRING_T;
        rc = sr_val_set_str_data(&pstSrValue[iCount], SR_STRING_T, stAllFileInfo.astFileEntryInfo[i].acFileModiyTime);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_val_set_str_data: %s", sr_strerror(rc));
            goto cleanup;
        }
        iCount++;
    }

    *output = pstSrValue;
    *output_cnt = RPC_BASE_RETURN_NUM + (stAllFileInfo.ulNum * SHOW_FILE_ATTR_NUM);

    return SR_ERR_OK;

cleanup:

    sr_free_values(pstSrValue, RPC_BASE_RETURN_NUM + (stAllFileInfo.ulNum * SHOW_FILE_ATTR_NUM));
    DSC_RpcCreateOutput(RPC_XPATH_SHOW_FILE, RPC_STATUS_FAILED, RPC_MESSAGE_STR_FAIL, output, output_cnt);

    return SR_ERR_OK;
}

static int
__RpcDelFileCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
               sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    struct stat stFileStat;
    char acCmdLine[OPLK_PATH_MAX_LEN] = {0};
    char acPathAndName[OPLK_FILE_PATH_LEN + FILE_NAME_LEN_MAX] = {0};
    char acName[FILE_NAME_LEN_MAX] = {0};
    char *pcPath = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "Executing del_file: %s", input[0].data.string_val);

    memset(acPathAndName, 0, sizeof(acPathAndName));
    memset(acName, 0, sizeof(acName));

    pcPath = FILE_GetInnerPathAndFileName(input[0].data.string_val, acName);
    if (NULL == pcPath) {
        DSC_RpcCreateOutput(RPC_XPATH_DEL_FILE, RPC_STATUS_FAILED, "File delete error, path error", output, output_cnt);
        return SR_ERR_OK;
    }

    strcpy(acPathAndName, pcPath);
    strcat(acPathAndName, acName);

    rc = stat(acPathAndName, &stFileStat);
    if (SR_ERR_OK == rc) {
        sprintf(acCmdLine, "rm -rf  %s 1>/dev/null 2>&1", acPathAndName);
        rc = system(acCmdLine);
        DSC_RpcCreateOutput(RPC_XPATH_DEL_FILE, RPC_STATUS_SUCCESS, "File delete ok", output, output_cnt);
    } else {
        DSC_RpcCreateOutput(RPC_XPATH_DEL_FILE, RPC_STATUS_FAILED, "File delete error, not exist", output, output_cnt);
    }

    return SR_ERR_OK;
}

static int
__GetVersionInfo(char *pcVersion)
{
    CMfg stBoardModMfg;
    int rc = OPLK_OK;

    if (NULL == pcVersion) {
        LOG_WRITE(EN_LOG_ERROR, "__GetVersionInfo input null");
        return OPLK_ERR;
    }

    memset(&stBoardModMfg, 0, sizeof(stBoardModMfg));
    rc = BOARD_GetSccMfg(&stBoardModMfg);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error BOARD_GetSccMfg execute failed!");
        return OPLK_ERR;
    }

    strcpy(pcVersion, stBoardModMfg.acSubBdlVer);

    return rc;
}

static int
__GetSysDateTime(char *pcDateTime)
{
    time_t nSeconds;
    struct tm *pstTime;

    if (NULL == pcDateTime) {
        LOG_WRITE(EN_LOG_ERROR, "__GetSysDateTime para point null!");
        return OPLK_ERR;
    }

    time(&nSeconds);
    pstTime = localtime(&nSeconds);

    sprintf(pcDateTime, "%04d%02d%02d-%02d%02d%02d",
            pstTime->tm_year + 1900, pstTime->tm_mon + 1, pstTime->tm_mday,
            pstTime->tm_hour, pstTime->tm_min, pstTime->tm_sec);

    return OPLK_OK;
}

static int
__SendDbBackupNotice(int32_t iFlag)
{
#define WR_CNT 1

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = DB_XPATH_BACKUP_NOTICE_STATUS;
    astValues[0].type = SR_ENUM_T;
    if (SR_ERR_OK == iFlag) {
        astValues[0].data.string_val = "Successful";
    } else {
        astValues[0].data.string_val = "Failed";
    }

    rc = sr_notif_send(pstSess,
                       DB_XPATH_BACKUP_NOTICE,
                       astValues,
                       WR_CNT, 0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s", sr_strerror(rc));
        goto cleanup;
    }
#undef WR_CNT
cleanup:
    return rc;
}

#define EXEC_SYSTEM_FUNC(Cmd, args ...)                                     \
    do {                                                                    \
        memset(acBuffer, 0, sizeof(acBuffer));                              \
        snprintf(acBuffer, sizeof(acBuffer), Cmd, ## args);                 \
        rc = UTIL_System(acBuffer);                                         \
        if (rc != OPLK_OK) {                                                \
            LOG_WRITE(EN_LOG_ERROR, "Error by system %s", acBuffer);        \
            goto cleanup;                                                   \
        }                                                                   \
    } while (0)

static int
__RpcDbBackupCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    char acBuffer[DEF_BUF_SIZE_512] = {0};
    char acFileName[FILE_NAME_LEN_MAX] = {0};
    char acBackupFileName[FILE_NAME_LEN_MAX] = {0};
    uint32_t ulFileNum = 0;
    ST_PERSIST *pstCfgStatic = NULL;
    char acNodeId[NODE_ID_STR_LEN_MAX] = {0};
    char acVersion[VERSION_STR_LEN_MAX] = {0};
    char acTime[DATE_STR_LEN_MAX] = {0};
    char *pcInnerPath = NULL;
    size_t nCnt = 0;
    char acResp[DEF_BUF_SIZE_32] = {0};

    LOG_WRITE(EN_LOG_NOTICE, "Executing  action: %s", input[0].data.string_val);

    /* 1.check the file count in /home/oplink/datastore1.                       */
    pcInnerPath = FILE_GetInnerPath("/db-backups");
    rc = FILE_GetFileCount(pcInnerPath, &ulFileNum);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error FILE_GetFileCount : %s", pcInnerPath);
        goto cleanup;
    }

    if (FILE_NUM_MAX == ulFileNum) {
        LOG_WRITE(EN_LOG_ERROR, "Error file num is to max.");
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    /* OC2A-496 'show db-backup' stack (backupfile count limit)                 */
    /*          ONLY support 10 files.                                          */
    UTIL_ExecuteCmd("ls -l /home/oplink/datastore1/*.db | wc -l", acResp, sizeof(acResp), 0);
    if (atoi(acResp) >= DSC_FILE_NUM_MAX) {
        LOG_WRITE(EN_LOG_ERROR, "Error db file num: %d is out of range: %d", atoi(acResp), DSC_FILE_NUM_MAX);
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    /* 2.gen the full db filename.                                              */
    memset(acNodeId, 0, sizeof(acNodeId));
    memset(acVersion, 0, sizeof(acVersion));
    memset(acTime, 0, sizeof(acTime));
    rc = Operationd_GetNodeId(acNodeId);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error Operationd_GetNodeId failed!");
        return OPLK_ERR;
    }

    rc = __GetVersionInfo(acVersion);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __GetVersionInfo failed!");
        return OPLK_ERR;
    }

    rc = __GetSysDateTime(acTime);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __GetSysDateTime failed!");
        return OPLK_ERR;
    }

#if 0
    memset(acBackupFileName, 0, sizeof(acBackupFileName));
    rc = FILE_GenDbFileName(input[0].data.string_val, acNodeId, acVersion, acTime, acBackupFileName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error FILE_GenDbFileName input filename : %s", input[0].data.string_val);
        goto cleanup;
    }
#endif
    memset(acBackupFileName, 0, sizeof(acBackupFileName));
    snprintf(acBackupFileName, sizeof(acBackupFileName), "%s-%s.db", acNodeId, input[0].data.string_val);

    /* 3.get the cfg file info                                                  */
    pstCfgStatic = PERSIST_GetCfgStaticData(&nCnt);
    if (NULL == pstCfgStatic) {
        LOG_WRITE(EN_LOG_ERROR, "Error _PERSIST_GetCfgStaticData exec failed!");
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }

    /* 4.create the tmp dir for tar                                             */
    EXEC_SYSTEM_FUNC("rm -fr %s", DB_BACKUP_DIR);
    EXEC_SYSTEM_FUNC("mkdir %s", DB_BACKUP_DIR);

    /* 5.get cfg file from eeprom, and put them to the tmp dir.                 */
    for (int i = 0; i < nCnt; i++) {
        if (NULL == pstCfgStatic[i].pcYangName) {
            continue;
        }
        memset(acFileName, 0, sizeof(acFileName));
        snprintf(acFileName, sizeof(acFileName), "%s/%s.tar.bz2", DB_BACKUP_DIR, pstCfgStatic[i].pcYangName);

        LOG_WRITE(EN_LOG_DEBUG, "db read eeprom : %d----%s", pstCfgStatic[i].iAddrSlice, acFileName);

        rc = BOARD_GetChassisCfg(pstCfgStatic[i].iAddrSlice, acFileName);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by BOARD_GetChassisCfg: %d, %s", pstCfgStatic[i].iAddrSlice, acFileName);
            continue;
        }
    }

    /* 6.cd to the tmp dir and tar these cfg file to a total file               */
    EXEC_SYSTEM_FUNC("cd %s; tar -jcvf %s/%s ./*.tar.bz2", DB_BACKUP_DIR, DB_SAVE_DIR, acBackupFileName);
cleanup:

    if (SR_ERR_OK == rc) {
        DSC_RpcCreateOutput(DB_XPATH_BACKUP, RPC_STATUS_SUCCESS, "DB backup ok", output, output_cnt);
    } else {
        DSC_RpcCreateOutput(DB_XPATH_BACKUP, RPC_STATUS_FAILED, "DB backup error", output, output_cnt);
    }

    __SendDbBackupNotice(rc);

    EXEC_SYSTEM_FUNC("rm -rf %s", DB_BACKUP_DIR);

    return SR_ERR_OK;
}

static int
__SendDbRestoreNotice(int32_t iFlag)
{
#define WR_CNT 1

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = DB_XPATH_RESTORE_NOTICE_STATUS;
    astValues[0].type = SR_ENUM_T;
    if (SR_ERR_OK == iFlag) {
        astValues[0].data.string_val = "Successful";
    } else {
        astValues[0].data.string_val = "Failed";
    }

    rc = sr_notif_send(pstSess,
                       DB_XPATH_RESTORE_NOTICE,
                       astValues,
                       WR_CNT, 0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s", sr_strerror(rc));
        goto cleanup;
    }
#undef WR_CNT
cleanup:
    return rc;
}

static int
__RpcDbRestoreCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                 sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    char acPathNameFileName[FILE_PATH_LEN_MAX + FILE_NAME_LEN_MAX] = {0};
    char acNodeId[NODE_ID_MAX_SIZE] = {0};
    char acBuffer[1024] = {0};
    bool bFileExist = false;
    char *pcInnerPath = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "Executing  action: %s, nodeid check : %d", input[0].data.string_val, input[1].data.bool_val);

    pcInnerPath = FILE_GetInnerPath("/db-backups");
    FILE_GenFullPathAndFileName(pcInnerPath, input[0].data.string_val, acPathNameFileName);
    bFileExist = FILE_CheckFileExist(acPathNameFileName);
    if (false == bFileExist) {
        LOG_WRITE(EN_LOG_ERROR, "error __RpcDbRestoreCb file is not exist!");
        goto cleanup;
    }

    /* when check nodeid is enable, need to check                               */
    memset(acNodeId, 0, sizeof(acNodeId));
    rc = Operationd_GetNodeId(acNodeId);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error Operationd_GetNodeId failed!");
        goto cleanup;
    }

    if (true == input[1].data.bool_val) {
        rc = FILE_CheckNodeIdToFileName(input[0].data.string_val, acNodeId);
        if (false == rc) {
            LOG_WRITE(EN_LOG_ERROR, "error nodeid is not matching filename is %s, %s", input[0].data.string_val, acNodeId);
            goto cleanup;
        }
    }

    EXEC_SYSTEM_FUNC("rm -f %s*.db", DB_RESTORE_DIR);
    /* EXEC_SYSTEM_FUNC("mkdir %s", DB_RESTORE_DIR);                            */
    EXEC_SYSTEM_FUNC("cp -f %s/%s %s", DB_SAVE_DIR, input[0].data.string_val, DB_RESTORE_DIR);

    __SendDbRestoreNotice(SR_ERR_OK);


    memset(m_acDbRestoreFile, 0, sizeof(m_acDbRestoreFile));
    strcpy(m_acDbRestoreFile, input[0].data.string_val);

    LOG_WRITE(EN_LOG_NOTICE, "db restore filename = %s", m_acDbRestoreFile);


    DSC_RpcCreateOutput(DB_XPATH_RESTORE, RPC_STATUS_SUCCESS, "DB restore ok", output, output_cnt);
    return SR_ERR_OK;

cleanup:
    __SendDbRestoreNotice(SR_ERR_OPERATION_FAILED);
    DSC_RpcCreateOutput(DB_XPATH_RESTORE, RPC_STATUS_FAILED, "DB restore error", output, output_cnt);
    return SR_ERR_OK;
}

static int
__SendDbActivateNotice(int32_t iFlag, DB_ACTIVATE_NOTICE_E enType)
{
#define WR_CNT 2

    sr_session_ctx_t *pstSess = DSC_GetRunningSession();
    int rc = SR_ERR_OK;
    sr_val_t astValues[WR_CNT];

    memset(astValues, 0, sizeof(astValues));
    astValues[0].xpath = DB_XPATH_ACTIVATE_NOTICE_STATUS;
    astValues[0].type = SR_ENUM_T;
    if (SR_ERR_OK == iFlag) {
        astValues[0].data.string_val = "Successful";
    } else {
        astValues[0].data.string_val = "Failed";
    }

    astValues[1].xpath = DB_XPATH_ACTIVATE_NOTICE_TYPE;
    astValues[1].type = SR_ENUM_T;
    if (DB_ACTIVATE == enType) {
        astValues[1].data.string_val = "activate";
    } else if (DB_COMMIT == enType) {
        astValues[1].data.string_val = "commit";
    } else {
        astValues[1].data.string_val = "cancel";
    }

    rc = sr_notif_send(pstSess,
                       DB_XPATH_ACTIVATE_NOTICE,
                       astValues,
                       WR_CNT, 0, 0);
    if ((rc != SR_ERR_NOT_FOUND) && (rc != SR_ERR_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "send notifcation failed: %s", sr_strerror(rc));
        goto cleanup;
    }
#undef WR_CNT
cleanup:
    return rc;
}

static int
__RpcDbActivateCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                  sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    int rc = SR_ERR_OK;
    int lHour = 0, lMin = 0, lSec = 0;
    bool bFileExist = false;
    uint32_t ulRollBackTimer = 0;
    char acPathNameFileName[FILE_PATH_LEN_MAX + FILE_NAME_LEN_MAX] = {0};

    LOG_WRITE(EN_LOG_NOTICE, "Executing  action: %s", input[0].data.string_val);

    memset(acPathNameFileName, 0, sizeof(acPathNameFileName));
    FILE_GenFullPathAndFileName(DB_RESTORE_DIR, m_acDbRestoreFile, acPathNameFileName);
    bFileExist = FILE_CheckFileExist(acPathNameFileName);
    if (false == bFileExist) {
        LOG_WRITE(EN_LOG_ERROR, "error __RpcDbActivateCb db file: %s is not exist", acPathNameFileName);
        goto cleanup;
    }

    /* Check parameter                                                          */
    rc = sscanf(input[0].data.string_val, "%d:%d:%d", &lHour, &lMin, &lSec);
    if (rc != 3) {
        LOG_WRITE(EN_LOG_ERROR, "Timestring format: '%s' error", input[0].data.string_val);
        goto cleanup;
    }

    if (lHour < 0 ||
        (lMin < 0 || lMin > 59) ||
        (lSec < 0 || lSec > 59)) {
        LOG_WRITE(EN_LOG_ERROR, "Time error format: '%s', %d:%d:%d",
                  input[0].data.string_val, lHour, lMin, lSec);
        goto cleanup;
    }

    ulRollBackTimer = lSec + lMin * 60 + lHour * 3600;
    rc = FILE_WriteDbState(ulRollBackTimer, DB_RESTORE_SWITCH);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error FILE_WriteDbState failed!");
        goto cleanup;
    }

    /* notification                                                             */
    __SendDbActivateNotice(rc, DB_ACTIVATE);

    /* reset cold                                                               */
    rc = IPC_SendResetRpc((sr_session_ctx_t *)private_ctx, RST_CHASSIS, RST_MODE_COLD);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Hard reset linux error %d", rc);
        goto cleanup;
    }

    DSC_RpcCreateOutput(DB_XPATH_ACTIVATE, RPC_STATUS_SUCCESS, "DB restore ok", output, output_cnt);
    return SR_ERR_OK;

cleanup:
    __SendDbActivateNotice(SR_ERR_OPERATION_FAILED, DB_ACTIVATE);
    DSC_RpcCreateOutput(DB_XPATH_ACTIVATE, RPC_STATUS_FAILED, "DB restore error", output, output_cnt);
    return SR_ERR_OK;
}

static int
__RpcDbCancelCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_ctx)
{
    DB_STATE_ST stDbState;
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, "Executing action accpet : %d", input[0].data.bool_val);

    memset(&stDbState, 0, sizeof(stDbState));
    FILE_ReadDbState(&stDbState);

    if (DB_RESTORE_PILOT != stDbState.ucDbRestoreState) {
        LOG_WRITE(EN_LOG_NOTICE, "__RpcDbCancelCb not support when not pilot status!");
        goto cleanup1;
    }

    FILE_WriteDbState(ulNULL, DB_RESTORE_COMMIT);

    if (true == input[0].data.bool_val) {
        /* cancel the alarm                                                     */
        alarm(0);

        /* save db to eeprom                                                    */
        rc = IPC_SaveEvent((sr_session_ctx_t *)private_ctx, 1);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Save current config error %d", rc);
            goto cleanup1;
        }
        __SendDbActivateNotice(rc, DB_COMMIT);
    } else {
        /* cold restart                                                         */
        rc = IPC_SendResetRpc((sr_session_ctx_t *)private_ctx, RST_CHASSIS, RST_MODE_COLD);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Hard reset linux error %d", rc);
            goto cleanup2;
        }

        __SendDbActivateNotice(rc, DB_CANCEL);
    }

    DSC_RpcCreateOutput(DB_XPATH_CANCEL, RPC_STATUS_SUCCESS, "db cancel ok", output, output_cnt);
    return SR_ERR_OK;

cleanup1:
    __SendDbActivateNotice(SR_ERR_OPERATION_FAILED, DB_COMMIT);
    DSC_RpcCreateOutput(DB_XPATH_CANCEL, RPC_STATUS_FAILED, "db cancel error", output, output_cnt);
    return SR_ERR_OK;

cleanup2:
    __SendDbActivateNotice(SR_ERR_OPERATION_FAILED, DB_CANCEL);
    DSC_RpcCreateOutput(DB_XPATH_CANCEL, RPC_STATUS_FAILED, "db cancel error", output, output_cnt);
    return SR_ERR_OK;
}

static void
__DbActivateTimerFunc()
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstRunningSess = DSC_GetRunningSession();

    FILE_WriteDbState(ulNULL, DB_RESTORE_COMMIT);

    LOG_WRITE(EN_LOG_NOTICE, "__DbActivateTimerFunc time over! the system will reset cold!");

    /* reset cold                                                               */
    rc = IPC_SendResetRpc((sr_session_ctx_t *)pstRunningSess, RST_CHASSIS, RST_MODE_COLD);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Hard reset linux error %d", rc);
    }

    __SendDbActivateNotice(rc, DB_CANCEL);

    return;
}

static int
__NodeIdChangeCb(sr_session_ctx_t *pstSess, uint32_t sub_id, const char *pcModuleName, const char *xpath, sr_event_t enEvent, uint32_t request_id, void *pvPrivateCtx)
{
    sr_change_iter_t *it = NULL;
    int rc = SR_ERR_OK;
    sr_change_oper_t oper;
    sr_val_t *pstOldValue = NULL;
    sr_val_t *pstNewValue = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "device Config Changes, event: %s", ev_to_str(enEvent));

    if (enEvent != SR_EV_DONE) {
        goto cleanup;
    }

    rc = sr_get_changes_iter(pstSess, OOD_XPATH_NODE_ID, &it);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get changes iter failed for xpath");
        goto cleanup;
    }

    while (SR_ERR_OK == sr_get_change_next(pstSess, it,
                                           &oper, &pstOldValue, &pstNewValue)) {

        /* assume only add or modify, so we always use the new value            */
        if (NULL == pstNewValue) {
            if (pstOldValue != NULL) {
                LOG_WRITE(EN_LOG_ERROR, "xpath: %s", pstOldValue->xpath);
            }
            LOG_WRITE(EN_LOG_ERROR, "Get device changes iter failed");
            goto cleanup;
        }

        if (sr_xpath_node_name_eq(pstNewValue->xpath, "node-id")) {
            strncpy(m_acNodeId, pstNewValue->data.string_val, sizeof(m_acNodeId) - 1);
            LOG_WRITE(EN_LOG_NOTICE, "node id change, the new value is : %s", m_acNodeId);
        }

        SR_SAFE_FREE_VAL(pstOldValue);
        SR_SAFE_FREE_VAL(pstNewValue);
    }

    rc = SR_ERR_OK;

cleanup:
    SR_SAFE_FREE_VAL(pstOldValue);
    SR_SAFE_FREE_VAL(pstNewValue);
    sr_free_change_iter(it);
    return rc;
}

static int
__NodeIdChangeSub(void)
{
    int rc = SR_ERR_OK;

    LOG_WRITE(EN_LOG_NOTICE, "__NodeIdChangeSub ++");

    rc = DSC_ChangeSubscribePassive(MODULE_OOD, OOD_XPATH_NODE_ID, __NodeIdChangeCb, NULL);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error conf subscribe %s, %s", OOD_XPATH_NODE_ID, sr_strerror(rc));
        goto cleanup;
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "__NodeIdChangeSub --");
    return rc;
}

static int
__InitGetNodeId(void)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *pstSess = NULL;

    pstSess = DSC_GetStartupSession();
    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(m_acNodeId, 0, sizeof(m_acNodeId));

    rc = get_string_item(pstSess, OOD_XPATH_NODE_ID, m_acNodeId, sizeof(m_acNodeId));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_string_item: %s, %s", OOD_XPATH_NODE_ID, sr_strerror(rc));
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "operationd init get nodeid: %s", m_acNodeId);
    return SR_ERR_OK;
cleanup:
    return rc;
}

int
Operationd_GetNodeId(char *pcNodeId)
{
    if (NULL == pcNodeId) {
        LOG_WRITE(EN_LOG_ERROR, "Operationd_GetNodeId input null");
        return OPLK_ERR;
    }

    strcpy(pcNodeId, m_acNodeId);

    return OPLK_OK;
}

int
File_RegisterRpc(sr_session_ctx_t *pstSess)
{
    int rc = SR_ERR_OK;
    bool bIsInstall = false;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    if (NULL == pstSess) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto cleanup;
    }

    for (int i = 0; i < (sizeof(m_astFileRpcSub) / sizeof(ST_RPC_SUB)); i++) {
        bIsInstall = true;
        if (m_astFileRpcSub[i].pcModuleName != NULL) {
            bIsInstall = DSC_IsSchemaInstalled(pstSess, m_astFileRpcSub[i].pcModuleName);
        }

        if (true == bIsInstall) {
            rc = DSC_RpcSubscribe(m_astFileRpcSub[i].pcXpath, m_astFileRpcSub[i].pFCallback, (void *)pstSess);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by sr_rpc_subscribe: %s: %s", m_astFileRpcSub[i].pcXpath, sr_strerror(rc));
                goto cleanup;
            }
            LOG_WRITE(EN_LOG_DEBUG, "Sub rpc: %s succ", m_astFileRpcSub[i].pcXpath);
        } else {
            LOG_WRITE(EN_LOG_DEBUG, "Not install: %s", m_astFileRpcSub[i].pcModuleName);
        }
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);
    return rc;
}

int
Db_Init(void)
{
    int rc = SR_ERR_OK;
    DB_STATE_ST stDbState;

    __InitGetNodeId();

    __NodeIdChangeSub();

    memset(m_acDbRestoreFile, 0, sizeof(m_acDbRestoreFile));
    memset(&stDbState, 0, sizeof(stDbState));
    FILE_ReadDbState(&stDbState);

    if (DB_RESTORE_PILOT == stDbState.ucDbRestoreState) {
        signal(SIGALRM, __DbActivateTimerFunc);
        alarm(stDbState.ulRollBackTimer);
        LOG_WRITE(EN_LOG_NOTICE, "__DbInit start at pilot mode! timer = %d", stDbState.ulRollBackTimer);
    } else {
        FILE_WriteDbState(ulNULL, DB_RESTORE_COMMIT);
        LOG_WRITE(EN_LOG_NOTICE, "__DbInit start at normal mode!");
    }

    return rc;
}
