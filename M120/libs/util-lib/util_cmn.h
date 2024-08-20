/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_cmn.h
* Description:   common
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __UTIL_CMN_H__
#define __UTIL_CMN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup  Pseudocode
 * @{
 */
#ifdef __SOURCE_INSIGHT__
/* Pseudo typedef                                                            --Prefix--           */
typedef int8_t int8_t;                                                      /* c                                                       */
typedef uint8_t uint8_t;                                                    /* uc                                                      */
typedef int16_t int16_t;                                                    /* s                                                       */
typedef uint16_t uint16_t;                                                  /* us                                                      */
typedef int32_t int32_t;                                                    /* l                                                       */
typedef uint32_t uint32_t;                                                  /* ul                                                      */
typedef int64_t int64_t;                                                    /* ll                                                      */
typedef uint64_t uint64_t;                                                  /* ull                                                     */

typedef bool bool;                                                          /* b                                                       */
typedef char char;                                                          /* c                                                       */
typedef int int;                                                            /* i                                                       */
typedef float float;                                                        /* f                                                       */
typedef double double;                                                      /* d                                                       */
typedef size_t size_t;                                                      /* n                                                       */
#endif
/**
 * @}
 */

/* Gets the value of 0~7 bits in 1 bytes  */
#define BIT_0_VALUE(x)                   ((x) & 0x01)
#define BIT_1_VALUE(x)                   (((x) >> 1) & 0x01)
#define BIT_2_VALUE(x)                   (((x) >> 2) & 0x01)
#define BIT_3_VALUE(x)                   (((x) >> 3) & 0x01)
#define BIT_4_VALUE(x)                   (((x) >> 4) & 0x01)
#define BIT_5_VALUE(x)                   (((x) >> 5) & 0x01)
#define BIT_6_VALUE(x)                   (((x) >> 6) & 0x01)
#define BIT_7_VALUE(x)                   (((x) >> 7) & 0x01)
#define BIT_8_VALUE(x)                   (((x) >> 8) & 0x01)
#define BIT_9_VALUE(x)                   (((x) >> 9) & 0x01)
#define BIT_15_VALUE(x)                  (((x) >> 15) & 0x01)

/* Set the value of 0~7 bits in 1 bytes  */
#define SET_BIT_0_VALUE(x)                   ((x) |= 0x01)
#define SET_BIT_1_VALUE(x)                   ((x) |= (0x01 << 1))
#define SET_BIT_2_VALUE(x)                   ((x) |= (0x01 << 2))
#define SET_BIT_3_VALUE(x)                   ((x) |= (0x01 << 3))
#define SET_BIT_4_VALUE(x)                   ((x) |= (0x01 << 4))
#define SET_BIT_5_VALUE(x)                   ((x) |= (0x01 << 5))
#define SET_BIT_6_VALUE(x)                   ((x) |= (0x01 << 6))
#define SET_BIT_7_VALUE(x)                   ((x) |= (0x01 << 7))
#define SET_BIT_8_VALUE(x)                   ((x) |= (0x01 << 8))

#define ROUND_MUTI_100000(value)        ((int32_t)(round((value) * 100000.0)))
#define ROUND_MUTI_1000(value)          ((int32_t)(round((value) * 1000.0)))
#define ROUND_MUTI_100(value)           ((int32_t)(round((value) * 100.0)))
#define ROUND_MUTI_10(value)            ((int32_t)(round((value) * 10.0)))

#ifndef MIN
# define MIN(a, b)  ((a) <= (b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b)  ((a) >= (b) ? (a) : (b))
#endif



#define DEF_BUF_SIZE_1024               (1024)
#define DEF_BUF_SIZE_512                (512)
#define DEF_BUF_SIZE_256                (256)
#define DEF_BUF_SIZE_128                (128)
#define DEF_BUF_SIZE_64                 (64)
#define DEF_BUF_SIZE_32                 (32)
#define DEF_BUF_SIZE_16                 (16)

#define DEF_SIZE_BM_U8                  (8)
#define DEF_SIZE_BM_U16                 (16)
#define DEF_SIZE_BM_U32                 (32)

#define OPLK_ERR                        (-1)
typedef enum
{
    /* match the sysrepo error code */
    OPLK_OK = 0,            /**< No error. */
    ERR_INVAL_ARG,          /**< Invalid argument. */
    ERR_NOMEM,              /**< Not enough memory. */
    ERR_NOT_FOUND,          /**< Item not found. */
    ERR_INTERNAL,           /**< Other internal error. */
    ERR_INIT_FAILED,        /**< Sysrepo infra initialization failed. */
    ERR_IO,                 /**< Input/Output error. */
    ERR_DISCONNECT,         /**< The peer disconnected. */
    ERR_MALFORMED_MSG,      /**< Malformed message. */
    ERR_UNSUPPORTED,        /**< Unsupported operation requested. */
    ERR_UNKNOWN_MODEL,      /**< Request includes unknown schema */
    ERR_BAD_ELEMENT,        /**< Unknown element in existing schema */
    ERR_VALIDATION_FAILED,  /**< Validation of the changes failed. */
    ERR_OPERATION_FAILED,   /**< An operation failed. */
    ERR_DATA_EXISTS,        /**< Item already exists. */
    ERR_DATA_MISSING,       /**< Item does not exists. */
    ERR_UNAUTHORIZED,       /**< Operation not authorized. */
    ERR_INVAL_USER,         /**< Invalid username. */
    ERR_LOCKED,             /**< Requested resource is already locked. */
    ERR_TIME_OUT,           /**< Time out has expired. */
    ERR_RESTART_NEEDED,     /**< Sysrepo Engine restart is needed. */
    ERR_VERSION_MISMATCH,   /**< Incompatible client library used to communicate with sysrepo. */

    /* arg error code */
    ERR_OVERFLOW_ARG = 40,

    /* auth error code */
    ERR_REMOTE_NO_SERVERS = 50,      /**< User auth remote no servers. */
    ERR_MAX_SESSION_LIMIT,       /**< Login max session limited. */

    /* file/memory error code */
    ERR_FILE_NOT_EXIST = 60,
    ERR_READ_FILE,
    ERR_WRITE_FILE,
    ERR_SHM_NOT_ACCESS,

    /* cmd error code */
    ERR_CMD_DELAY = 70,
    ERR_CMD_INPROGRESS,

    /* system status error code */
    ERR_NO_PLUGGED = 80,
    ERR_UPGRADING,
    ERR_LOADING,
    ERR_SAVE_FAILED,
    ERR_DOWNLOAD_FAILED,

    /* communication error code */
    ERR_COMMUNICATION = 90,

} EN_OPLK_ERR_CODE;

#define DEF_1KB                         (1024u)
#define DEF_1MB                         (1024 * 1024u)

/**
 * @defgroup  Version Length
 * @{
 */
#define DEF_VER_LEN_MAX                 (16u)
#define DEF_STR_LEN_MAX                 (32u)
/**
 * @}
 */

/**
 * @defgroup  Safe Operation
 * @{
 */
#define DEF_SAFE_FREE(ptr)          \
    do {                            \
        if ((ptr) != NULL) {        \
            free(ptr);              \
            (ptr) = NULL;           \
        }                           \
    } while (0)

#define DEF_SAFE_FCLOSE(ptr)        \
    do {                            \
        if ((ptr) != NULL) {        \
            fclose(ptr);            \
            (ptr) = NULL;           \
        }                           \
    } while (0)

#define DEF_SAFE_CLOSE(fd)          \
    do {                            \
        if ((fd) > 0) {             \
            close(fd);              \
            (fd) = -1;              \
        }                           \
    } while (0)

#define DEF_SAFE_PCLOSE(ret, ptr)   \
    do {                            \
        if (ptr != NULL) {          \
            ret = pclose(ptr);      \
            (ptr) = NULL;           \
        }                           \
    } while (0)

#define DEF_SAFE_CLOSEDIR(ptr)      \
    do {                            \
        if (ptr != NULL) {          \
            closedir(ptr);          \
            (ptr) = NULL;           \
        }                           \
    } while (0)

/**
 * @}
 */

/**
 * @defgroup  Check NULL ARG with return
 * @{
 */
#define DEF_CHECK_NULL_ARG_INTERNAL(ARG)    \
    if (NULL == ARG) {                      \
        LOG_WRITE(EN_LOG_ERROR, "NULL value detected for %s argument of %s", # ARG, __func__); \
        return OPLK_ERR;                    \
    }

#define DEF_CHECK_NULL_ARG(ARG)             \
    do {                                    \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG)    \
    } while (0)

#define DEF_NULL_ARG2(ARG1, ARG2)           \
    do {                                    \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG1)   \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG2)   \
    } while (0)

#define DEF_NULL_ARG3(ARG1, ARG2, ARG3)     \
    do {                                    \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG1)   \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG2)   \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG3)   \
    } while (0)

#define DEF_CHECK_NULL_ARG4(ARG1, ARG2, ARG3, ARG4) \
    do {                                            \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG1)           \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG2)           \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG3)           \
        DEF_CHECK_NULL_ARG_INTERNAL(ARG4)           \
    } while (0)
/**
 * @}
 */

/**
 * @defgroup  Sleep
 * @{
 */
#define DEF_SLEEP_US(ulUs)          \
    do {                            \
        usleep(ulUs);                \
    } while (0)

#define DEF_SLEEP_MS(ulMs)         \
    do {                            \
        usleep(ulMs * 1000);         \
    } while (0)

#define DEF_SLEEP_S(ulS)           \
    do {                            \
        sleep(ulS);                  \
    } while (0)
/**
 * @}
 */

/**
 * @defgroup  Redefine strncmp
 * @{
 */
#define DEF_STRNCMP(pcSrc, pcDst)                   \
    strncmp(pcSrc, pcDst, sizeof(pcSrc) - 1)

#define DEF_STRNCPY(pcDst, pcSrc)                   \
    do {                                            \
        strncpy(pcDst, pcSrc, sizeof(pcDst) - 1);   \
        pcDst[sizeof(pcDst) - 1] = 0;               \
    } while (0)
/**
 * @}
 */

#define DEF_IS_NUMBER(c)                           ((c >= '0' && c <= '9') ? 1 : 0)

#define DEF_IS_LOWER_ALPHA                         (0x1000)
#define DEF_IS_UPPER_ALPHA                         (0x0100)
#define DEF_IS_DIGIT                               (0x0010)
#define DEF_IS_PRINT                               (0X0001)

#define DEF_PROC_NAME_LEN                          (64)

/**
 * @defgroup  MTD
 * @{
 */
#define DEF_UPGD_MTD_MAX                (32u)

typedef enum {
    EN_MTD_NO_A__RCW = 0,
    EN_MTD_NO_A_BOOT,
    EN_MTD_NO_A__ENV,
    EN_MTD_NO_A__PPA,
    EN_MTD_NO_A_FMAN,
    EN_MTD_NO_A_RSV0,
    EN_MTD_NO_A_HEAD,
    EN_MTD_NO_A___OS,
    EN_MTD_NO_A__APP,
    EN_MTD_NO_A__LOG,
    EN_MTD_NO_A___PM,
    EN_MTD_NO_A_RSV1,

    EN_MTD_NO_A_MAX,

    EN_MTD_NO_B__RCW = EN_MTD_NO_A_MAX,
    EN_MTD_NO_B_BOOT,
    EN_MTD_NO_B__ENV,
    EN_MTD_NO_B__PPA,
    EN_MTD_NO_B_FMAN,
    EN_MTD_NO_B_RSV0,
    EN_MTD_NO_B_HEAD,
    EN_MTD_NO_B___OS,
    EN_MTD_NO_B__APP,

    EN_MTD_NO_LOG,
    EN_MTD_NO_PM,

    EN_MTD_NO_MAX

} EN_MTD_NO;
/**
 * @}
 */

/**
 * @defgroup  MTD Length
 * @{
 */
#define DEF_MTD_LEN_MAX                 (256 * DEF_1MB)

#define DEF_NOR_FLASH_SECT_LEN          (128 * DEF_1KB)

/* Nor Flash A                                                                                    */
#define DEF_MTD_LEN_A__RCW              (1 * DEF_NOR_FLASH_SECT_LEN)
#define DEF_MTD_LEN_A_BOOT              (1 * DEF_1MB)
#define DEF_MTD_LEN_A__ENV              (2 * DEF_NOR_FLASH_SECT_LEN)
#define DEF_MTD_LEN_A__PPA              (2 * DEF_NOR_FLASH_SECT_LEN)
#define DEF_MTD_LEN_A_FMAN              (2 * DEF_NOR_FLASH_SECT_LEN)
#define DEF_MTD_LEN_A_HEAD              (1 * DEF_NOR_FLASH_SECT_LEN)
#define DEF_MTD_LEN_A___OS              (32 * DEF_1MB)
#define DEF_MTD_LEN_A__APP              (48 * DEF_1MB)

/* Nor Flash B                                                                                    */
#define DEF_MTD_LEN_B__RCW              (DEF_MTD_LEN_A__RCW)
#define DEF_MTD_LEN_B_BOOT              (DEF_MTD_LEN_A_BOOT)
#define DEF_MTD_LEN_B__ENV              (DEF_MTD_LEN_A__ENV)
#define DEF_MTD_LEN_B__PPA              (DEF_MTD_LEN_A__PPA)
#define DEF_MTD_LEN_B_FMAN              (DEF_MTD_LEN_A_FMAN)
#define DEF_MTD_LEN_B_HEAD              (DEF_MTD_LEN_A_HEAD)
#define DEF_MTD_LEN_B___OS              (DEF_MTD_LEN_A___OS)
#define DEF_MTD_LEN_B__APP              (DEF_MTD_LEN_A__APP)

/* Misc                                                                                           */
#define DEF_MTD_LEN_RESV                (10 * DEF_1MB)
#define DEF_MTD_LEN__LOG                (16 * DEF_1MB)
#define DEF_MTD_LEN___PM                (2 * DEF_1MB)
/**
 * @}
 */

/**
 * @defgroup  Union 2B/4B: MSB
 * @{
 */
typedef union {
    uint16_t usData;

    struct {
        uint8_t ucByteH;
        uint8_t ucByteL;
    };
} UN_2B;

typedef union {
    uint32_t ulData;

    struct {
        uint16_t usShortH;
        uint16_t usShortL;
    };

    struct {
        uint8_t ucByteH;
        uint8_t ucByteMH;
        uint8_t ucByteML;
        uint8_t ucByteL;
    };

    struct {
        uint8_t ucX;
        uint8_t ucY;
        uint16_t usZ;
    };

} UN_4B;
/**
 * @}
 */

/**
 * @defgroup  Version Integer to String
 * @{
 */
static inline char *
_VER_BCD_TO_XYZ_(uint32_t ulVer, char *pcVer)
{
    memset(pcVer, 0, DEF_VER_LEN_MAX);
    sprintf(pcVer, "%x.%02x.%04x", ulVer >> 24, (ulVer >> 16) & 0xFF, ulVer & 0xFFFF);

    return pcVer;
}

static inline char *
_VER_BCD_TO_XY_(uint32_t ulVer, char *pcVer)
{
    memset(pcVer, 0, DEF_VER_LEN_MAX);
    sprintf(pcVer, "%x.%02x", ulVer >> 24, (ulVer >> 16) & 0xFF);

    return pcVer;
}
/**
 * @}
 */

/**
 * @defgroup  Version String to Integer
 * @{
 */
static inline uint32_t
_VER_XYZ_TO_BCD_(char *pcVer)
{
    UN_4B un4B;


    un4B.ulData = 0;
    sscanf(pcVer, "%hhx.%hhx.%hx", &un4B.ucX, &un4B.ucY, &un4B.usZ);

    return un4B.ulData;
}

static inline uint32_t
_VER_XY_TO_BCD_(char *pcVer)
{
    UN_4B un4B;


    un4B.ulData = 0;
    sscanf(pcVer, "%hhx.%hhx", &un4B.ucX, &un4B.ucY);

    return un4B.ulData;
}
/**
 * @}
 */

/**
 * @defgroup  double min/max
 * @{
 */
#define DMIN(a, b)   (true == UTIL_IsDoubleLess((a), (b)) ? (a) : (b))
#define DMAX(a, b)   (true == UTIL_IsDoubleMore((a), (b)) ? (a) : (b))
/**
 * @}
 */

extern int UTIL_ExecuteCmd(char *pcCmd, char *pcResp, uint32_t ulRespLen, uint32_t ulTimeoutMs);
extern int UTIL_ExceCmdUnresp(char *pcCmd, char *pcResp, uint32_t ulRespLen, uint32_t ulTimeoutMs);
extern int UTIL_ExecuteCmdLargeResp(char *pcCmd, char *pcResp, uint32_t ulRespLen, uint32_t ulTimeoutMs);

extern int UTIL_System(char *pcCmdBuf);

extern int UTIL_ExportProcessInfo(char *pcProcessName);

extern char *UTIL_UpperStr(char *pcStr);
extern char *UTIL_LowerStr(char *pcStr);
extern void UTIL_DelDigital(char acX[]);

extern int UTIL_ChkStrSpecChr(char *pcStr);
extern int UTIL_ChkStrNumberLetter(char *pcStr);


extern int UTIL_CheckStrWithAllSpecChars(char *pcString);
extern int UTIL_TranslateStrForExpect(const char *pcIn, char *pcOut, size_t nOutLen);

extern int UTIL_GetProcName(char *pcProcName, uint32_t ulLen);

extern void UTIL_AscToBcd(char *pcASC, char *pcBCD, uint32_t ulLen);
extern char *UTIL_BcdToAsc(char *pcBCD, uint32_t ulLen, char *pcASC);
extern int UTIL_AscToHex(const char *pcstr, char *pcHexStr, uint32_t ulLen);
extern int UTIL_StrToInt(char *pcSrc, uint32_t ulMultiple, int *piNum);
extern int UTIL_StrToFloat(char *pcSrc, size_t nMaxFractionDigits, double *pdNum);

extern char *UTIL_StrRpc(char *pcStr, char *pcOldStr, char *pcNewStr);

extern char *UTIL_TimeToStr(time_t timeT, char *time_str);

extern int UTIL_ReadMtd(uint8_t ucMtd, uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen);
extern int UTIL_SaveMtd(uint8_t ucMtd, uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen);
extern bool UTIL_ChkProcExist(char *pcProcName);

extern float UTIL_Ntohf(float fIn);
extern bool UTIL_htonl(uint32_t *puiBufIn, uint32_t *puiBufOut, uint32_t uiLen);
extern bool UTIL_ntohs(uint16_t *pusBufIn, uint16_t *pusBufOut, uint32_t uiLen);
extern bool UTIL_ntohl(uint32_t *puiBufIn, uint32_t *puiBufOut, uint32_t uiLen);

extern bool UTIL_IsDoubleEqual(double dX, double dY);
extern bool UTIL_IsDoubleMore(double dX, double dY);
extern bool UTIL_IsDoubleLess(double dX, double dY);
extern bool UTIL_IsDoubleMoreEqual(double dX, double dY);
extern bool UTIL_IsDoubleLessEqual(double dX, double dY);
extern int UTIL_GetLocalIp(const char *pcEth, char *pcIp, size_t nIpSize);
extern int UTIL_GetLocalMac(const char *pcEth, char *pcMac, size_t nMacSize);

/**************************************************************************************************/
/* definition of memory state                                                                     */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef struct {
    int64_t lMemAvailable;
    int64_t lMemFree;
    int64_t lMemTotal;
    int64_t lMemBuffers;
    int64_t lMemCached;
    int64_t lMemUtilized;
} ST_MEM_STAT;
extern int UTIL_GetMeminfo(ST_MEM_STAT *pstMemStat);
extern int UTIL_GetCpu1MinAvg(double *pdValue);
/* @ }                                                                                            */

extern int UTIL_SetRunningThreadName(const char *pcThreadName);

#ifdef __cplusplus
}
#endif

#endif
