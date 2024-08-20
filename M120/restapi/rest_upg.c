/******************************************************************************
*
* Copyright(C), 2018, OPLINK Tech.Co., Ltd
*
* Filename:      rest_msft.c
* Description:   only used for MSFT, parse requested json data and
*                      get business data for reponsing http/https request
* Author:        Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1]  Guangjun Guo   2018/03/17  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "rest_common.h"
#include "rest_msft.h"
#include "rest_log.h"
#include "rest_pluggable.h"
#include "util_inc.h"
#include "rest_upg.h"

int
REST_ParseSoftwareLoad(const cJSON *pstJson, REST_SOFTWARE_LOAD_ST *pstSoftwareLoad)
{
    REST_SOFTWARE_LOAD_ST stSoftwareLoad;
    JSON_OBJ_ST astJsonObj[] = {{STR_USER_ID, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSoftwareLoad.acUserId), sizeof(stSoftwareLoad.acUserId)},
                                {STR_PASSWORD, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSoftwareLoad.acPassword), sizeof(stSoftwareLoad.acPassword)},
                                {STR_SOURCE_FILE_NAME, EN_DATA_TYPE_STRING, TAKE_VOID_ADDRESS(stSoftwareLoad.acSrcFileName), sizeof(stSoftwareLoad.acSrcFileName)},
                                {NULL, EN_DATA_TYPE_MAX, NULL, 0}};

    memset(&stSoftwareLoad, 0, sizeof(stSoftwareLoad));
    if (OPLK_OK != JSON_GetObjItems(pstJson, astJsonObj)) {
        goto ERR_LABEL;
    }

    memcpy(pstSoftwareLoad, &stSoftwareLoad, sizeof(stSoftwareLoad));

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
REST_SoftwareDownload(REST_SOFTWARE_LOAD_ST *pstSoftwareLoad, bool bIsOneKeyUpgd)
{
    int rc = SR_ERR_OK;
    char acRemotePath[512] = {0};

    rc = UTIL_ChkStrSpecChr(pstSoftwareLoad->acUserId);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check SoftwareLoad UserId error!");
        goto ERR_LABEL;
    }

    /** msft requirement for scp password :

       MSFT rotates the password every month. The password are generated randomly with following requirement:
       The following character types must be present:
       o numeric character
       o lowercase alphabetical character
       o uppercase alphabetical character
       o special character - Special character consists of any of the following:
       ~!@#$%^&*_-+=`|\(){}[]:;<>,.?'/"

     **/
    rc = UTIL_CheckStrWithAllSpecChars(pstSoftwareLoad->acPassword);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check SoftwareLoad Password error!");
        goto ERR_LABEL;
    }

    rc = UTIL_ChkStrSpecChr(pstSoftwareLoad->acSrcFileName);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "check SoftwareLoad File Name error!");
        goto ERR_LABEL;
    }

    snprintf(acRemotePath, sizeof(acRemotePath), "scp:%s@%s&%s", pstSoftwareLoad->acUserId,
             pstSoftwareLoad->acSrcFileName, pstSoftwareLoad->acPassword);

    rc = DSC_TransferFile(bIsOneKeyUpgd ? FILE_TRANSFER_ONEKEY_UPGRADING : FILE_TRANSFER_DOWNLOAD, "/sw-image/FirmwareBundle", acRemotePath);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "transfer file failed.");
        goto ERR_LABEL;
    }

    return SR_ERR_OK;

ERR_LABEL:
    return rc;
}

int
REST_SoftwareActivate()
{
    int rc = SR_ERR_OK;
    char acStatus[256] = {0};

    if (OPLK_OK != UTIL_CopyFile("/tmp/download/FirmwareBundle", STR_PATH_TMP_FIRMWAREBUNDLE)) {
        rc = SR_ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    rc = DSC_SwUpgrade(STR_PATH_TMP_FIRMWAREBUNDLE, acStatus);
    if (SR_ERR_OK != rc) {
        goto ERR_LABEL;
    }

    return SR_ERR_OK;

ERR_LABEL:
    return rc;
}

