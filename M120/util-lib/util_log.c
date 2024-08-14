/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_log.c
* Description:   log
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "util_log.h"
#include "util_queue.h"
#include "util_file.h"
#include "util_log_hash.h"
static bool m_bHashTableEnable = false;

#define DEF_LOG_HEADER_KEYWORD                   ("oplink")

/* unix send log path */
#define DEF_HOST_RECV_LOG_PATH                   ("/var/tmp/logRecvHostSocket")
#define DEF_CLENT_SEND_LOG_PATH                  ("/var/tmp/logSend%sSocket")

/*define abnormal log msg for log host*/
#define DEF_RCEV_MSG_CHK_ERR                     ("Recv log message check sum error")
#define DEF_RCEV_MSG_LENGTH_ERR                  ("Recv log message length error")

/*define log serverity string*/
#define DEF_LOG_SERVERITY_EMERGENCY              ("Emergency")
#define DEF_LOG_SERVERITY_ALERT                  ("Alert")
#define DEF_LOG_SERVERITY_CRITICAL               ("Critical")
#define DEF_LOG_SERVERITY_ERROR                  ("Error")
#define DEF_LOG_SERVERITY_WARNING                ("Warning")
#define DEF_LOG_SERVERITY_NOTICE                 ("Notice")
#define DEF_LOG_SERVERITY_TRACE                  ("Trace")
#define DEF_LOG_SERVERITY_DEBUG                  ("Debug")

/*log tar file */
#define DEF_OPLK_LOG_TAR_FILE                    "/var/log/internal-log.tar.gz"
#define DEF_USER_LOG_TAR_FILE                    "/var/log/user-log.tar.gz"

/*internal log file */
#define DEF_LOG_ROOT_DIRECTORY                   "/var/log/"
#define DEF_OPLK_LOW_LOG_FILE                    "/var/log/internal-low.log"
#define DEF_OPLK_MID_LOG_FILE                    "/var/log/internal-middle.log"
#define DEF_OPLK_HIGH_LOG_FILE                   "/var/log/internal-high.log"
#define DEF_OPLK_SYSTEM_INFOR_FILE               "/var/log/system-infor.log"

/*user log file */
#define DEF_USER_LOG_FILE                        "/var/log/user-def.log"

/*system running status cmd*/
#define DEF_SYS_CMD_CAT_PROC                     ("cat /proc/meminfo")
#define DEF_SYS_CMD_NETSTAT                      ("netstat -an")
#define DEF_SYS_CMD_DF                           ("df -h")
#define DEF_SYS_CMD_IPCS                         ("ipcs -a")
#define DEF_SYS_CMD_PS                           ("ps aux")
#define DEF_SYS_CMD_PSTREE                       ("pstree")
#define DEF_SYS_CMD_TOP                          ("top -b -n2")

/* system running status size and log num */
#define DEF_SYS_INFO_SIZE                        (51200)
#define DEF_SYS_INFO_MAX_NUM                     (60)

/* compress log file to tar packet */
#define DEF_LOG_COMPRESS                         "tar --warning=no-file-changed -czPf %s %s*"

/*rm -rf log */
#define DEF_RM_OPLK_LOG                          ("rm -rf "DEF_OPLK_LOG_TAR_FILE)
#define DEF_RM_USER_LOG                          ("rm -rf "DEF_USER_LOG_TAR_FILE)

/* decompress tar packet to log file */
#define DEF_OPLK_LOG_DECOMPRESS                  ("tar -xzPf "DEF_OPLK_LOG_TAR_FILE " --exclude syslog")
#define DEF_USER_LOG_DECOMPRESS                  ("tar -xzPf "DEF_USER_LOG_TAR_FILE)

/*save log to and resume from msata */
#define DEF_MSATA_SAVE_OPLK_LOG_PATH             "/home/oplink/log/internal-log.tar.gz"
#define DEF_MSATA_SAVE_USER_LOG_PATH             "/home/oplink/log/user-log.tar.gz"

#define DEF_MSATA_SAVE_OPLK_LOG_CMD              "cp -rf "DEF_OPLK_LOG_TAR_FILE " "DEF_MSATA_SAVE_OPLK_LOG_PATH
#define DEF_MSATA_SAVE_USER_LOG_CMD              "cp -rf "DEF_USER_LOG_TAR_FILE " "DEF_MSATA_SAVE_USER_LOG_PATH

#define DEF_MSATA_RESUME_OPLK_LOG_CMD            "cp -rf "DEF_MSATA_SAVE_OPLK_LOG_PATH " "DEF_OPLK_LOG_TAR_FILE
#define DEF_MSATA_RESUME_USER_LOG_CMD            "cp -rf "DEF_MSATA_SAVE_USER_LOG_PATH " "DEF_USER_LOG_TAR_FILE


#define DEF_AUTO_SAVE_LOG_TIME                   (7200)  /*2 hours, unit: second, 2*60*60*/
#define DEF_CHECK_SYSTEM_INFOR_TIME              (900)   /*15 mins, unit: second, 15*60*/
#define DEF_LOG_FLASH_SIZE                       (10485760)   /*10 MB, 10 *1024*1024*/

#define DEF_KILLALL_SIGUSR1                      ("killall -SIGUSR1 %s")

/* LOG Bank */
typedef enum {
    EN_LOG_BANK_L = 0,
    EN_LOG_BANK_M,
    EN_LOG_BANK_H,

    EN_LOG_BANK_MAX

} EN_UTIL_LOG_BANK;

/*log file header for resume log file*/
typedef enum {
    EN_FILE_TYPE_TXT,
    EN_FILE_TYPE_TGZ,   /* tar and gzip */
} EN_UTIL_LOG_FILE_TYPE;

typedef struct {
    int iFd;
    uint8_t ucRecordSeverity;                       /* min severity for record log, the log severity more than ucRecordSeverity can be recorded */
    uint8_t ucLogEnable;                            /* the process is able to record log or not */
    uint8_t aucResv[2];
} ST_CLIENT_LOG_UNIX_SOCK;

typedef struct {
    uint32_t ulFileLen;
    EN_UTIL_LOG_FILE_TYPE enLogFileType;
} ST_UTIL_LOG_FILE_HEADER;

typedef struct {
    char *pcVarTarFile;
    char *pcMsataTarFile;
    char *pcResumeCmd;
    char *pcDecompressCmd;
    char *pcRmCmd;
} ST_RESUME_MSATA_LOG;


#define   DEF_LOG_NODE_NUM                    (256)

typedef struct {
    uint32_t ulMsgOid;                                                                                                  /* Oid,which is index of queue member.			   */
    uint32_t ulMsgLen;                                                                                                  /* length of  message.							   */
    char acMsgBuf[DEF_LOG_MSG_MAX_LEN];                                               /* buffer holds message.			                           */
} ST_LOG_QUEUE_MSG;  /* 520 Bytes */

static char m_aacProcName[][DEF_PROC_NAME_LEN] = {
    /* devlib */
    "hal",                 /* 0 + 1 */

    /* deamon */
    "csmd",                /* 1 + 14 */
    "authd",
    "timed",
    "cmptctrld",
    "persistd",
    "networkd",
    "recordlogd",
    "syslogalm",
    "almprovided",
    "autoctrld",
    "operationd",
    "performanced",
    "ocd",
    "pmd",
    "opsd",

    /* Agent */
    "restapi",             /* 15 + 6 */
    "restapi_8080",
    "restapi_8081",
    "restapi_8082",
    "restapi_8083",
    "snmpd",

    /* tools */
    "dbloader",        /* 21 + 3 */
    "auth_agent",
    "persist_reboot",

    /* CLI */
    "cli",             /* 24 + 2 */
    "mfg",

    /* monitord */
    "monitord",        /* 26 + 1*/
};


/* log Header */
static ST_OPLK_LOG_HEADER m_stHighLogHeader;
static ST_OPLK_LOG_HEADER m_stMidLogHeader;
static ST_OPLK_LOG_HEADER m_stLowLogHeader;
static ST_OPLK_LOG_HEADER m_stUserLogHeader;
static ST_OPLK_LOG_HEADER m_stSysInforHeader;

/* log file fp */
static FILE *m_fpHigLog = NULL;
static FILE *m_fpMidLog = NULL;
static FILE *m_fpLowLog = NULL;
static FILE *m_fpUserLog = NULL;
static FILE *m_fpSysInfor = NULL;

/*save log time */
static int64_t m_llSaveLogTime = 0;

/*the time for checking system running status   */
static int64_t m_llCheckSysTime = 0;

/*the time for checking system running status   */
static int8_t m_cQueueIndex = 0;

static EN_UTIL_LOG_OUTPUT m_enOplklogOutput = EN_OUTPUT_FILE;
static EN_UTIL_LOG_OUTPUT m_enUserlogOutput = EN_OUTPUT_FILE;

static EN_UTIL_LOG_SAVE_DEVICE m_enSaveDevice = EN_LOG_SAVE_MSATA;

static bool m_bExitHostLoop = false;

static ST_CLIENT_LOG_UNIX_SOCK m_stClientLogUnixSock;

static void __UTIL_SaveHostLog(void);

EN_UTIL_LOG_OUTPUT UTIL_GetHostOplkLogOutput(void);
EN_UTIL_LOG_OUTPUT UTIL_GetHostUserLogOutput(void);

static int32_t
__UTIL_GetProcId(void)
{
    char acProcName[DEF_PROC_NAME_LEN] = {0};
    uint32_t i = 0, ulCnt = 0;

    if (OPLK_OK != UTIL_GetProcName(acProcName, DEF_PROC_NAME_LEN)) {
        return OPLK_ERR;
    }

    ulCnt = sizeof(m_aacProcName) / sizeof(m_aacProcName[0]);

    for (i = 0; i < ulCnt; i++) {
        if (0 == strcmp(m_aacProcName[i], acProcName)) {
            break;
        }
    }

    if (i >= ulCnt) {
        return OPLK_ERR;
    }

    return i;
}

static char *
__UTIL_GetProcName(int iProcId, char *pcProcName, size_t nLen)
{
    uint32_t ulCnt = 0;

    if (NULL == pcProcName) {
        return NULL;
    }

    ulCnt = sizeof(m_aacProcName) / sizeof(m_aacProcName[0]);

    if (iProcId < 0 || iProcId >= ulCnt) {
        return NULL;
    }

    strncpy(pcProcName, m_aacProcName[iProcId], nLen - 1);
    return pcProcName;
}


static int
__UTIL_TarCmd(char *pcCmdBuf)
{
    int iStatus = -1;

    if (NULL == pcCmdBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pcCmdBuf");
        goto ERR_LABEL;
    }

    iStatus = system(pcCmdBuf);
    if (0 > iStatus) {
        LOG_WRITE(EN_LOG_ERROR, "system Status error, cmd %s", pcCmdBuf);
        goto ERR_LABEL;
    }

    if (0 == WIFEXITED(iStatus)) {
        LOG_WRITE(EN_LOG_ERROR, "exit status = [%d]", WEXITSTATUS(iStatus));
        goto ERR_LABEL;
    } else {
        /* 0:Successful termination,1:Some files differ  */
        if ((0 != WEXITSTATUS(iStatus)) && (1 != WEXITSTATUS(iStatus))) {
            LOG_WRITE(EN_LOG_ERROR, "run shell script fail:%s, script exit code: %d", strerror(errno), WEXITSTATUS(iStatus));
            LOG_WRITE(EN_LOG_ERROR, "shell: %s", pcCmdBuf);
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


static void
__UTIL_FflushLogFile(FILE *fp)
{
    if (fp != NULL) {
        fflush(fp);
    }
}

static void
__UTIL_SendSigUsr1(void)
{
    int iRet = OPLK_ERR;
    char acCmd[DEF_BUF_SIZE_64] = {0};

    snprintf(acCmd, DEF_BUF_SIZE_64, DEF_KILLALL_SIGUSR1, "persistd");
    iRet = UTIL_System(acCmd);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "cmd error:%s", acCmd);
    }

#if 0 /* persist_reboot not damon process */
    snprintf(acCmd, DEF_BUF_SIZE_64, DEF_KILLALL_SIGUSR1, "persist_reboot");
    iRet = UTIL_System(acCmd);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "cmd error:%s", acCmd);
    }
#endif

    return;
}

static int
__UTIL_InitLogHeader(ST_OPLK_LOG_HEADER *pstLogHeader)
{
    if (NULL == pstLogHeader) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstLogHeader");
        return OPLK_ERR;
    }

    memset(pstLogHeader, 0x00, sizeof(ST_OPLK_LOG_HEADER));
    snprintf(pstLogHeader->acKeyWord, DEF_LOG_HEADER_KEYWORD_LEN, "%s",
             DEF_LOG_HEADER_KEYWORD);
    pstLogHeader->ulStartIndex = 0;
    pstLogHeader->ulEndIndex = 0;
    /* pstLogHeader->ulHeaderCrc32 = CMN_CalcCRC32(0, (UCHAR *)pstLogHeader, LOG_HEADER_LEN4CRC); */
    return OPLK_OK;
}


static int
__UTIL_SyncGlobalLogHeader(char *pcFileName, ST_OPLK_LOG_HEADER *pstLogHeader,
                           FILE *fpFile)
{
    if (NULL == pcFileName || NULL == pstLogHeader || NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pcFileName || NULL == pstLogHeader || NULL == fpFile");
        return OPLK_ERR;
    }

    /* update the log header and log file pointer */
    if (strcmp(pcFileName, DEF_OPLK_LOW_LOG_FILE) == 0) {
        memcpy(&m_stLowLogHeader, pstLogHeader, sizeof(ST_OPLK_LOG_HEADER));
        snprintf(m_stLowLogHeader.acResv, DEF_LOG_HERADR_RESV_LEN, "%s",
                 DEF_OPLK_LOW_LOG_FILE);
        m_fpLowLog = fpFile;
    } else if (strcmp(pcFileName, DEF_OPLK_MID_LOG_FILE) == 0) {
        memcpy(&m_stMidLogHeader, pstLogHeader, sizeof(ST_OPLK_LOG_HEADER));
        snprintf(m_stMidLogHeader.acResv, DEF_LOG_HERADR_RESV_LEN, "%s",
                 DEF_OPLK_MID_LOG_FILE);
        m_fpMidLog = fpFile;
    } else if (strcmp(pcFileName, DEF_OPLK_HIGH_LOG_FILE) == 0) {
        memcpy(&m_stHighLogHeader, pstLogHeader, sizeof(ST_OPLK_LOG_HEADER));
        snprintf(m_stHighLogHeader.acResv, DEF_LOG_HERADR_RESV_LEN, "%s",
                 DEF_OPLK_HIGH_LOG_FILE);
        m_fpHigLog = fpFile;
    } else if (strcmp(pcFileName, DEF_USER_LOG_FILE) == 0) {
        memcpy(&m_stUserLogHeader, pstLogHeader, sizeof(ST_OPLK_LOG_HEADER));
        snprintf(m_stUserLogHeader.acResv, DEF_LOG_HERADR_RESV_LEN, "%s",
                 DEF_USER_LOG_FILE);
        m_fpUserLog = fpFile;
    } else if (strcmp(pcFileName, DEF_OPLK_SYSTEM_INFOR_FILE) == 0) {
        memcpy(&m_stSysInforHeader, pstLogHeader, sizeof(ST_OPLK_LOG_HEADER));
        snprintf(m_stSysInforHeader.acResv, DEF_LOG_HERADR_RESV_LEN, "%s",
                 DEF_OPLK_SYSTEM_INFOR_FILE);
        m_fpSysInfor = fpFile;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "Sync global log header error");
        return OPLK_ERR;
    }

    return OPLK_OK;
}

static void
__UTIL_UpdateLogHeaderIndex(ST_OPLK_LOG_HEADER *pstLogHeader,
                            uint32_t ulLogMaxNum)
{
    if (NULL == pstLogHeader) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstLogHeader");
        return;
    }

    pstLogHeader->ulEndIndex++;
    if (pstLogHeader->ulEndIndex >= ulLogMaxNum) {
        /* printf("Reach log limit %d, loop from beginning\n", pstLogHeader->iEndIndex); */
        pstLogHeader->ulEndIndex = 0;
    }

    if (pstLogHeader->ulEndIndex == pstLogHeader->ulStartIndex) {
        pstLogHeader->ulStartIndex++;
        if (pstLogHeader->ulStartIndex >= ulLogMaxNum) {
            pstLogHeader->ulStartIndex = 0;
        }
    }

    return;
}

static void
__UTIL_WriteLogHeader(FILE *fp, ST_OPLK_LOG_HEADER *pstLogHeader, uint32_t ulLogMaxNum)
{
    if (NULL == pstLogHeader || NULL == fp) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pstLogHeader || NULL == fp");
        return;
    }

    __UTIL_UpdateLogHeaderIndex(pstLogHeader, ulLogMaxNum);

    /* update the log header crc */
    /* pstLogHeader->ulHeaderCrc32 = CMN_CalcCRC32(0, (UCHAR *)(pstLogHeader), LOG_HEADER_LEN4CRC); */
    /* write the updated log header to log file */
    fseek(fp, 0, SEEK_SET);
    if (1 != fwrite(pstLogHeader, sizeof(ST_OPLK_LOG_HEADER), 1, fp)) {
        LOG_WRITE(EN_LOG_ERROR, "fwrite LogHeader error");
        return;
    }

    __UTIL_FflushLogFile(fp);

    return;
}

static int
__UTIL_RecreateLogFile(char *pcFileName)
{
    FILE *fpFile = NULL;
    ST_OPLK_LOG_HEADER stLogHeader;

    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcFileName");
        goto ERR_LABEL;
    }

    fpFile = fopen(pcFileName, "w");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "fopen %s error", pcFileName);
        goto ERR_LABEL;
    }

    __UTIL_InitLogHeader(&stLogHeader);

    if (fwrite(&stLogHeader, sizeof(stLogHeader), 1, fpFile) != 1) {
        LOG_WRITE(EN_LOG_ERROR, "fwrite %s error", pcFileName);
        goto ERR_LABEL;
    }

    __UTIL_FflushLogFile(fpFile);
    __UTIL_SyncGlobalLogHeader(pcFileName, &stLogHeader, fpFile);

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


static int
__UTIL_ResumeFlashLogFile(char *pcFileName)
{
    char *pcBuf = NULL;
    uint32_t ulLen = 0;
    ST_UTIL_LOG_FILE_HEADER stLogFileHeader;
    FILE *fp = NULL;
    int iRet = OPLK_ERR;

    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcFileName");
        goto ERR_LABEL;
    }

    memset(&stLogFileHeader, 0x00, sizeof(ST_UTIL_LOG_FILE_HEADER));
    /*Get log file header form Flash*/
    /* UTIL_ReadMtd(EN_MTD_NO_LOG_OPLK, 0, (uint8_t *)&stLogFileHeader, sizeof(ST_UTIL_LOG_FILE_HEADER));*/

    if ((stLogFileHeader.ulFileLen >= DEF_LOG_FLASH_SIZE - sizeof(stLogFileHeader))
        || (0 == stLogFileHeader.ulFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "FileLen %d sizeof stLogFileHeader %d", stLogFileHeader.ulFileLen, (uint32_t)sizeof(stLogFileHeader));
        goto END_LABEL;
    }

    pcBuf = (char *)malloc(stLogFileHeader.ulFileLen);
    if (pcBuf == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "pcBuf == NULL");
        goto ERR_LABEL;
    }

    /* resume the log file form Flash*/
    memset(pcBuf, 0, stLogFileHeader.ulFileLen);
    /* UTIL_ReadMtd(EN_MTD_NO_LOG_OPLK, sizeof(ST_OPLK_LOG_FILE_INFO), (uint8_t *)pcBuffer, stLogFileInfo.ulFileLen);*/
    ulLen = stLogFileHeader.ulFileLen;

    fp = fopen(pcFileName, "w+");        /* OPLK_TAR_LOG_FILE */
    if (fwrite(pcBuf, ulLen, 1, fp) != 1) {
        LOG_WRITE(EN_LOG_ERROR, "fwrite %s error", pcFileName);
        goto ERR_LABEL;
    }

    /*decompress the log*/
    __UTIL_FflushLogFile(fp);
    if (stLogFileHeader.enLogFileType == EN_FILE_TYPE_TGZ) {
        if (0 == strncmp(DEF_OPLK_LOG_TAR_FILE, pcFileName, strlen(DEF_OPLK_LOG_TAR_FILE))) {
            iRet = UTIL_System(DEF_OPLK_LOG_DECOMPRESS);
            if (OPLK_OK != iRet) {
                LOG_WRITE(EN_LOG_ERROR, "cmd error:%s", DEF_OPLK_LOG_DECOMPRESS);
                goto ERR_LABEL;
            }
        } else if (0 == strncmp(DEF_USER_LOG_TAR_FILE, pcFileName, strlen(DEF_USER_LOG_TAR_FILE))) {
            iRet = UTIL_System(DEF_USER_LOG_DECOMPRESS);
            if (OPLK_OK != iRet) {
                LOG_WRITE(EN_LOG_ERROR, "cmd error:%s", DEF_USER_LOG_DECOMPRESS);
                goto ERR_LABEL;
            }
        }
    }

    DEF_SAFE_FCLOSE(fp);
    DEF_SAFE_FREE(pcBuf);

END_LABEL:
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_FCLOSE(fp);
    DEF_SAFE_FREE(pcBuf);
    return OPLK_ERR;
}

static int
__UTIL_ResumeMsataLogFile(char *pcFileName)
{
    int iRet = OPLK_ERR;
    uint32_t ulIdex = 0;
    ST_RESUME_MSATA_LOG stResumeLog[] = {
        {DEF_OPLK_LOG_TAR_FILE, DEF_MSATA_SAVE_OPLK_LOG_PATH, DEF_MSATA_RESUME_OPLK_LOG_CMD, DEF_OPLK_LOG_DECOMPRESS, DEF_RM_OPLK_LOG},
        {DEF_USER_LOG_TAR_FILE, DEF_MSATA_SAVE_USER_LOG_PATH, DEF_MSATA_RESUME_USER_LOG_CMD, DEF_USER_LOG_DECOMPRESS, DEF_RM_USER_LOG}
    };

    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcFileName");
        goto ERR_LABEL;
    }

    if (0 == strncmp(stResumeLog[0].pcVarTarFile, pcFileName, strlen(stResumeLog[0].pcVarTarFile))) {
        ulIdex = 0;         /*for Resume oplk log*/
    } else if (0 == strncmp(stResumeLog[1].pcVarTarFile, pcFileName, strlen(stResumeLog[1].pcVarTarFile))) {
        ulIdex = 1; /*for Resume user log*/
    } else {
        LOG_WRITE(EN_LOG_ERROR, "ResumeMsataLogFile %s error", pcFileName);
        goto ERR_LABEL;
    }

    /*check file exist*/
    if (0 != access(stResumeLog[ulIdex].pcMsataTarFile, F_OK)) {
        LOG_WRITE(EN_LOG_NOTICE, "file missing:%s", stResumeLog[ulIdex].pcMsataTarFile);
        goto END_LABEL;
    }

    iRet = UTIL_System(stResumeLog[ulIdex].pcResumeCmd);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "system cmd error:%s", stResumeLog[ulIdex].pcResumeCmd);
        goto ERR_LABEL;
    }

    iRet = UTIL_System(stResumeLog[ulIdex].pcDecompressCmd);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "system cmd error:%s", stResumeLog[ulIdex].pcDecompressCmd);
        goto ERR_LABEL;
    }

    /*rm tar file exist*/
    iRet = UTIL_System(stResumeLog[ulIdex].pcRmCmd);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "system cmd error:%s", stResumeLog[ulIdex].pcRmCmd);
        goto ERR_LABEL;
    }

END_LABEL:
    return OPLK_OK;

ERR_LABEL:

    if (0 == access(stResumeLog[ulIdex].pcVarTarFile, F_OK)) {
        iRet = UTIL_System(stResumeLog[ulIdex].pcRmCmd);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "rm cmd error:%s", stResumeLog[ulIdex].pcRmCmd);
        }
    }

    return OPLK_ERR;
}

static int
__UTIL_InitLogFile(char *pcFileName)
{
    int iLen = 0;
    FILE *fpFile = NULL;
    uint32_t ulCrc32 = 0;
    ST_OPLK_LOG_HEADER stLogHeader;

    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcFileName");
        goto ERR_LABEL;
    }

    memset(&stLogHeader, 0x00, sizeof(stLogHeader));

    if (0 != access(pcFileName, F_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "0 != access FileName %s", pcFileName);
        goto RECREAT_FILE_LABEL;
    }

    fpFile = fopen(pcFileName, "r+");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == fpFile FileName %s", pcFileName);
        goto RECREAT_FILE_LABEL;
    }

    iLen = fread(&stLogHeader, sizeof(stLogHeader), 1, fpFile);
    if (1 != iLen) {
        LOG_WRITE(EN_LOG_ERROR, "fread iLen %d != 1", iLen);
        goto RECREAT_FILE_LABEL;
    }

    if (0 != strncmp(DEF_LOG_HEADER_KEYWORD, stLogHeader.acKeyWord, strlen(DEF_LOG_HEADER_KEYWORD))) {
        LOG_WRITE(EN_LOG_ERROR, "DEF_LOG_HEADER_KEYWORD %s", stLogHeader.acKeyWord);
        goto RECREAT_FILE_LABEL;
    }

    /*check ulCrc32 */
    if (ulCrc32 != stLogHeader.ulHeaderCrc32) {
        LOG_WRITE(EN_LOG_ERROR, "ulCrc32 %x != stLogHeader.ulHeaderCrc32 %x", ulCrc32, stLogHeader.ulHeaderCrc32);
        goto RECREAT_FILE_LABEL;
    }

    __UTIL_SyncGlobalLogHeader(pcFileName, &stLogHeader, fpFile);

    return OPLK_OK;

RECREAT_FILE_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    __UTIL_RecreateLogFile(pcFileName);
    return OPLK_OK;

ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return OPLK_ERR;
}

static int
__UTIL_WriteLogFile(FILE *pstFp,
                    ST_OPLK_LOG_HEADER *pstLogHeader,
                    char *pcLogInfo,
                    uint32_t ulLogMaxNum,
                    uint32_t ulLogSize)
{
    uint32_t ulLen = 0;

    if (NULL == pstFp || NULL == pstLogHeader || NULL == pcLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstFp || NULL == pstLogHeader || NULL == pcLogInfo");
        goto ERR_LABEL;
    }

    if (pstLogHeader->ulEndIndex >= ulLogMaxNum ||
        pstLogHeader->ulStartIndex >= ulLogMaxNum) {
        pstLogHeader->ulStartIndex = 0;
        pstLogHeader->ulEndIndex = 0;
    }

    /*  get to the file beginning */
    fseek(pstFp, 0, SEEK_SET);

    /*  find the position to append log */
    /* printf("begin to write log method %s, user %s, lognum is %d, log :[%s] to file %s, position %d\n",
             pstLogInfo->acMethod, pstLogInfo->acUserName, pstLogInfo->iLogNum, pstLogInfo->acRecordMsg, pstLogHeader->acResv, pstLogHeader->iEndIndex);
     */
    ulLen = sizeof(ST_OPLK_LOG_HEADER) + pstLogHeader->ulEndIndex * ulLogSize;
    fseek(pstFp, ulLen, SEEK_SET);

    ulLen = fwrite(pcLogInfo, ulLogSize, 1, pstFp);
    if (1 != ulLen) {
        LOG_WRITE(EN_LOG_ERROR, "fwrite ulLen %d != 1", ulLen);
        goto ERR_LABEL;
    }

    __UTIL_FflushLogFile(pstFp);
    __UTIL_WriteLogHeader(pstFp, pstLogHeader, ulLogMaxNum);

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


static int
__UTIL_RecordOplkLogFile(ST_OPLK_LOG_BODY *pstLogInfo)
{
    if (NULL == pstLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo");
        goto ERR_LABEL;
    }

    switch (pstLogInfo->ucBank) {
    case EN_LOG_BANK_L:
        __UTIL_WriteLogFile(m_fpLowLog, &m_stLowLogHeader, (char *)pstLogInfo,
                            DEF_OPLK_LOG_MAX_NUM, DEF_OPLK_LOG_BODY_LEN);
        break;
    case EN_LOG_BANK_M:
        __UTIL_WriteLogFile(m_fpMidLog, &m_stMidLogHeader, (char *)pstLogInfo,
                            DEF_OPLK_LOG_MAX_NUM, DEF_OPLK_LOG_BODY_LEN);
        break;
    case EN_LOG_BANK_H:
        __UTIL_WriteLogFile(m_fpHigLog, &m_stHighLogHeader, (char *)pstLogInfo,
                            DEF_OPLK_LOG_MAX_NUM, DEF_OPLK_LOG_BODY_LEN);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "pstLogInfo->ucBank %d error", pstLogInfo->ucBank);
        goto ERR_LABEL;
        break;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}

static int
__UTIL_RecordOplkLogSyslog(ST_OPLK_LOG_BODY *pstLogInfo)
{
    char acStrRecord[DEF_SYSLOG_LOG_MSG_LEN] = {0};
    uint8_t ucFacility = 0;

    char acProcName[DEF_PROC_NAME_LEN];

    if (NULL == pstLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo");
        goto ERR_LABEL;
    }

    /*according the Facility, syslog write the log to differernt log file*/
    /*LOG_LOCAL0 -LOG_LOCAL2 for oplink internal log, LOG_LOCAL3 for user log*/
    switch (pstLogInfo->ucSeverity) {
    case EN_LOG_EMERGENCY:
    case EN_LOG_CRITICAL:
    case EN_LOG_ERROR:
        ucFacility = LOG_LOCAL0;
        break;
    case EN_LOG_ALERT:
        ucFacility = LOG_LOCAL3;
        break;
    case EN_LOG_WARNING:
        ucFacility = LOG_LOCAL1;
        break;
    case EN_LOG_NOTICE:
    case EN_LOG_TRACE:
    case EN_LOG_DEBUG:
        ucFacility = LOG_LOCAL2;
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "pstLogInfo->ucSeverity %d %d error", pstLogInfo->ucSeverity, ucFacility);
        goto ERR_LABEL;
        break;
    }

    __UTIL_GetProcName(pstLogInfo->ucProcess, acProcName, sizeof(acProcName));

    /*according the SysLevel, syslog write the log to differernt 3 log file:internal-low.log,internal-mid.log,internal-high.log*/
    openlog(acProcName, LOG_PID, ucFacility);

    /* format log */
    snprintf(acStrRecord, DEF_SYSLOG_LOG_MSG_LEN - 1,
             "%lu [%s][%s:%d] %s\n",
             pstLogInfo->ullTimeMs,
             UTIL_LogSeverityEnumToStr(pstLogInfo->ucSeverity),
             pstLogInfo->acFileName,
             pstLogInfo->ulLineNum,
             pstLogInfo->acRecordMsg);

    /* syslog(pstLogInfo->ucSeverity, "%s", acStrRecord); */
#ifdef _DEBUG_SIMULATE_
    printf(" [%d] %s", pstLogInfo->ucSeverity, acStrRecord);
#else
    char acHashKey[DEF_BUF_SIZE_64] = {0};
    /* This is a debug function, generally, the hash table is enable in release version */
    if (false == m_bHashTableEnable) {
        syslog(pstLogInfo->ucSeverity, "%s", acStrRecord);
    } else {
        switch (pstLogInfo->ucSeverity) {
        case EN_LOG_EMERGENCY:
        case EN_LOG_CRITICAL:
        case EN_LOG_ALERT:
        case EN_LOG_WARNING:
            syslog(pstLogInfo->ucSeverity, "%s", acStrRecord);
            break;
        case EN_LOG_NOTICE:
        case EN_LOG_DEBUG:
        case EN_LOG_TRACE:
        case EN_LOG_ERROR:
        default:
            memset(acHashKey, 0x00, sizeof(acHashKey));
            snprintf(acHashKey, sizeof(acHashKey), "%s:%d", pstLogInfo->acFileName, pstLogInfo->ulLineNum);
            if (0 == UTIL_LogHash(acHashKey, acStrRecord)) {
                syslog(pstLogInfo->ucSeverity, "%s", acStrRecord);
            }
            break;
        }
    }
#endif

    closelog();

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


static int
__UTIL_RecordOplkLog(ST_OPLK_LOG_BODY *pstLogInfo)
{
    EN_UTIL_LOG_OUTPUT enOplklogOutput = EN_OUTPUT_FILE;

    if (NULL == pstLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo");
        goto ERR_LABEL;
    }

    /*accordding the severity, set the log write bank*/
    switch (pstLogInfo->ucSeverity) {
    case EN_LOG_EMERGENCY:
    case EN_LOG_ALERT:
    case EN_LOG_CRITICAL:
    case EN_LOG_ERROR:
        pstLogInfo->ucBank = EN_LOG_BANK_H;
        break;
    case EN_LOG_WARNING:
        pstLogInfo->ucBank = EN_LOG_BANK_M;
        break;
    case EN_LOG_NOTICE:
    case EN_LOG_TRACE:
    case EN_LOG_DEBUG:
        pstLogInfo->ucBank = EN_LOG_BANK_L;
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "pstLogInfo->ucSeverity %d error", pstLogInfo->ucSeverity);
        goto ERR_LABEL;
        break;
    }

    /*output the log infor to printf. logfile or syslog*/
    enOplklogOutput = UTIL_GetHostOplkLogOutput();

    switch (enOplklogOutput) {
    case EN_OUTPUT_FILE:
        __UTIL_RecordOplkLogFile(pstLogInfo);
        break;
    case EN_OUTPUT_SYSLOG:
        __UTIL_RecordOplkLogSyslog(pstLogInfo);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "enOplklogOutput %d error", enOplklogOutput);
        goto ERR_LABEL;
        break;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static int
__UTIL_RecordUserLogFile(ST_USER_LOG_BODY *pstLogInfo)
{
    if (NULL == pstLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo");
        goto ERR_LABEL;
    }

    __UTIL_WriteLogFile(m_fpUserLog, &m_stUserLogHeader, (char *)pstLogInfo,
                        DEF_OPLK_LOG_MAX_NUM, DEF_USER_LOG_BODY_LEN);

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static int
__UTIL_RecordUserLogSyslog(ST_USER_LOG_BODY *pstLogInfo)
{
    char acStrRecord[DEF_SYSLOG_LOG_MSG_LEN] = {0};
    uint8_t ucFacility = LOG_LOCAL3;

    char acProcName[DEF_PROC_NAME_LEN];

    if (NULL == pstLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo");
        goto ERR_LABEL;
    }

    /*according the Facility, syslog write the log to differernt log file*/
    /*LOG_LOCAL0 -LOG_LOCAL2 for oplink internal log, LOG_LOCAL3 for user log*/
    ucFacility = LOG_LOCAL3;

    __UTIL_GetProcName(pstLogInfo->ucProcess, acProcName, sizeof(acProcName));

    /*according the SysLevel, syslog write the log to differernt 3 log file:internal-low.log,internal-mid.log,internal-high.log*/
    openlog(acProcName, LOG_PID, ucFacility);

    /* format log */
    snprintf(acStrRecord, DEF_SYSLOG_LOG_MSG_LEN - 1,
             "%lu [%s][%s:%d] %s\n",
             pstLogInfo->ullTimeMs,
             UTIL_LogSeverityEnumToStr(pstLogInfo->ucSeverity),
             pstLogInfo->acUserName,
             pstLogInfo->ucUserLevel,
             pstLogInfo->acRecordMsg);

    syslog(pstLogInfo->ucSeverity, "%s", acStrRecord);

    closelog();

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;

}

static int
__UTIL_RecordUserLog(ST_USER_LOG_BODY *pstLogInfo)
{
    EN_UTIL_LOG_OUTPUT enUserlogOutput = EN_OUTPUT_FILE;

    if (NULL == pstLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo");
        goto ERR_LABEL;
    }

    /*output the log infor to printf. logfile or syslog*/
    enUserlogOutput = UTIL_GetHostUserLogOutput();

    switch (enUserlogOutput) {
    case EN_OUTPUT_FILE:
        __UTIL_RecordUserLogFile(pstLogInfo);
        break;
    case EN_OUTPUT_SYSLOG:
        __UTIL_RecordUserLogSyslog(pstLogInfo);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "enUserlogOutput %d error", enUserlogOutput);
        goto ERR_LABEL;
        break;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static int
__UTIL_BuildOplkLog(ST_OPLK_LOG_BODY *pstLogInfo,
                    uint8_t ucProcess,
                    const char *pcFileName,
                    uint32_t ulLineNum,
                    uint8_t ucSeverity,
                    char *pcStrInfo)
{
    int iLen = 0;

    if (NULL == pstLogInfo || NULL == pcFileName || NULL == pcStrInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pstLogInfo || NULL == pcFileName || NULL == pcStrInfo");
        goto ERR_LABEL;
    }

    /* get the log happened time */
    pstLogInfo->ullTimeMs = UTIL_GetTimeMs();

    /* fill the file name, line no and severity */
    pstLogInfo->ucProcess = ucProcess;
    snprintf(pstLogInfo->acFileName, DEF_LOG_FILE_NAME_LEN, "%s", pcFileName);
    pstLogInfo->ulLineNum = ulLineNum;
    pstLogInfo->ucSeverity = ucSeverity;

    /* log body content */
    iLen = snprintf(pstLogInfo->acRecordMsg, DEF_OPLK_LOG_MSG_LEN - 1, "%s", pcStrInfo);
    if (iLen < 0) {
        LOG_WRITE(EN_LOG_DEBUG, "output may be truncated");
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static int
__UTIL_ParseLogMsg(char *pcRecvBuf, uint32_t ulLen)
{
    ST_OOP_MSG_HEADER *pstMsgBuf = NULL;
    ST_OPLK_LOG_BODY *pstOplkLogInfo = NULL;
    ST_USER_LOG_BODY *pstUserLogInfo = NULL;
    uint32_t ulOid = 0;
    int iRet = OPLK_ERR;

    if (NULL == pcRecvBuf) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcRecvBuf");
        goto ERR_LABEL;
    }

    pstMsgBuf = (ST_OOP_MSG_HEADER *)pcRecvBuf;
    ulOid = ntohl(pstMsgBuf->ulOid);
    switch (ulOid) {
    case DEF_OID_LOG_ADD_OPLK_LOG: {
        pstOplkLogInfo = (ST_OPLK_LOG_BODY *)((char *)pstMsgBuf + sizeof(ST_OOP_MSG_HEADER));

        /* printf("ucProcess %d\r\n", pstOplkLogInfo->ucProcess); */
        iRet = __UTIL_RecordOplkLog(pstOplkLogInfo);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "__UTIL_RecordOplkLog error");
            goto ERR_LABEL;
        }
        break;
    }

    case DEF_OID_LOG_ADD_USER_LOG: {
        pstUserLogInfo = (ST_USER_LOG_BODY *)((char *)pstMsgBuf + sizeof(ST_OOP_MSG_HEADER));

        iRet = __UTIL_RecordUserLog(pstUserLogInfo);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "__UTIL_RecordUserLog error");
            goto ERR_LABEL;
        }
        break;
    }

    case DEF_OID_LOG_SAVE_LOG: {
        __UTIL_SaveHostLog();
        __UTIL_SendSigUsr1();
        break;
    }
    default: {
        LOG_WRITE(EN_LOG_ERROR, "unknow log msg oid %x", ulOid);
        break;
    }

    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static int
__UTIL_ReadFileByLen(char *pcFileName, int32_t lLen, char *pcBuffer)
{
    FILE *fpFile = NULL;

    if (lLen <= 0 || pcBuffer == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "lLen <= 0 || pcBuffer == NULL");
        goto ERR_LABEL;
    }

    if (NULL == (fpFile = fopen(pcFileName, "r"))) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == fopen %s", pcFileName);
        goto ERR_LABEL;
    }

    if (lLen != fread(pcBuffer, sizeof(int8_t), lLen, fpFile)) {
        LOG_WRITE(EN_LOG_ERROR, "lLen %d != fread", lLen);
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(fpFile);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_FCLOSE(fpFile);
    return OPLK_ERR;
}

static int
__UTIL_SaveLogToMtd(char *pcFileName)
{
    char *pcBuffer = NULL;
    int32_t lLen = 0;
    ST_UTIL_LOG_FILE_HEADER stLogFileInfo;
    int iRet = OPLK_ERR;

    if (NULL == pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "NULL == pcFileName");
        goto ERR_LABEL;
    }

    if (0 != access(pcFileName, F_OK)) {
        LOG_WRITE(EN_LOG_TRACE, "do not can get file %s", pcFileName);
        goto END_LABEL;
    }

    if (DEF_LOG_FLASH_SIZE <= lLen) {
        LOG_WRITE(EN_LOG_ERROR, "DEF_LOG_FLASH_SIZE <= lLen %d", lLen);
        goto ERR_LABEL;
    }

    stLogFileInfo.ulFileLen = lLen;
    stLogFileInfo.enLogFileType = EN_FILE_TYPE_TGZ;

    /* extra  bytes left for the file len and file type storage */
    pcBuffer = (char *)malloc(lLen + sizeof(ST_UTIL_LOG_FILE_HEADER));
    if (pcBuffer == NULL) {
        LOG_WRITE(EN_LOG_ERROR, "pcBuffer == NULL");
        goto ERR_LABEL;
    }

    memset(pcBuffer, 0, lLen + sizeof(ST_UTIL_LOG_FILE_HEADER));
    memcpy(pcBuffer, &stLogFileInfo, sizeof(ST_UTIL_LOG_FILE_HEADER));   /* record the file length info */
    __UTIL_ReadFileByLen(pcFileName, lLen, pcBuffer + sizeof(ST_UTIL_LOG_FILE_HEADER));

    if (0 == strncmp(DEF_OPLK_LOG_TAR_FILE, pcFileName, strlen(DEF_OPLK_LOG_TAR_FILE))) {
        /* save the log to flash */
        /* UTIL_SaveMtd(EN_MTD_NO_OPLK_LOG, 0, (uint8_t *)pcBuffer, lLen+sizeof(ST_UTIL_LOG_FILE_HEADER)); */

    } else if (0 == strncmp(DEF_USER_LOG_TAR_FILE, pcFileName, strlen(DEF_USER_LOG_TAR_FILE))) {
        /* save the log to flash */
        /* UTIL_SaveMtd(EN_MTD_NO_USER_LOG, 0, (uint8_t *)pcBuffer, lLen+sizeof(ST_UTIL_LOG_FILE_HEADER)); */

    } else {
        LOG_WRITE(EN_LOG_ERROR, "0 == strncm pcFileName %s", pcFileName);
        goto ERR_LABEL;
    }

    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "rm log error");
        goto ERR_LABEL;
    }

END_LABEL:

    DEF_SAFE_FREE(pcBuffer);
    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_FREE(pcBuffer);
    return OPLK_ERR;
}

static int
__UTIL_SaveUserLog(void)
{
    int iRet = OPLK_ERR;
    char acCompress[DEF_BUF_SIZE_256] = {0};

    if (0 != access(DEF_USER_LOG_FILE, F_OK)) {
        LOG_WRITE(EN_LOG_ERROR, "0 != access FileName %s", DEF_USER_LOG_FILE);
        goto END_LABEL;
    }

    /*the file exist, so can compress the file*/
    snprintf(acCompress, DEF_BUF_SIZE_256, DEF_LOG_COMPRESS, DEF_USER_LOG_TAR_FILE, DEF_USER_LOG_FILE);

    /* LogFileType = EN_FILE_TYPE_TGZ */
    iRet = __UTIL_TarCmd(acCompress);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "Compress cmd error:%s", acCompress);
        goto ERR_LABEL;
    }

    if (EN_LOG_SAVE_MSATA == m_enSaveDevice) {
        /*save and delete the tar file*/
        iRet = UTIL_System(DEF_MSATA_SAVE_USER_LOG_CMD);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "cp cmd error:%s", DEF_MSATA_SAVE_USER_LOG_CMD);
            goto ERR_LABEL;
        }
    } else {
        iRet = __UTIL_SaveLogToMtd(DEF_USER_LOG_TAR_FILE);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "__UTIL_SaveLogToMtd %s error", DEF_USER_LOG_TAR_FILE);
            goto ERR_LABEL;
        }
    }

    /* rm /var/log/user-log.tar.gz */
    if (0 == access(DEF_USER_LOG_TAR_FILE, F_OK)) {
        iRet = UTIL_System(DEF_RM_USER_LOG);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "rm cmd error:%s", DEF_RM_USER_LOG);
            goto ERR_LABEL;
        }
    }

END_LABEL:

    return OPLK_OK;

ERR_LABEL:

    if (0 == access(DEF_USER_LOG_TAR_FILE, F_OK)) {
        iRet = UTIL_System(DEF_RM_USER_LOG);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "rm cmd error:%s", DEF_RM_USER_LOG);
        }
    }

    return OPLK_ERR;
}


static int
__UTIL_SaveOplkLog(void)
{
    int iRet = OPLK_ERR;
    char acCompress[DEF_BUF_SIZE_256] = {0};

    snprintf(acCompress, DEF_BUF_SIZE_256, DEF_LOG_COMPRESS, DEF_OPLK_LOG_TAR_FILE, DEF_LOG_ROOT_DIRECTORY);
    /* the all log file are compressed */
    iRet = __UTIL_TarCmd(acCompress);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "Compress cmd error:%s", acCompress);
        goto ERR_LABEL;
    }

    if (EN_LOG_SAVE_MSATA == m_enSaveDevice) {
        iRet = UTIL_System(DEF_MSATA_SAVE_OPLK_LOG_CMD);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "cp cmd error:%s", DEF_MSATA_SAVE_OPLK_LOG_CMD);
            goto ERR_LABEL;
        }
    } else {
        iRet = __UTIL_SaveLogToMtd(DEF_OPLK_LOG_TAR_FILE);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "__UTIL_SaveLogToMtd error");
            goto ERR_LABEL;
        }
    }
    /* rm /var/log/internal-log.tar.gz */
    if (0 == access(DEF_OPLK_LOG_TAR_FILE, F_OK)) {
        iRet = UTIL_System(DEF_RM_OPLK_LOG);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "rm cmd error:%s", DEF_RM_USER_LOG);
            goto ERR_LABEL;
        }
    }

    return OPLK_OK;

ERR_LABEL:

    if (0 == access(DEF_OPLK_LOG_TAR_FILE, F_OK)) {
        iRet = UTIL_System(DEF_RM_OPLK_LOG);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "rm cmd error:%s", DEF_RM_USER_LOG);
        }
    }

    return OPLK_ERR;
}


static void
__UTIL_SaveLogAuto(int64_t llCurTime)
{
    int64_t llLastTime = 0;

    llLastTime = m_llSaveLogTime;

    /*auto save Log every 2hours*/
    if ((llCurTime - llLastTime) >= DEF_AUTO_SAVE_LOG_TIME) {
        /* printf("__UTIL_SaveLogAuto m_lProcUptime %ld %ld\r\n", llCurTime, m_llSaveLogTime); */

        __UTIL_SaveHostLog();

        m_llSaveLogTime = llCurTime;
    }

    return;
}


static int
__UTIL_RecordSystemInfor(int64_t llCurTime)
{
    int64_t llLastTime = 0;
    char *pcSysInfor = NULL;
    char *pacSysCmd[7] = {DEF_SYS_CMD_CAT_PROC, DEF_SYS_CMD_NETSTAT, DEF_SYS_CMD_DF,
                          DEF_SYS_CMD_IPCS, DEF_SYS_CMD_PS, DEF_SYS_CMD_PSTREE, DEF_SYS_CMD_TOP};
    int i = 0;

    llLastTime = m_llCheckSysTime;
    /*auto save Log every 2hours*/
    if ((llCurTime - llLastTime) >= DEF_CHECK_SYSTEM_INFOR_TIME) {
        /* printf("__UTIL_RecordSystemInfor m_lCheckSysTime %ld %ld\r\n", llCurTime, m_llCheckSysTime); */

        pcSysInfor = malloc(DEF_SYS_INFO_SIZE + 1);
        if (NULL == pcSysInfor) {
            goto ERR_LABEL;
        }

        for (i = 0; i < 7; i++) {
            if (NULL == pacSysCmd[i]) {
                LOG_WRITE(EN_LOG_ERROR, "system command is Null");
                goto ERR_LABEL;
            }

            memset(pcSysInfor, 0x20, DEF_SYS_INFO_SIZE);
            UTIL_ExecuteCmdLargeResp(pacSysCmd[i], pcSysInfor, DEF_SYS_INFO_SIZE, 500);

            __UTIL_WriteLogFile(m_fpSysInfor, &m_stSysInforHeader, (char *)pcSysInfor,
                                DEF_SYS_INFO_MAX_NUM, DEF_SYS_INFO_SIZE);
        }

        m_llCheckSysTime = llCurTime;

    }

    DEF_SAFE_FREE(pcSysInfor);

    return OPLK_OK;

ERR_LABEL:

    DEF_SAFE_FREE(pcSysInfor);

    return OPLK_ERR;
}

static void *
__UTIL_ProcessLogMsgThread(void)
{
    ST_LOG_QUEUE_MSG stQueueMsg;
    uint32_t ulLen = 0;

    while (false == m_bExitHostLoop) {
        memset(&stQueueMsg, 0x00, sizeof(stQueueMsg));

        /* get log message node from queue */
        ulLen = sizeof(ST_LOG_QUEUE_MSG);
        if (OPLK_OK != UTIL_GetQueueNode(m_cQueueIndex, (char *)&stQueueMsg, &ulLen)) {
            DEF_SLEEP_MS(5);
            continue;
        }

        __UTIL_ParseLogMsg(stQueueMsg.acMsgBuf, stQueueMsg.ulMsgLen);
    }

    LOG_WRITE(EN_LOG_ERROR, "__UTIL_ProcessLogMsgThread exit");

    return NULL;
}

static int
__UTIL_ProcessLogMsg(void)
{
    int iRet = OPLK_ERR;

    pthread_t tUtilLogMsg;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    /* Create thread. */
    iRet = pthread_create(&tUtilLogMsg, &attr, (void *)__UTIL_ProcessLogMsgThread, NULL);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "Create __UTIL_ProcessLogMsgThread error");
        goto ERR_LABEL;
    }

    return OPLK_OK;
ERR_LABEL:
    return OPLK_ERR;
}


static void *
__UTIL_RecvLogMsgThread(void)
{
    int iFd = -1;
    fd_set readFds;
    struct timeval stTimeout;
    int iRecvLen = 0;
    char acRecvBuf[DEF_LOG_MSG_MAX_LEN];
    int iRet = OPLK_ERR;
    int i = 0;

    ST_LOG_QUEUE_MSG stQueueMsg;

    /* create unix socket to receive log message */
    for (i = 0; i < 3; i++) {
        iFd = UTIL_CreateMsgSock(DEF_HOST_RECV_LOG_PATH);
        if (0 > iFd) {

            UTIL_CloseMsgSock(DEF_HOST_RECV_LOG_PATH, iFd);
            continue;
        }
        /* UTIL_CreateMsgSock ok*/
        break;
    }

    if (i >= 3) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateMsgSock %d error", i);
        goto ERR_LABEL;
    }

    /* Main loop. Listens writing log message and app heart beat */
    while (false == m_bExitHostLoop) {
        FD_ZERO(&readFds);
        FD_SET(iFd, &readFds);

        stTimeout.tv_sec = 5;
        stTimeout.tv_usec = 0;

        iRet = select(iFd + 1, &readFds, NULL, NULL, &stTimeout);
        if (0 > iRet) {
            goto ERR_LABEL;
        } else if (0 == iRet) {
            continue;     /*timeout*/
        }

        /*select return > 0 received message from oop socket*/
        if (FD_ISSET(iFd, &readFds)) {
            memset(acRecvBuf, 0x00, sizeof(acRecvBuf));
            iRecvLen = recvfrom(iFd, acRecvBuf, DEF_LOG_MSG_MAX_LEN, MSG_WAITALL, NULL, NULL);
            if (iRecvLen > 0) {

                if (OPLK_OK != UTIL_ChkOopMsg(acRecvBuf, iRecvLen)) {
                    /* add Recv OOP msg check sum error to Log queue */
                    LOG_WRITE(EN_LOG_ERROR, DEF_RCEV_MSG_CHK_ERR);
                    continue;
                }

                if (DEF_LOG_MSG_MAX_LEN <= iRecvLen) {
                    /* add Recv OOP msg length error to Log queue */
                    LOG_WRITE(EN_LOG_ERROR, DEF_RCEV_MSG_LENGTH_ERR);
                    continue;
                }

                /*add log message to log queue*/
                memset(&stQueueMsg, 0x00, sizeof(stQueueMsg));
                stQueueMsg.ulMsgLen = iRecvLen;
                memcpy(stQueueMsg.acMsgBuf, acRecvBuf, iRecvLen);
                iRet = UTIL_AddQueueNode(m_cQueueIndex, (char *)&stQueueMsg, sizeof(stQueueMsg));
                if (OPLK_OK != iRet) {
                    DEF_SLEEP_US(3);
                    LOG_WRITE(EN_LOG_ERROR, "UTIL_AddQueueNode error %d", m_cQueueIndex);
                    continue;
                }
            }
        }
    }

ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "__UTIL_RecvLogMsgThread exit");

    return NULL;
}

static int
__UTIL_RecvLogMsg(void)
{
    int iRet = OPLK_ERR;

    pthread_t tRecvLogMsg;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    /* Create thread to handle oop-set queue. */
    iRet = pthread_create(&tRecvLogMsg, &attr,
                          (void *)__UTIL_RecvLogMsgThread, NULL);
    if (OPLK_OK != iRet) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static void *
__UTIL_SaveLogThread(void)
{
    int64_t llCurTime = 0;

    while (false == m_bExitHostLoop) {

        DEF_SLEEP_S(300);

        llCurTime = UTIL_GetUptime();
        if (0 < llCurTime) {

            /*record syslog running information every 15mins*/
            __UTIL_RecordSystemInfor(llCurTime);
            /*auto save Log every 2hours*/
            __UTIL_SaveLogAuto(llCurTime);

        }
    }

    LOG_WRITE(EN_LOG_ERROR, "__UTIL_SaveLogThread exit");

    return NULL;
}

static int
__UTIL_SaveLog(void)
{
    int iRet = OPLK_ERR;

    pthread_t tSaveLog;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    /* Create thread to record system infor and save log auto. */
    iRet = pthread_create(&tSaveLog, &attr, (void *)__UTIL_SaveLogThread, NULL);
    if (OPLK_OK != iRet) {
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

static EN_UTIL_LOG_SEVERITY
__UTIL_GetClientLogSeverity(void)
{
    return (EN_UTIL_LOG_SEVERITY)m_stClientLogUnixSock.ucRecordSeverity;
}

static bool
__UTIL_GetClientLogEnable(void)
{
    return (bool)m_stClientLogUnixSock.ucLogEnable;
}

static void
__UTIL_SaveHostLog(void)
{
    __UTIL_SaveOplkLog();
    __UTIL_SaveUserLog();

    return;
}


int
UTIL_InitHostLog(void)
{
    m_llCheckSysTime = UTIL_GetUptime();
    m_llSaveLogTime = m_llCheckSysTime;

    if (EN_LOG_SAVE_MSATA == m_enSaveDevice) {
        __UTIL_ResumeMsataLogFile(DEF_OPLK_LOG_TAR_FILE);
        __UTIL_ResumeMsataLogFile(DEF_USER_LOG_TAR_FILE);
    } else {
        __UTIL_ResumeFlashLogFile(DEF_OPLK_LOG_TAR_FILE);
        __UTIL_ResumeFlashLogFile(DEF_USER_LOG_TAR_FILE);
    }

    m_cQueueIndex = UTIL_InitQueue(sizeof(ST_LOG_QUEUE_MSG), DEF_LOG_NODE_NUM);
    if (0 >= m_cQueueIndex) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_InitQueue fail %d", m_cQueueIndex);
        return OPLK_ERR;
    }

    return OPLK_OK;
}

int
UTIL_CloseHostLog(void)
{
    return UTIL_CloseQueue(m_cQueueIndex);
}

void
UTIL_SetClientLogSeverity(EN_UTIL_LOG_SEVERITY enSeverity)
{
    if (EN_LOG_DEBUG < enSeverity) {
        m_stClientLogUnixSock.ucRecordSeverity = EN_LOG_NOTICE;
        return;
    }

    m_stClientLogUnixSock.ucRecordSeverity = enSeverity;

    return;
}


int
UTIL_SetHostLogOutput(EN_UTIL_LOG_OUTPUT enOplkOutput, EN_UTIL_LOG_OUTPUT enUserOutput)
{
    int iRet = OPLK_ERR;

    if (EN_OUTPUT_FILE == enOplkOutput) {
        /*oplk log output to logfile, init the logfile*/
        iRet = __UTIL_InitLogFile(DEF_OPLK_LOW_LOG_FILE);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "Init LogFile %s error", DEF_OPLK_LOW_LOG_FILE);
            goto ERR_LABEL;
        }

        iRet = __UTIL_InitLogFile(DEF_OPLK_MID_LOG_FILE);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "Init LogFile %s error", DEF_OPLK_MID_LOG_FILE);
            goto ERR_LABEL;
        }

        iRet = __UTIL_InitLogFile(DEF_OPLK_HIGH_LOG_FILE);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "Init LogFile %s error", DEF_OPLK_HIGH_LOG_FILE);
            goto ERR_LABEL;
        }

    } else if (EN_OUTPUT_SYSLOG == enOplkOutput) {
        /*nothing todo, syslog have been configed by rsyslog.conf*/
    }

    /*user log output to logfile, init the logfile*/
    if (EN_OUTPUT_FILE == enUserOutput ) {
        iRet = __UTIL_InitLogFile(DEF_USER_LOG_FILE);
        if (OPLK_OK != iRet) {
            LOG_WRITE(EN_LOG_ERROR, "Init LogFile %s error", DEF_OPLK_HIGH_LOG_FILE);
            goto ERR_LABEL;
        }
    } else if (EN_OUTPUT_SYSLOG == enUserOutput ) {
        /*nothing todo, syslog have been configed by rsyslog.conf*/
    }

    __UTIL_InitLogFile(DEF_OPLK_SYSTEM_INFOR_FILE);

    m_enOplklogOutput = enOplkOutput;
    m_enUserlogOutput = enUserOutput;

    return OPLK_OK;

ERR_LABEL:

    m_enOplklogOutput = enOplkOutput;
    m_enUserlogOutput = enUserOutput;

    return OPLK_ERR;
}

EN_UTIL_LOG_OUTPUT
UTIL_GetHostOplkLogOutput(void)
{
    return m_enOplklogOutput;
}

EN_UTIL_LOG_OUTPUT
UTIL_GetHostUserLogOutput(void)
{
    return m_enUserlogOutput;
}


int
UTIL_InitClientLog(void)
{
    char acProcName[DEF_PROC_NAME_LEN];
    char acProcLogPath[DEF_BUF_SIZE_128];
    int iFd = -1;
    int i = 0;

    /* default record debug log */
    m_stClientLogUnixSock.iFd = -1;
    m_stClientLogUnixSock.ucRecordSeverity = EN_LOG_NOTICE;
    m_stClientLogUnixSock.ucLogEnable = true;

    memset(acProcName, 0, sizeof(acProcName));
    if (OPLK_OK != UTIL_GetProcName(acProcName, sizeof(acProcName))) {
        LOG_WRITE(EN_LOG_ERROR, "Get proc name error. pid=%d", getpid());
        return OPLK_ERR;
    }

    memset(acProcLogPath, 0, sizeof(acProcLogPath));
    snprintf(acProcLogPath, sizeof(acProcLogPath), DEF_CLENT_SEND_LOG_PATH, acProcName);

    for (i = 0; i < 3; i++) {
        iFd = UTIL_CreateMsgSock(acProcLogPath);
        if (0 > iFd) {

            UTIL_CloseMsgSock(acProcLogPath, iFd);
            continue;
        }
        /* UTIL_CreateMsgSock ok*/
        break;
    }

    if (i >= 3) {
        LOG_WRITE(EN_LOG_ERROR, "UTIL_CreateMsgSock %d error", i);
        return OPLK_ERR;
    }

    m_stClientLogUnixSock.iFd = iFd;

    return OPLK_OK;
}

int
UTIL_CloseClientLog(void)
{
    char acProcName[DEF_PROC_NAME_LEN];
    char acProcLogPath[DEF_BUF_SIZE_128];

    memset(acProcName, 0, sizeof(acProcName));
    if (OPLK_OK != UTIL_GetProcName(acProcName, sizeof(acProcName))) {
        LOG_WRITE(EN_LOG_ERROR, "Get proc name error. pid=%d", getpid());
        return OPLK_ERR;
    }

    memset(acProcLogPath, 0, sizeof(acProcLogPath));
    snprintf(acProcLogPath, sizeof(acProcLogPath), DEF_CLENT_SEND_LOG_PATH, acProcName);

    UTIL_CloseMsgSock(acProcLogPath, m_stClientLogUnixSock.iFd);

    return OPLK_OK;
}

int
UTIL_SendClientOplkLog(EN_UTIL_LOG_SEVERITY enSeverity,
                       const char *pcFileName,
                       uint32_t ulLineNum,
                       const char *pcLogInfo, ...)
{
    va_list args;
    int iFd = -1;
    int iRet = OPLK_ERR;
    ST_OPLK_LOG_BODY stLogInfo;
    char acOopBuf[DEF_LOG_MSG_MAX_LEN] = {0};
    char acLogContent[DEF_OPLK_LOG_MSG_LEN]; /*Record Msg */
    int32_t lLen = 0;
    int iProcess = 0;

    static bool m_bReadProc = false;
    char acProcName[DEF_PROC_NAME_LEN];

    if (NULL == pcFileName || NULL == pcLogInfo) {
        LOG_WRITE(EN_LOG_ERROR, "input NULL == pcFileName || NULL == pcLogInfo");
        goto ERR_LABEL;
    }

    memset(&stLogInfo, 0, sizeof(ST_OPLK_LOG_BODY));
    memset(acLogContent, 0, DEF_OPLK_LOG_MSG_LEN);

    va_start(args, pcLogInfo);
    vsnprintf(acLogContent, DEF_OPLK_LOG_MSG_LEN - 1, pcLogInfo, args);
    va_end(args);

    if (!m_bReadProc) {
        memset(acProcName, 0, sizeof(acProcName));
        UTIL_GetProcName(acProcName, sizeof(acProcName));
        m_bReadProc = true;
    }

    if (0 == strcmp(acProcName, "recordlogd")) {
        /* LOG_LOCAL4 for /var/log/recordlogd.log */
        openlog("recordlogd", LOG_PID, LOG_LOCAL4);

        syslog(enSeverity, "%s %u %s\n", pcFileName, ulLineNum, acLogContent);

        closelog();

        goto END_LABLE;
    }

    if (false == __UTIL_GetClientLogEnable()) {
        /*the current process log do not need record*/
        goto END_LABLE;
    }

    /* check record level, if the log severity < the setting severity, then do not record the log */
    if (enSeverity > __UTIL_GetClientLogSeverity()) {
        /* printf("enSeverity Severity error\r\n"); */
        goto END_LABLE;
    }

    iProcess = __UTIL_GetProcId();
    if (iProcess < 0) {
        printf("__UTIL_GetProcId error\r\n");
        goto ERR_LABEL;
    }

    /* build log body */
    iRet = __UTIL_BuildOplkLog(&stLogInfo, (uint8_t)iProcess, pcFileName, ulLineNum,
                               enSeverity, acLogContent);
    if (OPLK_OK != iRet) {
        printf("__UTIL_BuildOplkLog error\r\n");
        goto ERR_LABEL;
    }

    /* creat oop message: log body + log header */
    lLen = UTIL_CreateOopMsg(acOopBuf, DEF_OOP_MSG_TYPE_REQ, 0, 0,
                             DEF_OID_LOG_ADD_OPLK_LOG, 0,
                             (char *)&stLogInfo, sizeof(stLogInfo));
    if (0 > lLen) {
        printf("UTIL_CreateOopMsg error\r\n");
        goto ERR_LABEL;
    }

    /* send oop message */
    iFd = m_stClientLogUnixSock.iFd;
    if (-1 == iFd) {
        printf("m_stClientLogUnixSock[CurrentProc %s].iFd error\r\n", acProcName);
        printf("SendClientLog error:%s %u %s\n", pcFileName, ulLineNum, acLogContent);
        goto ERR_LABEL;
    }

    iRet = UTIL_SendOopMsg(iFd, acOopBuf, lLen, DEF_HOST_RECV_LOG_PATH);
    if (OPLK_OK != iRet) {
        printf("UTIL_SendOopMsg %d %s error %s\r\n", iFd, DEF_HOST_RECV_LOG_PATH, acLogContent);
        goto ERR_LABEL;
    }

END_LABLE:

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_SendClientUserLog(ST_USER_LOG_BODY *pstUserLog)
{
    int iRet = OPLK_ERR;
    int iFd = -1;
    char acOopBuf[DEF_LOG_MSG_MAX_LEN] = {0};
    int32_t lReqLen = 0;

    static bool m_bReadProc = false;
    char acProcName[DEF_PROC_NAME_LEN];

    if (NULL == pstUserLog) {
        printf("input NULL == pstUserLog\r\n");
        goto ERR_LABEL;
    }

    if (!m_bReadProc) {
        memset(acProcName, 0, sizeof(acProcName));
        UTIL_GetProcName(acProcName, sizeof(acProcName));
        m_bReadProc = true;
    }

    if (false == __UTIL_GetClientLogEnable()) {
        /*the current process log do not need record*/
        goto END_LABLE;
    }

    /* creat oop message: log body + log header */
    lReqLen = UTIL_CreateOopMsg(acOopBuf, DEF_OOP_MSG_TYPE_REQ, 0, 0,
                                DEF_OID_LOG_ADD_USER_LOG, 0,
                                (char *)pstUserLog, sizeof(ST_USER_LOG_BODY));
    if (0 > lReqLen) {
        printf("UTIL_CreateOopMsg failure %d", lReqLen);
        goto ERR_LABEL;
    }

    iFd = m_stClientLogUnixSock.iFd;
    if (-1 == iFd) {
        printf("m_stClientLogUnixSock[CurrentProc %s].iFd error\r\n", acProcName);
        goto ERR_LABEL;
    }

    /* send oop message */
    iRet = UTIL_SendOopMsg(iFd, acOopBuf, lReqLen, DEF_HOST_RECV_LOG_PATH);
    if (OPLK_OK != iRet) {
        printf("UTIL_SendOopMsg %d %s error %s\r\n", iFd, DEF_HOST_RECV_LOG_PATH, pstUserLog->acRecordMsg);
        goto ERR_LABEL;
    }

END_LABLE:

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
UTIL_RecvHostLog()
{
    /* create thread for process log message */
    __UTIL_ProcessLogMsg();

    /* create thread for loop to receive log message */
    __UTIL_RecvLogMsg();

    /* Create thread to record system infor and save log auto. */
    __UTIL_SaveLog();

    return OPLK_OK;
}

int
UTIL_ListOplkLogs(char *pcFileName,
                  EN_UTIL_LOG_SEVERITY enSeverity,
                  uint32_t *pulLogCount,
                  int (*pfCallbackFormatLog)(void *pArg, ST_OPLK_LOG_BODY *pstLogBody))
{
    FILE *pstLogFd = NULL;
    FILE *fpCsvFd = NULL;
    ST_OPLK_LOG_HEADER stLogHeader;
    ST_OPLK_LOG_BODY stLogBody;
    uint32_t ulLen = 0;
    uint32_t ulLogCount = 0;
    uint32_t ulReportPos = 0;
    uint32_t i = 0;
    int iRet = OPLK_ERR;
    char acBuffer[DEF_BUF_SIZE_512] = {0};

    if (NULL == pcFileName || NULL == pulLogCount) {
        printf("NULL == pcFileName || NULL == pulLogCount\r\n");
        goto ERR_LABEL;
    }

    pstLogFd = fopen(pcFileName, "r");
    if (NULL == pstLogFd) {
        printf("NULL == pstLogFd\r\n");
        goto ERR_LABEL;
    }

    /* get to the file beginning */
    fseek(pstLogFd, 0, SEEK_SET);
    if (fread(&stLogHeader, sizeof(ST_OPLK_LOG_HEADER), 1, pstLogFd) != 1) {
        printf("fread error\r\n");
        goto ERR_LABEL;
    }

    /* if the log overwrite, the end is the limit number OPLK_LOG_MSG_MAX_NUM */
    if (stLogHeader.ulStartIndex > stLogHeader.ulEndIndex) {
        ulLogCount = DEF_OPLK_LOG_MAX_NUM;
    } else {
        /* the log file end is the end index */
        ulLogCount = stLogHeader.ulEndIndex;
    }

    /* printf("\r\n_HEADER %ld %d %d %d\r\n", sizeof(ST_OPLK_LOG_HEADER), ulLogCount, stLogHeader.ulStartIndex, stLogHeader.ulEndIndex); */

    fpCsvFd = fopen(DEF_OPLK_LOG_CSV_FILE, "w");
    ulReportPos = stLogHeader.ulStartIndex;

    for (i = 0; i < ulLogCount; i++) {

        ulLen = sizeof(ST_OPLK_LOG_HEADER) + ulReportPos * DEF_OPLK_LOG_BODY_LEN;
        fseek(pstLogFd, ulLen, SEEK_SET);

        if ((iRet = fread(acBuffer, 1, DEF_OPLK_LOG_BODY_LEN, pstLogFd)) != DEF_OPLK_LOG_BODY_LEN) {
            printf("fread stLogBody %d\r\n", iRet);
            goto ERR_LABEL;
        }

        /* sizeof stLogBody == DEF_OPLK_LOG_BODY_LEN */
        memcpy(&stLogBody, acBuffer, DEF_OPLK_LOG_BODY_LEN);

        /*format the log and write to the csv file*/
        if (OPLK_OK != pfCallbackFormatLog(fpCsvFd, &stLogBody)) {
            printf("Call back Format Log error\r\n");
            goto ERR_LABEL;
        }

        ulReportPos++;
        /*start at uiStartIndex, if DEF_OPLK_LOG_MAX_NUM == iReportPos, than iReportPos = 0, Ergodic search ulEndIndex*/
        if (DEF_OPLK_LOG_MAX_NUM == ulReportPos) {
            ulReportPos = 0;
        }

        if (ulReportPos == stLogHeader.ulEndIndex) {
            i++;
            break;
        }
    }

    DEF_SAFE_FCLOSE(pstLogFd);
    DEF_SAFE_FCLOSE(fpCsvFd);
    *pulLogCount = i;
    return OPLK_OK;

ERR_LABEL:
    DEF_SAFE_FCLOSE(pstLogFd);
    DEF_SAFE_FCLOSE(fpCsvFd);
    *pulLogCount = -1;
    return OPLK_ERR;
}


char *
UTIL_LogSeverityEnumToStr(uint8_t ucServrity)
{
    char *pcProcStr = NULL;

    switch (ucServrity) {
    case EN_LOG_EMERGENCY:
        pcProcStr = DEF_LOG_SERVERITY_EMERGENCY;
        break;
    case EN_LOG_ALERT:
        pcProcStr = DEF_LOG_SERVERITY_ALERT;
        break;
    case EN_LOG_CRITICAL:
        pcProcStr = DEF_LOG_SERVERITY_CRITICAL;
        break;
    case EN_LOG_ERROR:
        pcProcStr = DEF_LOG_SERVERITY_ERROR;
        break;
    case EN_LOG_WARNING:
        pcProcStr = DEF_LOG_SERVERITY_WARNING;
        break;
    case EN_LOG_NOTICE:
        pcProcStr = DEF_LOG_SERVERITY_NOTICE;
        break;
    case EN_LOG_TRACE:
        pcProcStr = DEF_LOG_SERVERITY_TRACE;
        break;
    case EN_LOG_DEBUG:
        pcProcStr = DEF_LOG_SERVERITY_DEBUG;
        break;
    default:
        pcProcStr = NULL;
        break;
    }

    return pcProcStr;

}


void
UTIL_SetHostExitLoop(bool bExitLoop)
{
    m_bExitHostLoop = bExitLoop;
}


int
UTIL_WriteSyslog(int iFacility,
                 EN_UTIL_LOG_SEVERITY enSeverity,
                 const char *pcFileName,
                 uint32_t ulLineNum,
                 const char *pcLogInfo, ...)
{
#define DEF_MEIWU_LOG_MSG_LEN                 (384)
    va_list args;
    char acLogContent[DEF_BUF_SIZE_1024]; /*Record Msg */
    char acStrRecord[DEF_BUF_SIZE_1024 + DEF_BUF_SIZE_256] = {0};
    char acProcName[DEF_PROC_NAME_LEN] = {0};

    if (NULL == pcFileName || NULL == pcLogInfo) {
        printf("input NULL == pcFileName || NULL == pcLogInfo\r\n");
        goto ERR_LABEL;
    }

    if (false == __UTIL_GetClientLogEnable()) {
        /*the current process log do not need record */
        goto END_LABLE;
    }

    /* check record level, if the log severity < the setting severity, then do not record the log */
    /* if (enSeverity > EN_LOG_NOTICE) { */
    if (enSeverity > __UTIL_GetClientLogSeverity()) {
        goto END_LABLE;
    }

    memset(acProcName, 0x00, DEF_PROC_NAME_LEN);
    UTIL_GetProcName(acProcName, DEF_PROC_NAME_LEN);

    memset(acLogContent, 0, DEF_BUF_SIZE_1024);

    va_start(args, pcLogInfo);
    vsnprintf(acLogContent, sizeof(acLogContent) - 1, pcLogInfo, args);
    va_end(args);

    if ((LOG_LOCAL6 == iFacility) || (LOG_LOCAL3 == iFacility)) {
        openlog(acProcName, LOG_PID, iFacility);
        snprintf(acStrRecord, sizeof(acStrRecord), "%s\n", acLogContent);
    } else {
        openlog(acProcName, LOG_PID, 0);

        /* format log */
        snprintf(acStrRecord, sizeof(acStrRecord),
                 "%lu [%s][%s:%d] %s\n",
                 UTIL_GetTimeMs(),
                 UTIL_LogSeverityEnumToStr(enSeverity),
                 pcFileName,
                 ulLineNum,
                 acLogContent);
    }

    syslog(enSeverity, "%s", acStrRecord);

    closelog();

END_LABLE:

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}


void
UTIL_SetProcLogEnable(bool bEnable)
{
    m_stClientLogUnixSock.ucLogEnable = bEnable;

    return;
}

void
UTIL_SetHostLogSaveDevice(EN_UTIL_LOG_SAVE_DEVICE enDevice)
{
    m_enSaveDevice = enDevice;
    return;
}

int
UTIL_SaveClientLog(void)
{
    int iFd = -1;
    int iRet = OPLK_ERR;
    char acOopBuf[DEF_LOG_MSG_MAX_LEN] = {0};
    int32_t lLen = 0;
    int32_t lEnable = true;     /* enable */

    iFd = m_stClientLogUnixSock.iFd;
    if (-1 == iFd) {
        printf("m_stClientLogUnixSockiFd %d error\r\n", iFd);
        goto ERR_LABEL;
    }

    /* creat oop message: log body + log header */
    lLen = UTIL_CreateOopMsg(acOopBuf, DEF_OOP_MSG_TYPE_REQ, 0, 0,
                             DEF_OID_LOG_SAVE_LOG, 0, (char *)&lEnable, sizeof(int32_t));
    if (0 > lLen) {
        printf("UTIL_CreateOopMsg error\r\n");
        goto ERR_LABEL;
    }

    /* send oop message */
    iRet = UTIL_SendOopMsg(iFd, acOopBuf, lLen, DEF_HOST_RECV_LOG_PATH);
    if (OPLK_OK != iRet) {
        printf("UTIL_SaveClientLog %d %s error\r\n", iFd, DEF_HOST_RECV_LOG_PATH);
        goto ERR_LABEL;
    }

    return OPLK_OK;

ERR_LABEL:

    return OPLK_ERR;
}

int
LOG_SetLogHashEnable(bool bEnable)
{
    m_bHashTableEnable = bEnable;
    return 0;
}
