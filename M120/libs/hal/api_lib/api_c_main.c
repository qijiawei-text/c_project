
#include <stdio.h>
#include "DevBoardCApi.h"
#include "DevDbgCApi.h"
#include "DevEdfaCApi.h"
#include "DevWssCApi.h"
#include "DevSlotCApi.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <mtd/mtd-user.h>
#include <math.h>
#include <sys/time.h>
#include "DevData.h"
/* 重新命名变量名 */
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef short int16_t;



/* 定义函数执行返回状态 */
#define STATE_OK      0
#define STATE_ERROR   1

#define MAC_ADDR_LEN                (17)


#define BOARD_FAN_NUM             4
#define BOARD_PSU_NUM             2
#define BOARD_LED_NUM             7
#define BOARD_TEMP_NUM            2


/**-------------------------------------------------------------------------------------------------
 * @defgroup PRBS
 * @{
 */
#define DEF_PRBS_7     (7)
#define DEF_PRBS_11    (11)
#define DEF_PRBS_15    (15)
/**
 * DEF_PRBS_7  BUFFERSIZE = 128
 * DEF_PRBS_11  BUFFERSIZE = 2048
 * DEF_PRBS_15  BUFFERSIZE = 32768
 */

#define DEF_PRBS_MODE   DEF_PRBS_11
#define BUFFERSIZE      2048

/* Msata interface path */
#define MSATA_PATH_ONE          "/home/oplink/log/test1.txt"
#define MSATA_PATH_TWO          "/home/oplink/datastore1/test1.txt"
#define MSATA_PATH_THREE        "/home/oplink/datastore2/test1.txt"


#define READ_SCC_MFG_PATH       "/home/oplink/datastore1/scc.conf"
#define SCC_MFG_PATH            "/home/oplink/datastore1/scc1.conf"

#define READ_RPC_MFG_PATH       "/home/oplink/datastore1/Chassis.conf"
#define RPC_MFG_PATH            "/home/oplink/datastore1/Chassis1.conf"

#define READ_FAN_MFG_PATH       "/home/oplink/datastore1/fan_mfg1.conf"
#define FAN_MFG_PATH            "/home/oplink/datastore1/fan_mfg2.conf"

#define READ_SLC_MFG_PATH       "/home/oplink/datastore1/optical.conf"
#define SLC_MFG_PATH            "/home/oplink/datastore1/optical1.conf"

/* Manufacture info */
#define MODULE_MFG_NAME             "MODULE_MFG_NAME"
#define MODULE_PRODUCT_NAME         "MODULE_PRODUCT_NAME"
#define MODULE_TYPE                 "MODULE_TYPE"
#define MODULE_SW_VER               "MODULE_SW_VER"
#define MODULE_FW_VER               "MODULE_FW_VER"
#define MODULE_HW_REV               "MODULE_HW_REV"
#define MODULE_PCB_REV              "MODULE_PCB_REV"
#define MODULE_SN                   "MODULE_SN"
#define MODULE_PART_NO              "MODULE_PART_NO"
#define MODULE_MFG_DATE             "MODULE_MFG_DATE"
#define MODULE_CAL_DATE             "MODULE_CAL_DATE"
#define MODULE_CLEI_CODE            "MODULE_CLEI_CODE"
#define MODULE_SUB_BOARD_REV        "MODULE_SUB_BOARD_REV"
#define MAC_ADDRESS_ETH0            "MAC_ADDRESS_ETH0"
#define MAC_ADDRESS_ETH1            "MAC_ADDRESS_ETH1"
#define MAC_ADDRESS_ETH2            "MAC_ADDRESS_ETH2"
#define MAC_ADDRESS_ETH3            "MAC_ADDRESS_ETH3"
#define MAC_ADDRESS_ETH4            "MAC_ADDRESS_ETH4"
#define MAC_ADDRESS_ETH5            "MAC_ADDRESS_ETH5"
/* fnc additional */
#define MODULE_UNIT_NAME            "MODULE_UNIT_NAME"
#define MODULE_FC_PN                "MODULE_FC_PN"
#define MODULE_FC_ISS               "MODULE_FC_ISS"
#define MODULE_FUJITSU_PN           "MODULE_FUJITSU_PN"


#define DELAY_EEPROM_MS                  1000
#define DELAY_GEISTATUS_MS               10000


#define DEF_SAFE_FCLOSE(ptr)        \
    do {                            \
        if ((ptr) != NULL) {        \
            fclose(ptr);            \
            (ptr) = NULL;           \
        }                           \
    } while (0)


#define SCC_EEPROM_IDX       0
#define RPC_EEPROM_IDX       1


#define FAN0_EEPROM_IDX      3
#define FAN1_EEPROM_IDX      4
#define FAN2_EEPROM_IDX      5
#define FAN3_EEPROM_IDX      6
#define SCC_EEPROM_ADDR             0x1000
#define RPC_EEPROM_ADDR             0x1000

#define FAN0_EEPROM_ADDR            0x1000
#define FAN1_EEPROM_ADDR            0x1000
#define FAN2_EERPOM_ADDR            0x1000
#define FAN3_EEPROM_ADDR            0x1000


#define EEPROM_NUM                  4


#define REG_SCC_CPLD_V1P1_ADC   0x1030
#define REG_SCC_CPLD_V1P5_ADC   0x1034
#define REG_SCC_CPLD_V1P2_ADC   0x1038
#define REG_SCC_CPLD_V1P8_ADC   0x103c
#define REG_SCC_CPLD_V1P0_ADC   0x1040
#define REG_SCC_CPLD_V2P5_ADC   0x1044

typedef struct {
    unsigned char ucStandard;                /* PRBS7 PRBS11 PRBS15                                */
    unsigned int ulSeqLen;                  /* sequence cycle bits length                          */
} ST_PRBS_SEQ;



typedef struct {
    uint32_t idx;
    uint32_t uiAddr;
}ST_EEPROM;



static ST_PRBS_SEQ m_stPrbs[] = {
    {DEF_PRBS_7, 127},
    {DEF_PRBS_11, 2047},
    {DEF_PRBS_15, 32767},
};

static ST_EEPROM m_stEeprom[] = {
    {SCC_EEPROM_IDX, SCC_EEPROM_ADDR},
    {RPC_EEPROM_IDX, RPC_EEPROM_ADDR},
    {FAN1_EEPROM_IDX, FAN1_EEPROM_ADDR},
    {FAN3_EEPROM_IDX, FAN3_EEPROM_ADDR},
};






struct timeval startTime, stopTime;

/* 获取当前时间 */
static void
__StartTime(void)
{
    gettimeofday(&startTime, NULL);
}


static double
__StopTime(void)
{
    double time_intervel = 0.0;

    gettimeofday(&stopTime, NULL);

    time_intervel = ((stopTime.tv_sec - startTime.tv_sec) * 1000000 + (stopTime.tv_usec - startTime.tv_usec));
    time_intervel /= 1000;

    return time_intervel;

}





uint8_t
__delayMs(int32_t msec)
{
    uint8_t ucState = STATE_OK;
    ucState = usleep(msec * 1000);
    if (ucState == -1) {
        return STATE_ERROR;
    }
    return ucState;
}

/**
 * @}
 */

/*return -1 or Seq length                                                                         */
static int32_t
__UTIL_Prbs(uint8_t ucStandard, uint32_t ulInitValue, int8_t *pcSeq)
{
    int32_t i = 0, j = 0;
    uint8_t ucMsb = 0, aucPolynomial[20] = {0};
    uint8_t ucSeq[32768];
    uint8_t ucIndex = 0;                                                    /* m_stPrbs index                                          */
    uint32_t ulTempValue = 0;
    int8_t *pcPtr = NULL;

    if (NULL == pcSeq) {
        printf("NULL == pcSeq");
        return -1;
    }

    for (i = 0; i < ((sizeof(m_stPrbs)) / (sizeof(ST_PRBS_SEQ))); i++) {
        if (ucStandard == m_stPrbs[i].ucStandard) {
            ucIndex = i;
            break;
        }
    }
    if (i >= ((sizeof(m_stPrbs)) / (sizeof(ST_PRBS_SEQ)))) {
        printf("Only supported PRBS7 & PRBS11 & PRBS15\r\n");
        return -1;
    }

    /* Iinti Polynomial                                                         */
    ulTempValue = ulInitValue;
    for (i = 0; i < ucStandard; i++) {
        aucPolynomial[ucStandard - 1 - i] = (uint8_t)ulTempValue & 0x01;
        ulTempValue = ulTempValue >> 1;
    }

    for (i = 0; i < (m_stPrbs[ucIndex].ulSeqLen + 1); i++) {
        ucSeq[i] = aucPolynomial[ucStandard - 1];
        if ((DEF_PRBS_7 == ucStandard) || (DEF_PRBS_15 == ucStandard)) {
            /*xn+x(n-1)+1                                                       */
            ucMsb = aucPolynomial[ucStandard - 1] ^ aucPolynomial[ucStandard - 2];
        } else if (DEF_PRBS_11 == ucStandard) {
            /*xn+x(n-2)+1                                                       */
            ucMsb = aucPolynomial[ucStandard - 1] ^ aucPolynomial[ucStandard - 3];
        }
        /* shift                                                                */
        for (j = ucStandard - 1; j > 0; j--) {
            aucPolynomial[j] = aucPolynomial[j - 1];
        }
        aucPolynomial[0] = ucMsb;
    }

    pcPtr = pcSeq;
    ulTempValue = 0;
    j = 0;
    for (i = 1; i < (m_stPrbs[ucIndex].ulSeqLen + 1); i++) {
        /* printf("%d", ucSeq[i]);                                              */
        if (0 == (i % 8)) {
            /* printf("       %d\r\n", ulTempValue);                            */
            *pcPtr = (int8_t)ulTempValue;
            ulTempValue = 0;
            pcPtr++;
            j++;
        }
        ulTempValue = (ulTempValue | ucSeq[i]) << 1;
    }

    *pcPtr = (int8_t)ulTempValue;
    j++;

    return j * 8;
}


uint8_t
WriteKeyWordValue(FILE *fpFile, const char *pacInKeyWord, char *pacInValue)
{
    char acLine[256] = {0};
    uint8_t ucState = STATE_OK;

    if (NULL == fpFile || NULL == pacInKeyWord || NULL == pacInValue) {
        return STATE_ERROR;
    }

    sprintf(acLine, "%s %s\n", pacInKeyWord, pacInValue);

    /* printf("%s\r\n",acLine); */

    ucState = fputs(acLine, fpFile);
    if (ucState == EOF) {
        printf("write error\r\n");
        ucState = STATE_ERROR;

    }
    memset(acLine, 0, sizeof(acLine));
    return ucState;

}

uint8_t
WriteMfg(char *pcFilePath, CMfg *stMfg)
{
    FILE *fpFile = NULL;
    if (NULL == pcFilePath) {
        printf("pcFilePath is null\r\n");
        return STATE_ERROR;
    }

    fpFile = fopen(pcFilePath, "w+");
    if (NULL == fpFile) {
        printf("Fail to fopen. FilePath = %s\r\n", pcFilePath);
        return STATE_ERROR;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_MFG_NAME, stMfg->acManufacturer)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_PRODUCT_NAME, stMfg->acProductName)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_TYPE, stMfg->acModuleType)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_SW_VER, stMfg->acSwVer)) {

    }

    if (!WriteKeyWordValue(fpFile, MODULE_FW_VER, stMfg->acFwVer)) {

    }

    if (!WriteKeyWordValue(fpFile, MODULE_HW_REV, stMfg->acHwVer)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_PCB_REV, stMfg->acPcbRev)) {

    }

    if (!WriteKeyWordValue(fpFile, MODULE_SN, stMfg->acSN)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_PART_NO, stMfg->acPN)) {
        goto ERR_LABEL;
    }
    /* strcat(stMfg.acMfgDate, "T00:00:00Z"); */
    if (!WriteKeyWordValue(fpFile, MODULE_MFG_DATE, stMfg->acMfgDate)) {
        goto ERR_LABEL;
    }

    /* strcat(stMfg.acCaliDate, "T00:00:00Z"); */

    if (!WriteKeyWordValue(fpFile, MODULE_CAL_DATE, stMfg->acCaliDate)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_CLEI_CODE, stMfg->acCleiCode)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_SUB_BOARD_REV, stMfg->acSubBdlVer)) {
        goto ERR_LABEL;
    }
    #if 0
    if (!WriteKeyWordValue(fpFile, MAC_ADDRESS_ETH0, stMfg->acEth0Mac)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MAC_ADDRESS_ETH3, stMfg->acEth3Mac)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MAC_ADDRESS_ETH4, stMfg->acEth4Mac)) {
        goto ERR_LABEL;
    }
    #endif

    /* fnc additional */
    if (!WriteKeyWordValue(fpFile, MODULE_UNIT_NAME, stMfg->acUnitName)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_FC_PN, stMfg->acFCPN)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_FC_ISS, stMfg->acFCIss)) {
        goto ERR_LABEL;
    }

    if (!WriteKeyWordValue(fpFile, MODULE_FUJITSU_PN, stMfg->acFujitsuPN)) {
        goto ERR_LABEL;
    }

    if (ferror(fpFile)) {
        printf("Fail to ferror %s\r\n", pcFilePath);
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(fpFile);

    return STATE_OK;
ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return STATE_ERROR;
}

uint8_t
FindKeyWordValue(FILE *fpFile, const char *pacInKeyWord, char *pacOutValue)
{
    char acLine[256] = {0};
    char acParaName[256] = {0};
    char acParaVlaue[256] = {0};

    if (NULL == fpFile || NULL == pacInKeyWord || NULL == pacOutValue) {
        printf("pull error!\r\n");
        return STATE_ERROR;
    }

    /****************************************************************************/
    /* read one line from file                                                  */
    /****************************************************************************/
    fseek(fpFile, 0, SEEK_SET);
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        if ('#' == acLine[0]) {
            continue;
        }

        sscanf(acLine, "%s %s", acParaName, acParaVlaue);
        if (0 == strncasecmp(acParaName, pacInKeyWord, strlen(pacInKeyWord))) {
            if ( strlen(acParaVlaue) > (MFG_INFO_LEN_MAX - 1)) {
                printf("value is too long, not bigger than %d", (MFG_INFO_LEN_MAX - 1));
                return STATE_ERROR;
            }

            strcpy(pacOutValue, acParaVlaue);
            /* printf("copy para!\r\n"); */
            /* OPLK_LOG_PRINT_DBG("find %s is %s", pacInKeyWord, pacOutValue); */
            memset(acLine, 0, sizeof(acLine));

            return STATE_OK;
        }

    }

    /*  printf("can not find keyword:%s", pacInKeyWord); */
    return STATE_ERROR;

}


uint8_t
CheckMacAddr(const char *pcMac)
{
    char acTmpBuf[64];
    char *pcStrtmp = NULL;
    uint8_t ucHexNum = 0;

    if ((NULL == pcMac) || (MAC_ADDR_LEN != strlen(pcMac))) {
        return STATE_ERROR;
    }

    memset(acTmpBuf, 0, sizeof(acTmpBuf));
    strncpy(acTmpBuf, pcMac, sizeof(acTmpBuf));
    pcStrtmp = strtok(acTmpBuf, ":");

    while (NULL != pcStrtmp) {
        if (2 != strlen(pcStrtmp)) {
            return STATE_ERROR;
        }

        if ((pcStrtmp[0] >= '0' && pcStrtmp[0] <= '9')
            || (pcStrtmp[0] >= 'A' && pcStrtmp[0] <= 'F')
            || (pcStrtmp[0] >= 'a' && pcStrtmp[0] <= 'f')) {

            if ((pcStrtmp[1] >= '0' && pcStrtmp[1] <= '9')
                || (pcStrtmp[1] >= 'A' && pcStrtmp[1] <= 'F')
                || (pcStrtmp[1] >= 'a' && pcStrtmp[1] <= 'f')) {
                ucHexNum++;
                pcStrtmp = strtok(NULL, ":");
            } else {
                return STATE_ERROR;
            }
        } else {
            return STATE_ERROR;
        }
    }

    if (6 != ucHexNum) {
        return STATE_ERROR;
    }
    return STATE_OK;
}
uint8_t
ReadMfg(char *c_pcFilePath, CMfg *stMfg)
{
    uint8_t ucState = STATE_OK;
    FILE *fpFile = NULL;

    if (NULL == c_pcFilePath) {
        printf("input ptr is null");
        return STATE_ERROR;
    }

    fpFile = fopen(c_pcFilePath, "r");
    if (NULL == fpFile) {
        printf("Fail to fopen-> FilePath = %s", c_pcFilePath);
        return STATE_ERROR;
    }

    if (FindKeyWordValue(fpFile, MODULE_MFG_NAME, stMfg->acManufacturer) == STATE_ERROR) {
        printf("error %s\r\n", stMfg->acManufacturer);
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }
    /* printf("%s \r\n", stMfg->acManufacturer); */

    if (FindKeyWordValue(fpFile, MODULE_PRODUCT_NAME, stMfg->acProductName) == STATE_ERROR) {
        printf("error %s\r\n", stMfg->acManufacturer);
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }
    /* printf("%s \r\n", stMfg->acProductName); */
    if (FindKeyWordValue(fpFile, MODULE_TYPE, stMfg->acModuleType) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    if (FindKeyWordValue(fpFile, MODULE_SW_VER, stMfg->acSwVer) == STATE_ERROR) {

    }

    if (FindKeyWordValue(fpFile, MODULE_FW_VER, stMfg->acFwVer) == STATE_ERROR) {

    }

    if (FindKeyWordValue(fpFile, MODULE_HW_REV, stMfg->acHwVer) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    if (FindKeyWordValue(fpFile, MODULE_PCB_REV, stMfg->acPcbRev) == STATE_ERROR) {

    }

    if (FindKeyWordValue(fpFile, MODULE_SN, stMfg->acSN) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    if (FindKeyWordValue(fpFile, MODULE_PART_NO, stMfg->acPN) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    if (FindKeyWordValue(fpFile, MODULE_MFG_DATE, stMfg->acMfgDate) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    /* strcat(stMfg->acMfgDate, "T00:00:00Z"); */

    if (FindKeyWordValue(fpFile, MODULE_CAL_DATE, stMfg->acCaliDate) == STATE_ERROR) {

    }

    /* strcat(stMfg->acCaliDate, "T00:00:00Z"); */

    if (!FindKeyWordValue(fpFile, MODULE_CLEI_CODE, stMfg->acCleiCode)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_SUB_BOARD_REV, stMfg->acSubBdlVer)) {

    }



    if (FindKeyWordValue(fpFile, MAC_ADDRESS_ETH0, stMfg->acEth0Mac) == STATE_ERROR) {

    } else if (CheckMacAddr(stMfg->acEth0Mac) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    if (FindKeyWordValue(fpFile, MAC_ADDRESS_ETH3, stMfg->acEth3Mac) == STATE_ERROR) {

    } else if (CheckMacAddr(stMfg->acEth3Mac) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    if (FindKeyWordValue(fpFile, MAC_ADDRESS_ETH4, stMfg->acEth4Mac) == STATE_ERROR) {

    } else if (CheckMacAddr(stMfg->acEth4Mac) == STATE_ERROR) {
        ucState = STATE_ERROR;
        goto ERR_LABEL;
    }

    /* fnc additional */
    if (!FindKeyWordValue(fpFile, MODULE_UNIT_NAME, stMfg->acUnitName)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_FC_PN, stMfg->acFCPN)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_FC_ISS, stMfg->acFCIss)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_FUJITSU_PN, stMfg->acFujitsuPN)) {

    }

    if (ferror(fpFile)) {
        printf("Fail to ferror %s", c_pcFilePath);
        goto ERR_LABEL;
    }

ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return ucState;
}


uint8_t
UTIL_SetMfg(char *c_pcReadFilePath, char *c_pcWriteFilePath, CMfg *stMfg, uint8_t num)
{
    uint8_t ucState = STATE_OK;
    memset(stMfg, 0x00, sizeof(stMfg));

    ucState = ReadMfg(c_pcReadFilePath, stMfg);
    if (ucState == STATE_ERROR) {
        printf("ReadMfg error\r\n");
        /* goto END; */
    }
    #if 0
    printf("%s \r\n", stMfg->acManufacturer);
    printf("%s \r\n", stMfg->acProductName);
    printf("%s \r\n", stMfg->acModuleType);
    printf("%s \r\n", stMfg->acSwVer);
    printf("%s \r\n", stMfg->acFwVer);
    printf("%s \r\n", stMfg->acHwVer);
    printf("%s \r\n", stMfg->acPcbRev);
    printf("%s \r\n", stMfg->acSN);
    printf("%s \r\n", stMfg->acPN);
    printf("%s \r\n", stMfg->acMfgDate);
    printf("%s \r\n", stMfg->acCaliDate);
    #endif

    sprintf(stMfg->acManufacturer, "%s%d", stMfg->acManufacturer, num % 10);
    sprintf(stMfg->acProductName, "%s%d", stMfg->acProductName, num % 10);
    /* sprintf(stMfg->acModuleType, "%s%d", stMfg->acModuleType, num  % 10); */
    sprintf(stMfg->acBootVer, "%s%d", stMfg->acBootVer, num % 10);
    sprintf(stMfg->acSwVer, "%s%d", stMfg->acSwVer, num % 10);
    sprintf(stMfg->acFwVer, "%s%d", stMfg->acFwVer, num % 10);
    sprintf(stMfg->acHwVer, "%s%d", stMfg->acHwVer, num % 10);
    sprintf(stMfg->acPcbRev, "%s%d", stMfg->acPcbRev, num % 10);
    sprintf(stMfg->acSN, "%s%d", stMfg->acSN, num % 10);
    sprintf(stMfg->acPN, "%s%d", stMfg->acPN, num % 10);

    printf("%s \r\n", stMfg->acManufacturer);
    printf("%s \r\n", stMfg->acProductName);
    printf("%s \r\n", stMfg->acModuleType);
    printf("%s \r\n", stMfg->acSwVer);
    printf("%s \r\n", stMfg->acFwVer);
    printf("%s \r\n", stMfg->acHwVer);
    printf("%s \r\n", stMfg->acPcbRev);
    printf("%s \r\n", stMfg->acSN);
    printf("%s \r\n", stMfg->acPN);
    printf("%s \r\n", stMfg->acMfgDate);
    printf("%s \r\n", stMfg->acCaliDate);

    ucState = WriteMfg(c_pcWriteFilePath, stMfg);
    if (ucState == STATE_ERROR) {
        printf("WriteMfg error\r\n");
        goto END;
    }
END:
    return ucState;
}

uint8_t
UITL_CompareMfg(char *MfgType, CMfg stWriteMfg, CMfg stReadMfg)
{
    uint8_t ucState = STATE_OK;
#if 0
    printf("%s \r\n", stWriteMfg.acManufacturer);
    printf("%s \r\n", stWriteMfg.acProductName);
    printf("%s \r\n", stWriteMfg.acModuleType);
    printf("%s \r\n", stWriteMfg.acSwVer);
    printf("%s \r\n", stWriteMfg.acFwVer);
    printf("%s \r\n", stWriteMfg.acHwVer);
    printf("%s \r\n", stWriteMfg.acPcbRev);
    printf("%s \r\n", stWriteMfg.acSN);
    printf("%s \r\n", stWriteMfg.acPN);
    printf("%s \r\n", stWriteMfg.acMfgDate);
    printf("%s \r\n", stWriteMfg.acCaliDate);

#endif

#if 1
    printf("%s \r\n", stReadMfg.acManufacturer);
    printf("%s \r\n", stReadMfg.acProductName);
    printf("%s \r\n", stReadMfg.acModuleType);
    printf("%s \r\n", stReadMfg.acSwVer);
    printf("%s \r\n", stReadMfg.acFwVer);
    printf("%s \r\n", stReadMfg.acHwVer);
    printf("%s \r\n", stReadMfg.acPcbRev);
    printf("%s \r\n", stReadMfg.acSN);
    printf("%s \r\n", stReadMfg.acPN);
    printf("%s \r\n", stReadMfg.acMfgDate);
    printf("%s \r\n", stReadMfg.acCaliDate);

#endif
#if 1
    ucState = strcmp(stWriteMfg.acManufacturer, stReadMfg.acManufacturer);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acManufacturer, stReadMfg.acManufacturer);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acProductName, stReadMfg.acProductName);
    if ( ucState != 0) {

        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acProductName, stReadMfg.acProductName);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acModuleType, stReadMfg.acModuleType);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acModuleType, stReadMfg.acModuleType);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acSwVer, stReadMfg.acSwVer);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acSwVer, stReadMfg.acSwVer);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acFwVer, stReadMfg.acFwVer);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acFwVer, stReadMfg.acFwVer);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acHwVer, stReadMfg.acHwVer);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acHwVer, stReadMfg.acHwVer);
        ucState = STATE_ERROR;
    }

    ucState = strcmp(stWriteMfg.acPcbRev, stReadMfg.acPcbRev);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acPcbRev, stReadMfg.acPcbRev);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acSN, stReadMfg.acSN);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acSN, stReadMfg.acSN);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acPN, stReadMfg.acPN);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acPN, stReadMfg.acPN);
        ucState = STATE_ERROR;
    }
    #if 0
    ucState = strcmp(stWriteMfg.acEth0Mac, stReadMfg.acEth0Mac);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acEth0Mac, stReadMfg.acEth0Mac);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acEth3Mac, stReadMfg.acEth3Mac);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acEth3Mac, stReadMfg.acEth3Mac);
        ucState = STATE_ERROR;
    }
    ucState = strcmp(stWriteMfg.acEth4Mac, stReadMfg.acEth4Mac);
    if ( ucState != 0) {
        printf("%s stWriteMfg:%s!= stReadMfg:%s\r\n", MfgType, stWriteMfg.acEth4Mac, stReadMfg.acEth4Mac);
        ucState = STATE_ERROR;
    }
    #endif

#endif

    return ucState;
}


uint8_t
UTIL_MsataInterface(char *pcPathName)
{
    int8_t acInitSeq[BUFFERSIZE] = {0};
    int8_t acReadSeq[BUFFERSIZE] = {0};
    FILE *file = NULL;
    int32_t iLen = 0;
    int32_t i = 0;
    int32_t iFlag = 0;
    size_t number = 0;
    uint8_t ucState = STATE_OK;
    /* 参数检查 */
    if (pcPathName == NULL) {
        printf("Msata Pathname is NULL\r\n");
        ucState = STATE_ERROR;
        goto END;
    }

    srand((unsigned)time(NULL));                                        /* init rand                                               */
    iLen = __UTIL_Prbs(DEF_PRBS_MODE, rand(), acInitSeq);

    /* printf("__UTIL_Prbs Length = %d\r\n",iLen); */
    if (-1 == iLen) {

        printf("Msata__UTIL_Prbs error\r\n");
        ucState = STATE_ERROR;
        goto END;

    }
    /* 打开文件流 */
    file = fopen(pcPathName, "w+");
    if (NULL == file) {
        printf("Msata open file error\r\n");
        ucState = STATE_ERROR;
        goto END;

    }

    /* 设置流定向及缓冲类型 */
    /* fwide(file,1); */
    /* setvbuf(file,NULL,_IOFBF,0); */

    memset(&number, 0, sizeof(size_t));
    number = fwrite(acInitSeq, iLen, 1, file);

    if (number != 1) {
        printf("Msata write fail\r\n");
        ucState = STATE_ERROR;
        goto END;
    }

    sleep(2);

    iFlag = fclose(file);
    if (iFlag == EOF) {
        printf("Msata close fail\r\n");
        ucState = STATE_ERROR;
        goto END;
    }
    /* 打开文件流 */
    file = fopen(pcPathName, "r+");
    if (NULL == file) {
        printf("Msata open file error\r\n");
        ucState = STATE_ERROR;
        goto END;

    }

    memset(&number, 0, sizeof(size_t));
    number = fread(acReadSeq, iLen, 1, file);

    /* printf("number = %ld\r\n",number); */
    /* printf("size = %d\r\n",size); */
    if (number != 1) {
        printf("Msata read fail\r\n");
        goto END;
    }

    for (i = 0; i < iLen; i++) {
        if (acInitSeq[i] != acReadSeq[i]) {
            printf("pcSeq[i] %d != acReadSeq[i] %d i %d iLen %d\r\n", acInitSeq[i], acReadSeq[i], i, iLen);
            break;
        }
    }

END:
    if (file != NULL) {
        iFlag = fclose(file);
        if (iFlag == EOF) {
            printf(" Msata close fail\r\n");
            ucState = STATE_ERROR;
        }
    }

    return ucState;
}

uint8_t
UTIL_MsataInterfaceTest(void)
{
    uint8_t ucState = STATE_OK;

    ucState = UTIL_MsataInterface(MSATA_PATH_ONE);

    if (ucState == STATE_ERROR) {
        printf("Msata path one error\r\n");
        goto END;
    }
    ucState = UTIL_MsataInterface(MSATA_PATH_TWO);

    if (ucState == STATE_ERROR) {
        printf("Msata path two error\r\n");
        goto END;
    }

    ucState = UTIL_MsataInterface(MSATA_PATH_THREE);

    if (ucState == STATE_ERROR) {
        printf("Msata path three error\r\n");
        goto END;
    }

    printf("Msata finished one Loop\r\n");

END:
    return ucState;
}

uint32_t
UTIL_PCIEInterfaceTest(void)
{
    uint32_t uiState = STATE_OK;
    uint32_t uidx = 0;
    uint64_t ulAddr = 0x10;
    uint64_t ulLen = 4;
    uint8_t pucWriteVal[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    uint8_t pucReadVal[4] = {0};

    __StartTime();

    uiState = DBG_WriteLogicReg(uidx, ulAddr, ulLen, pucWriteVal);


    if (uiState != STATE_OK) {
        printf("PCIE Write Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }
    printf("Write Logic Reg time interval %lf ms\r\n", __StopTime());

    memset(pucReadVal, 0x00, sizeof(pucReadVal));
    __StartTime();
    uiState = DBG_ReadLogicReg(uidx, ulAddr, ulLen, pucReadVal);


    if (uiState != STATE_OK) {
        printf("PCIE Read Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }
    printf("Read Logic Reg time interval %lf ms\r\n", __StopTime());


    for (int i = 0; i < ulLen; i++) {
        if (pucWriteVal[i] != pucReadVal[i]) {
            printf("pucWriteVal[%d]= %x != pucReadVal[%d] =%x\r\n", i, pucWriteVal[i], i, pucReadVal[i]);
            break;
        }
    }

    #if 0
    for (int i = 0; i < 2; i++) {
        ulAddr = 0x3110 + 0x100 * i;
        __StartTime();

        uiState = DBG_WriteLogicRegSlc(i + 1, ulAddr, ulLen, pucWriteVal);

        if (uiState != STATE_OK) {
            printf("PCIE SLC %d Write Error!\r\n", i + 1);
            uiState = STATE_ERROR;
            goto END;
        }
        printf("Write Logic slc %d Reg time interval %lf ms\r\n", i + 1, __StopTime());

        memset(pucReadVal, 0x00, sizeof(pucReadVal));
        __StartTime();
        uiState = DBG_ReadLogicRegslc(i + 1, ulAddr, ulLen, pucReadVal);


        if (uiState != STATE_OK) {
            printf("PCIE  SLC %d Read Error!\r\n", i + 1);
            uiState = STATE_ERROR;
            goto END;
        }
        printf("Read Logic slc %d Reg time interval %lf ms\r\n", i + 1, __StopTime());


        for (int i = 0; i < ulLen; i++) {
            if (pucWriteVal[i] != pucReadVal[i]) {
                printf("pucWriteVal[%d]= %x != pucReadVal[%d] =%x\r\n", i, pucWriteVal[i], i, pucReadVal[i]);
                break;
            }
        }


    }
    #endif

END:
    return uiState;

}

uint8_t
UTIL_SystemInterface(void)
{
    uint8_t ucState = STATE_OK;

    ucState = UTIL_MsataInterfaceTest();

    if (ucState == STATE_ERROR) {
        printf("Msata Interface error\r\n");
    }

    ucState = UTIL_PCIEInterfaceTest();

    if (ucState == STATE_ERROR) {
        printf("PCIEInterface error");
    }

    return ucState;

}

uint32_t
UTIL_SCCEEPROMInterfaceTest(void)
{
    uint32_t uiState = STATE_OK;
    CMfg stWriteMfg;
    CMfg stReadMfg;
    static uint8_t num = 0;
    memset(&stWriteMfg, 0x00, sizeof(stWriteMfg));
    #if 1
    uiState = UTIL_SetMfg(READ_SCC_MFG_PATH, SCC_MFG_PATH, &stWriteMfg, num++);
    if (uiState != STATE_OK) {
        printf("SCC EEPROM Set Mfg Error\r\n");
    }
    #endif

    #if 1
    __StartTime();
    uiState = BOARD_SetSccMfg(SCC_MFG_PATH);

    if (uiState != STATE_OK) {
        printf("SCC EERPOM Write Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }
    printf("Write SCC EERPOM  time interval %lf ms\r\n", __StopTime());
    #endif
    /* Set delay */
    __delayMs(DELAY_EEPROM_MS);

    memset(&stReadMfg, 0x00, sizeof(stReadMfg));
    __StartTime();
    uiState = BOARD_GetSccMfg(&stReadMfg);

    if (uiState != STATE_OK) {
        printf("SCC EEPROM Read Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }
    printf("Read SCC EEPROM time interval %lf ms\r\n", __StopTime());

    uiState = UITL_CompareMfg("SCC EEPROM", stWriteMfg, stReadMfg);
    if (uiState != STATE_OK) {

        printf("SCC EEPROM CompareMfg Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }

END:
    return uiState;
}

uint32_t
UTIL_RPCEEPROMInterfaceTest(void)
{
    uint32_t uiState = STATE_OK;
    CMfg stWriteMfg;
    CMfg stReadMfg;
    static uint8_t num = 0;
    memset(&stWriteMfg, 0x00, sizeof(stWriteMfg));
    #if 1
    uiState = UTIL_SetMfg(READ_RPC_MFG_PATH, RPC_MFG_PATH, &stWriteMfg, num++);
    if (uiState != STATE_OK) {
        printf("RPC EEPROM Set Mfg Error\r\n");
    }
    #endif

    __StartTime();
    uiState = BOARD_SetChassisMfg(RPC_MFG_PATH);

    if (uiState != STATE_OK) {
        printf("RPC EERPOM Write Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }
    printf("Write RPC EERPOM  time interval %lf ms\r\n", __StopTime());

    /* set Delay */
    __delayMs(DELAY_EEPROM_MS);

    memset(&stReadMfg, 0x00, sizeof(stReadMfg));
    __StartTime();
    uiState = BOARD_GetChassisMfg(&stReadMfg);

    if (uiState != STATE_OK) {
        printf("RPC EEPROM Read Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }
    printf("Read RPC EEPROM time interval %lf ms\r\n", __StopTime());

    uiState = UITL_CompareMfg("RPC EEPROM", stWriteMfg, stReadMfg);
    if (uiState != STATE_OK) {

        printf("RPC EEPROM CompareMfg Error!\r\n");
        uiState = STATE_ERROR;
        goto END;
    }

END:
    return uiState;

}


uint32_t
UTIL_SLCEEPROMInterfaceTest(void)
{
    uint32_t uiState = STATE_OK;
    CMfg stWriteMfg;
    CMfg stReadMfg;
    static uint8_t num = 0;


    for (int i = 0; i < 2; i++) {
        memset(&stWriteMfg, 0x00, sizeof(stWriteMfg));
        uiState = UTIL_SetMfg(READ_SLC_MFG_PATH, SLC_MFG_PATH, &stWriteMfg, num);
        if (uiState != STATE_OK) {
            printf("SLC EEPROM Set Mfg Error\r\n");
        }
        __StartTime();
        uiState = SLOT_SetMfg(i, SLC_MFG_PATH);

        if (uiState != STATE_OK) {
            printf("SLC %d EERPOM Write Error!\r\n", i);
            uiState = STATE_ERROR;
            goto END;
        }
        printf("Write SLC %d EERPOM  time interval %lf ms\r\n", i, __StopTime());

        /* set delay */
        __delayMs(DELAY_EEPROM_MS);


        memset(&stReadMfg, 0x00, sizeof(stReadMfg));
        __StartTime();
        uiState = SLOT_GetMfg(i, &stReadMfg);

        if (uiState != STATE_OK) {
            printf("SLC %d EEPROM Read Error!\r\n", i);
            uiState = STATE_ERROR;
            goto END;
        }
        printf("Read SLC %d EEPROM time interval %lf ms\r\n", i, __StopTime());

        uiState = UITL_CompareMfg("SLC EEPROM", stWriteMfg, stReadMfg);
        if (uiState != STATE_OK) {

            printf("SLC %d EEPROM CompareMfg Error!\r\n", i);
            uiState = STATE_ERROR;
            /* goto END; */
        }

    }
    num++;

END:
    return uiState;
}

uint32_t
UTIL_PSUInterfaceTest(void)
{
    uint32_t uiState = STATE_OK;

    CMfg stReadMfg;
    for (int i = 0; i < BOARD_PSU_NUM; i++) {
        __StartTime();
        uiState = POWER_GetMfg(0, &stReadMfg);

        if (uiState != STATE_OK) {
            printf("PSU GetMfg Error!\r\n");
            uiState = STATE_ERROR;
            goto END;
        }
        printf("PSU %d GetMfg time interval %lf ms\r\n", i, __StopTime());

        printf("%s \r\n", stReadMfg.acManufacturer);
        printf("%s \r\n", stReadMfg.acProductName);
        printf("%s \r\n", stReadMfg.acModuleType);
        printf("%s \r\n", stReadMfg.acSwVer);
        printf("%s \r\n", stReadMfg.acFwVer);
        printf("%s \r\n", stReadMfg.acHwVer);
        printf("%s \r\n", stReadMfg.acPcbRev);
        printf("%s \r\n", stReadMfg.acSN);
        printf("%s \r\n", stReadMfg.acPN);
        printf("%s \r\n", stReadMfg.acMfgDate);
        printf("%s \r\n", stReadMfg.acCaliDate);

    }

END:
    return uiState;
}

uint32_t
UTIL_FANEEPROMInterfaceTest(void)
{
    uint32_t uiState = STATE_OK;
    CMfg stWriteMfg;
    CMfg stReadMfg;
    static uint8_t num = 0;

    for (int i = 1; i < BOARD_FAN_NUM; i = i + 2) {
        #if 1
        memset(&stWriteMfg, 0x00, sizeof(stWriteMfg));
        uiState = UTIL_SetMfg(READ_FAN_MFG_PATH, FAN_MFG_PATH, &stWriteMfg, num);
        if (uiState != STATE_OK) {
            printf("FAN %d EEPROM Set Mfg Error\r\n", i);
        }
        #endif
        __StartTime();
        uiState = FAN_SetMfg(i, FAN_MFG_PATH);
        if (uiState != STATE_OK) {
            printf("FAN %d EERPOM Write Error!\r\n", i);
            uiState = STATE_ERROR;

        }
        printf("Write FAN %d EERPOM  time interval %lf ms\r\n", i, __StopTime());

        /* Set delay */
        __delayMs(DELAY_EEPROM_MS);

        memset(&stReadMfg, 0x00, sizeof(stReadMfg));
        __StartTime();
        uiState = FAN_GetMfg(i, &stReadMfg);

        if (uiState != STATE_OK) {
            printf("FAN %d EEPROM Read Error!\r\n", i);
            uiState = STATE_ERROR;

        }
        printf("Read FAN %d EEPROM time interval %lf ms\r\n", i, __StopTime());

        uiState = UITL_CompareMfg("FAN EEPROM", stWriteMfg, stReadMfg);
        if (uiState != STATE_OK) {

            printf("FAN %d EEPROM CompareMfg Error!\r\n", i);
            uiState = STATE_ERROR;
            /* goto END; */
        }
    }
    num++;

END:
    return uiState;
}

uint8_t
UTIL_BoardI2CInterfaceTest(void)
{
    uint8_t ucState = STATE_OK;
    CBoardStatusData stBoardStausData = {0};
    static int i = 1;
    uint32_t uiDuty = 10;
    uint32_t uiFanLedValue = LED_STATUS_GREEN_BLINK;
    uint32_t uiBoardLedValue = 1;
    uint8_t ucVal = 0x0f;

    /* memset(stBoardStatusData, 0x00, sizeof(CBoardStatusData)); */
    ucState = DBG_WriteLogicReg(0, 0x100C, 1, &ucVal);
    if (ucState != STATE_OK) {
        printf("Set CPLD FAN Enable error\r\n");
    }
    uiDuty = 10 * (i % 10);
    for (int i = 1; i < BOARD_FAN_NUM; i = i + 2) {
        __StartTime();
        ucState = FAN_SetSpeed(i, uiDuty);
        if ( ucState == STATE_ERROR) {
            printf("Set Fan %d Speed Error\r\n", i);
        }
        #if 0
        ucState = FAN_SetLed(i, uiFanLedValue);
        if ( ucState == STATE_ERROR) {
            printf("Set Fan %d Led Error\r\n", i);
        }
        #endif
        printf("Set Fan %d Speed and Led time interval %lf ms\r\n", i, __StopTime());

    }



#if 1
    /* Led 0-2  SCC LED */
    /* Led 3-7  RPC LED */
    uiBoardLedValue = i % 7;
    if (uiBoardLedValue == 4) {
        uiBoardLedValue == LED_STATUS_GREEN_BLINK;
    }
    for (int i = 0; i < BOARD_LED_NUM; i++) {
        __StartTime();
        ucState = BOARD_SetLedState(i, uiBoardLedValue);
        if (ucState == STATE_ERROR) {
            printf("Set Board Led %d Error\r\n", i);
        }
        printf("Set Board Led %d time interval %lf ms\r\n", i, __StopTime());
    }
#endif

    /* set Delay */
    __delayMs(DELAY_GEISTATUS_MS);

    __StartTime();
    ucState = BOARD_GetStatusData(&stBoardStausData);

    if (ucState == STATE_ERROR ) {
        printf("Get Board Interface error");
    }

    printf("Get Board Status time interval %lf ms\r\n", __StopTime());


    printf("FAN State Information:\r\n");


    for (int i = 1; i < BOARD_FAN_NUM; i = i + 2) {

        printf("FAN%d:Online is %d\r\n", i, stBoardStausData.astFan[i].bIsOnline);
        printf("FAN%d:Failure is %d\r\n", i, stBoardStausData.astFan[i].bIsFailure);
        #if 1
        if (stBoardStausData.astFan[i].bIsOnline) {
            printf("Fan %d set Speed %d get Speed %d \r\n", i, uiDuty, stBoardStausData.astFan[i].uiSpeed);
        }
        #endif
    }
    #if 1
    printf("Led State information:\r\n");

    for (int i = 0; i < BOARD_LED_NUM; i++) {

        printf("LED%d:Set State %d Get state %d\r\n", i, uiBoardLedValue, stBoardStausData.astLed[i].uiState);
    }
    #endif

    printf("PSU State information:\r\n");

    for (int i = 0; i < 2; i++) {

        if (stBoardStausData.astPower[i].bIsOnline) {
            printf("PSU%d:Online is %d\r\n", i, stBoardStausData.astPower[i].bIsOnline);
            printf("PSU%d:Failure is %d\r\n", i, stBoardStausData.astPower[i].bIsFailure);
            printf("PSU%d:InVol is %d\r\n", i, stBoardStausData.astPower[i].usInVol);
            printf("PSU%d:OutVol is %d\r\n", i, stBoardStausData.astPower[i].usOutVol);
            printf("PSU%d:InCurrent is %d\r\n", i, stBoardStausData.astPower[i].uiInCurrent);
            printf("PSU%d:InCurrent is %d\r\n", i, stBoardStausData.astPower[i].uiOutCurrent);
            printf("PSU%d:Watts is %d\r\n", i, stBoardStausData.astPower[i].uiWatts);
            printf("PSU%d:Temp is %d\r\n", i, stBoardStausData.astPower[i].iTemp);

        }

    }

    printf("Temp State information:\r\n");
    printf("I2C Temp1 :%d\r\n", stBoardStausData.aiTemp[0]);
    printf("I2C Temp2 :%d\r\n", stBoardStausData.aiTemp[1]);

    printf("Voltage Moniter State information:\r\n");
    for (int i = 0; i < 6; i++) {
        printf("SCC CPLD ADC%d:%f\r\n", i, stBoardStausData.astMax1139[0].adVoltage[i]);
    }
    /* for (int i = 0; i < 6; i++) */
    /* { */
    /*      printf("SLC0 VCC%d:%f\r\n",i,stBoardStausData.astMax1139[1].adVoltage[i]); */
    /* } */
    /* for (int i = 0; i < 6; i++) */
    /* { */
    /*      printf("SLC1 VCC%d:%f\r\n",i,stBoardStausData.astMax1139[2].adVoltage[i]); */
    /* } */

    i++;
    return ucState;

}


uint8_t
UTIL_EepromTest(void)
{
    uint8_t ucState = STATE_OK;
    uint64_t ulLen = 4;
    uint8_t WriteVal[4] = {0};
    uint8_t ReadVal[4] = {0};
    static int32_t num = 0;

    for (int i = 0; i < ulLen; i++) {
        WriteVal[i] = 10 * i * (num % 10);
    }

    num++;
    for (int i = 0; i < EEPROM_NUM; i++) {
        __StartTime();
        ucState = DBG_WriteEeprom(m_stEeprom[0].idx, m_stEeprom[0].uiAddr, ulLen, WriteVal);
        if (ucState != STATE_OK) {
            printf("EEPROM %d Write Error!\r\n", i);
        }
        printf("Write EERPOM %d time interval %lf ms\r\n", i, __StopTime());
        memset(ReadVal, 0x00, sizeof(ReadVal));
        __StartTime();
        ucState = DBG_ReadEeprom(m_stEeprom[0].idx, m_stEeprom[0].uiAddr, ulLen, ReadVal);
        if (ucState != STATE_OK) {
            printf("EEPROM %d Read Error!\r\n", i);
        }
        printf("Read EERPOM %d time interval %lf ms\r\n", i, __StopTime());

        for (int i = 0; i < ulLen; i++) {
            printf("%d\r\n", ReadVal[i]);
        }

        for (int i = 0; i < ulLen; i++) {
            if (WriteVal[i] != ReadVal[i]) {
                printf("EEPROM %d WriteVal:%d != ReadVal:%d\r\n", i, WriteVal[i], ReadVal[i]);
            }
        }

    }
    return ucState;
}



uint8_t
UTIL_Max1139Test(void)
{
    uint8_t ucState = STATE_OK;
    int aiMax1139Reg [6] =
    {REG_SCC_CPLD_V1P1_ADC,
     REG_SCC_CPLD_V1P5_ADC,
     REG_SCC_CPLD_V1P2_ADC,
     REG_SCC_CPLD_V1P8_ADC,
     REG_SCC_CPLD_V1P0_ADC,
     REG_SCC_CPLD_V2P5_ADC, };
    uint8_t acReadVal[2] = {0};
    uint16_t ulMax1139Adc = 0;
    /* float fMax1139Adc = 0; */
    for (int i = 0; i < 6; i++) {
        memset(acReadVal, 0x00, sizeof(acReadVal));
        __StartTime();
        ucState = DBG_ReadLogicReg(0, aiMax1139Reg[i], 2, acReadVal);
        if (ucState != STATE_OK) {
            printf("PCIE Read Error!\r\n");
            ucState = STATE_ERROR;
            /* goto END; */
        }
        printf("Max1139 Reg %d time interval %lf ms\r\n", i, __StopTime());
        ulMax1139Adc = acReadVal[1] << 8 | acReadVal[0];
        /*  printf("Max1139 Reg %d Value:%d %d \r\n",i,acReadVal[0],acReadVal[1]); */
        printf("Max1139 Reg %d Value:%d \r\n", i, ulMax1139Adc);
    #if 0
        fMax1139Adc = Max1139Adc;
        printf("Max1139 Reg %d Value:%lf \r\n", i, fMax1139Adc);
    #endif


    }

    return ucState;
}
uint8_t
UTIL_I2CInterface(void)
{
    uint8_t ucState = STATE_OK;

    #if 0
    ucState = UTIL_SCCEEPROMInterfaceTest();

    if (ucState == STATE_ERROR) {
        printf("SCC EEPROM Interface error\r\n");
    }

    ucState = UTIL_RPCEEPROMInterfaceTest();

    if (ucState == STATE_ERROR) {
        printf("RPC EEPROM Interface error\r\n");
    }
    ucState = UTIL_FANEEPROMInterfaceTest();

    if (ucState == STATE_ERROR) {
        printf("FAN EEPROM Interface error\r\n");
    }
    #endif

    ucState = UTIL_PSUInterfaceTest();
    if (ucState == STATE_ERROR) {
        printf("PSU EEPROM Interface error\r\n");
    }


    ucState = UTIL_BoardI2CInterfaceTest();
    if ( ucState == STATE_ERROR) {
        printf("Board I2C Interface error\r\n");
    }


    ucState = UTIL_EepromTest();
    if (ucState == STATE_ERROR) {
        printf("Eeprom error!\r\n");
    }

    ucState = UTIL_Max1139Test();
    if (ucState == STATE_ERROR) {
        printf("Max1139 error!\r\n");
    }



    return ucState;

}

uint8_t
UTIL_SPIFlashInterfaceTest(void)
{
    uint8_t ucState = STATE_OK;
    uint64_t ulchip = 7;
    uint64_t ulAddr = 0;
    uint64_t ulLen = 4;
    uint8_t ucWriteVal[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    uint8_t ucReadVal[4] = {0};
    __StartTime();
    ucState = DBG_SpiFlashWrite(ulchip, ulAddr, ulLen, ucWriteVal);
    if (ucState == STATE_ERROR) {
        printf("SpiFlash write error\r\n");
    }

    printf("Write SpiFlash time interval %lf ms\r\n", __StopTime());

    __StartTime();
    ucState = DBG_SpiFlashRead(ulchip, ulAddr, ulLen, ucReadVal);
    if (ucState == STATE_ERROR) {
        printf("SpiFlash read error\r\n");
    }
    printf("Read SpiFlash time interval %lf ms\r\n", __StopTime());
    for (int i; i < ulLen; i++) {
        printf("SpiFlash:%d\r\n", ucReadVal[i]);
    }

    __StartTime();
    ucState = DBG_SpiFlashRead(7, ulAddr, ulLen, ucReadVal);
    if (ucState == STATE_ERROR) {
        printf("SpiFlash read error\r\n");
    }
    printf("Read SpiFlash time interval %lf ms\r\n", __StopTime());
    for (int i; i < ulLen; i++) {
        printf("SpiFlash:%d\r\n", ucReadVal[i]);
    }

    return ucState;
}


uint8_t
UTIL_MDIOInterfaceTest(void)
{
    uint8_t ucState = STATE_OK;
    uint32_t ulIdx = 0;
    uint32_t ulStartAddr = 0;
    uint32_t ulReg = 16;
    uint16_t usWriteVal = 0x01;
    uint16_t usReadVal = 0;
    static uint32_t num = 0;
    usWriteVal = (num++) % 10;

    __StartTime();
    ucState = DBG_MdioWrite(ulIdx, ulStartAddr, ulReg, usWriteVal);
    if (ucState == STATE_ERROR) {
        printf("Mdio Write error\r\n");
    }
    printf("Mdio Write time interval %lf ms\r\n", __StopTime());
    __StartTime();
    ucState = DBG_MdioRead(ulIdx, ulStartAddr, ulReg, &usReadVal);
    if (ucState == STATE_ERROR) {
        printf("Mdio read error\r\n");
    }

    printf("Mdio read time interval %lf ms\r\n", __StopTime());
    #if 0
    printf("Mdio read value %d\r\n", usReadVal);
    #endif

    if (usReadVal != usWriteVal) {
        printf("MDIO usWriteVal:%d != Readval:%d\r\n", usWriteVal, usReadVal);
    }

    return ucState;
}


uint8_t
UTIL_SPIInterface(void)
{
    uint8_t ucState = STATE_OK;

    ucState = UTIL_SPIFlashInterfaceTest();

    if (ucState == STATE_ERROR) {

        printf("SPIFlash Interface error\r\n");
    }


    return ucState;

}


uint8_t
UTIL_MDIOInterface(void)
{
    uint8_t ucState = STATE_OK;

    ucState = UTIL_MDIOInterfaceTest();

    if (ucState == STATE_ERROR) {
        printf("MDIO Interface error\r\n");
    }

    return ucState;

}


int32_t
main(int32_t argc, char *argv[])
{
    uint32_t ulLoopNum = 1;
    uint8_t ucState = STATE_OK;
    if ( NULL == argv[1] ) {
        printf("usage  : api_c_main loopNum\n");
        printf("example: api_c_main 10\n");
        printf("\r\n");
        return -1;
    }
    ulLoopNum = atoi(argv[1]);
    for (uint32_t i = 0; i < ulLoopNum; i++) {

        printf("\r\n------------------------------Start %d Loop-------------------------------------------\r\n", i + 1);
        #if 0
        printf("\r\nSystem Interface information:\r\n");
        ucState = UTIL_SystemInterface();
        if (ucState == STATE_ERROR) {
            printf("SystemInterface error\r\n");
        }
        #endif
        printf("\r\nI2C Interface information:\r\n");
        ucState = UTIL_I2CInterface();
        if (ucState == STATE_ERROR) {
            printf("I2C Interface error\r\n");
        }
        #if 0
        printf("\r\nSPI Interface information:\r\n");
        ucState = UTIL_SPIInterface();
        if (ucState == STATE_ERROR) {
            printf("SPI Interface error\r\n");
        }
        #endif
        printf("\r\nMDIO Interface information:\r\n");
        ucState = UTIL_MDIOInterface();
        if (ucState == STATE_ERROR) {
            printf("MDIO Interface error\r\n");
        }

        printf("-----------------------------------------------Interface Finish %d Loop-----------------------------------------------\r\n", i + 1);

    }
}
#if 0
int
main()
{
    CMfg stMfg;
    /*BOARD_GetChassisMfg(&stMfg); */
    EDFA_GetMfg(1, &stMfg);
    WSS_GetMfg(9, &stMfg);
    WSS_GetMfg(10, &stMfg);
    return 0;
}
#endif
