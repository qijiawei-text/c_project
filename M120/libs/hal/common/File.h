#pragma once

#include "DevInternal.h"
#include "InterfaceDriver.h"
#include "RspHandler.h"


#define HEADER_KEYWORD      "BOARD_CONFIG"
#define HEADER_VERSION      "1.01.0001"

typedef enum
{
    FILE_TYPE_ORG = 1,
    FILE_TYPE_TAR,
    FILE_TYPE_MAX
}FILE_TYPE_EN;

#define LEN_FILE_DESCR      (64)
typedef struct FILE_INFO_st
{
    int iAddr;
    int iLen;
    uint32_t uiBodyCrc;
    char acDescr[LEN_FILE_DESCR];
}FILE_INFO_ST;

#define LEN_HEADER          (256)
#define LEN_HEADER_KEYWORD  (16)
#define LEN_HEADER_VERSION  (16)
#define MAX_FILE_COUNT      (1)
#define LEN_HEADER_RES      (LEN_HEADER - LEN_HEADER_KEYWORD \
                             - LEN_HEADER_VERSION \
                             - 4 * sizeof(int)        \
                             - MAX_FILE_COUNT * sizeof(FILE_INFO_ST))

typedef struct CONF_FILE_HEADER_st
{
    char acKeyWord[LEN_HEADER_KEYWORD];
    char acVersion[LEN_HEADER_VERSION];
    int iFileLen;                                   /* File total len.                          */
    int iFileCount;
    int iType;
    FILE_INFO_ST astInfo[MAX_FILE_COUNT];
    char acResv[LEN_HEADER_RES];
    uint32_t uiHeaderCrc;                               /* Header crc32.                            */
}CONF_FILE_HEADER_ST;



/* @defgroup  Xmodem Protocol Definitions */
#define XMD_SOH                         (0x01)                              /* Start for 128B                                          */
#define XMD_STX                         (0x02)                              /* Start for 1KB                                           */
#define XMD_EOT                         (0x04)                              /* End of Transfer                                         */
#define XMD_ACK                         (0x06)                              /* Acknowledge                                             */
#define XMD_NAK                         (0x15)                              /* No acknowledge                                          */
#define XMD_CAN                         (0x18)                              /* Cancel                                                  */
#define XMD_CRC                         ('C')

/* @defgroup  Xmodem EOT Index */
typedef enum
{
    XMD_EOT_IDX_0 = 0,
    XMD_EOT_IDX_1,
    XMD_EOT_IDX_2,

    XMD_EOT_IDX_MAX

} EN_XMD_EOT_IDX;

/* @defgroup  Xmodem Package Index */
typedef enum
{
    XMD_PKG_IDX_TYPE = 0,
    XMD_PKG_IDX_SEQ,
    XMD_PKG_IDX_SEQCK,
    XMD_PKG_IDX_DATA,
    XMD_PKG_IDX_END_128B = 130,
    XMD_PKG_IDX_128B_CK0 = 131,
    XMD_PKG_IDX_128B_CK1 = 132,
    XMD_PKG_IDX_END_1KB = 1026,
    XMD_PKG_IDX_1KB_CK0 = 1027,
    XMD_PKG_IDX_1KB_CK1 = 1028

} EN_XMD_PKG_IDX;

#define XMD_RETRY_MAX                   (10)
#define XMD_RETRY_INTV                  (20)
#define XMD_WAIT_INTV                   (20)
#define XMD_TIME_OUT                    (700)

/* @brief  Xmodem Check Type */
#define XMD_CHK_TYPE_CRC_16             ('C')
#define XMD_CHK_TYPE_CHKSUM             (0x15)
/* @defgroup  Xmodem Package Size */
#define XMD_PKG_SIZE_1KB                (1024)
#define XMD_PKG_SIZE_128B               (128)

#define XMD_PKG_LEN                     (1024 + 5)


class CFile
{
public:
    CFile()
    {
    }
    ~CFile()
    {
    }

    static char *Malloc(const char *pcFileName, int &iFileLen);
    static bool Write(const char *pcFileName, uint8_t *pucBuf, uint32_t ulLen);
    static bool InitHeader(CONF_FILE_HEADER_ST &rstHeader);
    static bool CheckHeader(const CONF_FILE_HEADER_ST &c_rstHeader);
    static int GetLength(const char *pcFilePath);
    static bool XModem(const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, const std::string &c_rstaStartCmd, uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);
};

