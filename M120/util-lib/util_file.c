/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_file.c
* Description:   file operation
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>

#include <unistd.h>
#include <dirent.h>
#include "util_file.h"
#include "cJSON.h"
#include "fcntl.h"

/***FUNC+******************************************************************************************/
/* Name   : UTIL_GetFileLen                                                                       */
/* Descrp : Gets the length of the given file                                                     */
/* Input  : pcFileName-- pointer to the source file name                                          */
/* Output : None                                                                                  */
/* Return : The length of the given file                                                          */
/***FUNC-******************************************************************************************/
int32_t
UTIL_GetFileLen(const char *pcFileName)
{
    FILE *pfFile = NULL;
    struct stat stFilestat;
    int32_t lFileLen = 0;

    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }
    if (NULL == (pfFile = fopen(pcFileName, "r"))) {
        LOG_WRITE(EN_LOG_NOTICE, "NULL == fopen");
        goto ERR_LABEL;
    }

    if (fstat(fileno(pfFile), &stFilestat) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "fstat < 0");
        goto ERR_LABEL;
    }

    /* int32_t max file len 2G, st_size type long */
    lFileLen = stFilestat.st_size;

    DEF_SAFE_FCLOSE(pfFile);
    return lFileLen;

ERR_LABEL:

    DEF_SAFE_FCLOSE(pfFile);
    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_MallocFromFile                                                                   */
/* Descrp : Gets the data from the given file                                                     */
/* Input  : pcFileName-- pointer to the source file name                                          */
/* Output : pulFileLen-- pointer to the file length which will be updated                         */
/* Return : Pointer to the given file data buffer                                                 */
/***FUNC-******************************************************************************************/
uint8_t *
UTIL_MallocFromFile(char *pcFileName, uint32_t *pulFileLen)
{
    uint32_t ulFileLen = 0;
    uint8_t *pucBuf = NULL;
    FILE *pfFile = NULL;


    if ((NULL == pcFileName) ||
        (NULL == pulFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "input is null");
        goto ERR_LABEL;
    }

    /* Gets file length for malloc RAM                                          */
    ulFileLen = UTIL_GetFileLen(pcFileName);
    if (0 == ulFileLen) {
        LOG_WRITE(EN_LOG_ERROR, "ulFileLen is zero");
        goto ERR_LABEL;
    }

    /* Malloc RAM                                                               */
    if (NULL == (pucBuf = (uint8_t *)malloc(ulFileLen))) {
        LOG_WRITE(EN_LOG_ERROR, "malloc is error");
        goto ERR_LABEL;
    }
    memset(pucBuf, 0, ulFileLen);

    /* Copies the file into RAM buffer                                          */
    if (NULL == (pfFile = fopen(pcFileName, "r"))) {
        LOG_WRITE(EN_LOG_ERROR, "fopen is error");
        goto ERR_LABEL;
    }

    /* Copies file to RAM                                                       */
    if (ulFileLen != fread(pucBuf, sizeof(uint8_t), ulFileLen, pfFile)) {
        LOG_WRITE(EN_LOG_ERROR, "fread is error");
        goto ERR_LABEL;
    }

    /* File is no use, close it now                                             */
    DEF_SAFE_FCLOSE(pfFile);

    *pulFileLen = ulFileLen;

    return pucBuf;

ERR_LABEL:
    DEF_SAFE_FCLOSE(pfFile);
    DEF_SAFE_FREE(pucBuf);

    return NULL;
}

int
UTIL_CreatePidFile(const char *pcPidFile, int *piFd)
{
    char acStr[DEF_BUF_SIZE_256] = {0};
    int iRet = OPLK_ERR;

    if (NULL == pcPidFile || NULL == piFd) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    /* open PID file */
    *piFd = open(pcPidFile, O_RDWR | O_CREAT, 0640);
    if (*piFd < 0) {
        LOG_WRITE(EN_LOG_ERROR, "Unable to open PID file '%s': %d.", pcPidFile, errno);
        goto ERR_LABEL;
    }

    /* acquire lock on the PID file */
    if (lockf(*piFd, F_TLOCK, 0) < 0) {
        if (EACCES == errno || EAGAIN == errno) {
            LOG_WRITE(EN_LOG_ERROR, "Another instance of daemon is running, unable to start.");
            exit(1);
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Unable to lock sysrepo PID file '%s': %d.", pcPidFile, errno);
        }
        goto ERR_LABEL;
    }

    /* write PID into the PID file */
    snprintf(acStr, sizeof(acStr), "%d\n", getpid());
    iRet = write(*piFd, acStr, strlen(acStr));
    if (0 > iRet) {
        LOG_WRITE(EN_LOG_ERROR, "Unable to write into PID file '%s': %d.", pcPidFile, errno);
        goto ERR_LABEL;
    }

    /* do not close nor unlock the PID file, keep it open while the daemon is alive */
    return OPLK_OK;

ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_DelPidFile(const char *pcPidFile, int iFd)
{
    char acBuf[DEF_BUF_SIZE_128];
    int iRet = OPLK_ERR;

    if (NULL == pcPidFile) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    unlink(pcPidFile);

    DEF_SAFE_CLOSE(iFd);

    memset(acBuf, 0, sizeof(acBuf));
    sprintf(acBuf, "rm -f %s", pcPidFile);

    iRet = UTIL_System(acBuf);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "system cmd error:%s", acBuf);
        goto ERR_LABEL;
    }


    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

/***FUNC+******************************************************************************************/
/* Name   : UTIL_ValidateJsonFile                                                                 */
/* Descrp : Validate json file format                                                             */
/* Input  : pcFileName -- A pointer of filename                                                   */
/* Return : The operation is done or not                                                          */
/*            This parameter can be one of the following values:                                  */
/*              @arg OPLK_OK :                                                                    */
/*              @arg OPLK_ERR:                                                                    */
/***FUNC-******************************************************************************************/
int
UTIL_ValidateJsonFile(char *pcFileName)
{
    uint8_t *pucBuffer = NULL;
    uint32_t ulFileLen = 0;
    cJSON *pstJson = NULL;

    /* validate input parameter */
    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        goto ERR_LABEL;
    }

    /* Get buffer from file */
    pucBuffer = UTIL_MallocFromFile(pcFileName, &ulFileLen);
    if (NULL == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "Malloc error: %s", pcFileName);
        goto ERR_LABEL;
    }

    /* validate file content */
    pstJson = cJSON_Parse((const char *)pucBuffer);
    if (NULL == pstJson) {
        LOG_WRITE(EN_LOG_ERROR, "Validate json: %s error", pcFileName);
        goto ERR_LABEL;
    }

    if (NULL != pstJson) {
        cJSON_Delete(pstJson);
        pstJson = NULL;
    }

    DEF_SAFE_FREE(pucBuffer);
    return OPLK_OK;

ERR_LABEL:
    if (NULL != pstJson) {
        cJSON_Delete(pstJson);
        pstJson = NULL;
    }
    DEF_SAFE_FREE(pucBuffer);
    return OPLK_ERR;
}

int
UTIL_GetFileName(char *pcPath, char *pcName, size_t nLen)
{
    DIR *pstDirFd = NULL;
    struct dirent *pstDirInfo = NULL;
    int rc = OPLK_ERR;

    if (NULL == (pstDirFd = opendir(pcPath))) {
        LOG_WRITE(EN_LOG_ERROR, "open path : %s failed!", pcPath);
        return rc;
    }

    while ((pstDirInfo = readdir(pstDirFd)) != NULL) {
        if (strcmp(pstDirInfo->d_name, ".") == 0 || strcmp(pstDirInfo->d_name, "..") == 0) { /* current dir OR parrent dir */
            continue;
        } else if (pstDirInfo->d_type == 8) { /* file */
            rc = OPLK_OK;
            strncpy(pcName, pstDirInfo->d_name, nLen);
        } else if (pstDirInfo->d_type == 10) { /* link file */
        } else if (pstDirInfo->d_type == 4) { /* dir */
        }
    }

    closedir(pstDirFd);

    return rc;
}

int
UTIL_CopyFile(const char *pcSrcFilePath, const char *pcDestFilePath)
{
    char *pcShellCmd = NULL;
    int rc = OPLK_OK;

    if (NULL == pcSrcFilePath || NULL == pcSrcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "input null");
        return OPLK_ERR;
    }

    rc = asprintf(&pcShellCmd, "cp -f %s %s >/dev/null 2>&1", pcSrcFilePath, pcDestFilePath);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "asprintf error");
        return OPLK_ERR;
    }

    rc = UTIL_System(pcShellCmd);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_System failed : %s", pcShellCmd);
    }

    DEF_SAFE_FREE(pcShellCmd);

    return rc;
}

int
UTIL_DeleteFile(const char *pcFile)
{
    char acCmdLine[DEF_BUF_SIZE_256] = {0};

    if ((NULL == pcFile) || (0 == strlen(pcFile))) {
        goto ERR_LABEL;
    }

    if (access(pcFile, F_OK) != 0) {
        LOG_WRITE(EN_LOG_ERROR, "File: %s not exist!", pcFile);
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "rm -f %s >/dev/null 2>&1", pcFile);
    if (OPLK_OK != UTIL_System(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_CreateDir(const char *pcDir)
{
    char acCmdLine[DEF_BUF_SIZE_256] = {0};

    if (NULL == pcDir) {
        goto ERR_LABEL;
    }

    memset(acCmdLine, 0, sizeof(acCmdLine));
    snprintf(acCmdLine, sizeof(acCmdLine), "mkdir -p %s >/dev/null 2>&1", pcDir);

    if (OPLK_OK != UTIL_System(acCmdLine)) {
        LOG_WRITE(EN_LOG_ERROR, "execute %s failed", acCmdLine);
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

int
UTIL_LockFile(const char *pcFileName, const char *pcFunc)
{
    int iFd = -1;
    struct flock stFilelock;

    if (NULL == pcFileName || NULL == pcFunc) {
        goto ERR_LABEL;
    }

    /* If NOT exist, create a new ONE */
    iFd = open(pcFileName, O_RDWR | O_CREAT, 0644);
    if (iFd < 0) {
        goto ERR_LABEL;
    }

    /* Lock the password file before updating */
    stFilelock.l_type = F_WRLCK;
    stFilelock.l_whence = SEEK_SET;
    stFilelock.l_start = 0;
    stFilelock.l_len = 0;
    if (fcntl(iFd, F_SETLK, &stFilelock) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "%s lock '%s' error: %s", pcFunc, pcFileName, strerror(errno));
        goto ERR_LABEL;
    }

    DEF_SAFE_CLOSE(iFd);
    return OPLK_OK;
ERR_LABEL:
    DEF_SAFE_CLOSE(iFd);
    return OPLK_ERR;
}

int
UTIL_UnlockFile(const char *pcFileName, const char *pcFunc)
{
    int iFd = -1;
    struct flock stFilelock;

    if (NULL == pcFileName || NULL == pcFunc) {
        goto ERR_LABEL;
    }

    /* If NOT exist, create a new ONE */
    iFd = open(pcFileName, O_RDWR | O_CREAT, 0644);
    if (iFd < 0) {
        goto ERR_LABEL;
    }

    /* Lock the password file before updating */
    stFilelock.l_type = F_UNLCK;
    stFilelock.l_whence = SEEK_SET;
    stFilelock.l_start = 0;
    stFilelock.l_len = 0;
    if (fcntl(iFd, F_SETLK, &stFilelock) < 0) {
        LOG_WRITE(EN_LOG_ERROR, "%s unlock '%s' error: %s", pcFunc, pcFileName, strerror(errno));
        goto ERR_LABEL;
    }

    DEF_SAFE_CLOSE(iFd);
    return OPLK_OK;
ERR_LABEL:
    DEF_SAFE_CLOSE(iFd);
    return OPLK_ERR;
}

