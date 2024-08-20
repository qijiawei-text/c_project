#include "MfgFile.h"

#define MAC_ADDR_LEN                (17)

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
#define MODULE_FUJITSU_SN           "MODULE_FUJITSU_SN"

/* Ueno project has config-1/confg-2/config-3/config-4 type    */
/*   Config-1 is  OPS + AMP + AMP-P,    define it to be 'AMP'  */
/*   Config-2/3/4 is OPS + OPS4 + OPS4, define it to be 'OPS'  */
/* Using the 'MODULE_CONFIG_TYPE' to seperate the AMP or OPS   */
#define MODULE_CONFIG_TYPE          "MODULE_CONFIG_TYPE"

bool
CMfgFile::FindKeyWordValue(FILE *fpFile, const char *pacInKeyWord, char *pacOutValue)
{
    char acLine[256] = {0};
    char acParaName[256] = {0};
    char acParaVlaue[256] = {0};

    if (NULL == fpFile || NULL == pacInKeyWord || NULL == pacOutValue) {
        return false;
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
                LOG_WRITE(EN_LOG_ERROR, "value is too long, not bigger than %d", (MFG_INFO_LEN_MAX - 1));
                return false;
            }

            strcpy(pacOutValue, acParaVlaue);
            /* OPLK_LOG_PRINT_DBG("find %s is %s", pacInKeyWord, pacOutValue); */
            return true;
        }

        memset(acLine, 0, sizeof(acLine));
    }

    LOG_WRITE(EN_LOG_ERROR, "can not find keyword:%s", pacInKeyWord);
    return false;

}

bool
CMfgFile::FindKeyWordShortValue(FILE *fpFile, const char *pacInKeyWord, short *psOutValue)
{
    char acLine[256] = {0};
    char acParaName[256] = {0};
    short sVal = 0;

    if (NULL == fpFile || NULL == pacInKeyWord || NULL == psOutValue) {
        return false;
    }

    /****************************************************************************/
    /* read one line from file                                                  */
    /****************************************************************************/
    fseek(fpFile, 0, SEEK_SET);
    while (fgets(acLine, sizeof(acLine), fpFile)) {
        if ('#' == acLine[0]) {
            continue;
        }

        sscanf(acLine, "%s %hd", acParaName, &sVal);
        if (0 == strncasecmp(acParaName, pacInKeyWord, strlen(pacInKeyWord))) {
            *psOutValue = sVal;

            /* OPLK_LOG_PRINT_DBG("find %s is %d", pacInKeyWord, sVal); */
            return true;
        }

        memset(acLine, 0, sizeof(acLine));
    }

    LOG_WRITE(EN_LOG_ERROR, "can not find keyword:%s", pacInKeyWord);
    return false;

}

bool
CMfgFile::CheckMacAddr(const char *pcMac)
{
    char acTmpBuf[64];
    char *pcStrtmp = NULL;
    unsigned char ucHexNum = 0;

    if ((NULL == pcMac) || (MAC_ADDR_LEN != strlen(pcMac))) {
        return false;
    }

    memset(acTmpBuf, 0, sizeof(acTmpBuf));
    strncpy(acTmpBuf, pcMac, sizeof(acTmpBuf));
    pcStrtmp = strtok(acTmpBuf, ":");

    while (NULL != pcStrtmp) {
        if (2 != strlen(pcStrtmp)) {
            return false;
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
                return false;
            }
        } else {
            return false;
        }
    }

    if (6 != ucHexNum) {
        return false;
    }
    return true;
}


bool
CMfgFile::ReadMfg(const char *c_pcFilePath, CMfg &stMfg)
{
    FILE *fpFile = NULL;

    if (NULL == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr is null");
        return false;
    }

    fpFile = fopen(c_pcFilePath, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s", c_pcFilePath);
        return false;
    }

    if (!FindKeyWordValue(fpFile, MODULE_MFG_NAME, stMfg.acManufacturer)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_PRODUCT_NAME, stMfg.acProductName)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_TYPE, stMfg.acModuleType)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_SW_VER, stMfg.acSwVer)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_FW_VER, stMfg.acFwVer)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_HW_REV, stMfg.acHwVer)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_PCB_REV, stMfg.acPcbRev)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_SN, stMfg.acSN)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_PART_NO, stMfg.acPN)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_MFG_DATE, stMfg.acMfgDate)) {
        goto ERR_LABEL;
    }

    strcat(stMfg.acMfgDate, "T00:00:00Z");

    if (!FindKeyWordValue(fpFile, MODULE_CAL_DATE, stMfg.acCaliDate)) {

    }

    strcat(stMfg.acCaliDate, "T00:00:00Z");

    if (!FindKeyWordValue(fpFile, MODULE_CLEI_CODE, stMfg.acCleiCode)) {

    }

    if (!FindKeyWordValue(fpFile, MODULE_SUB_BOARD_REV, stMfg.acSubBdlVer)) {

    }

    if (!FindKeyWordValue(fpFile, MAC_ADDRESS_ETH0, stMfg.acEth0Mac)) {

    } else if (!CheckMacAddr(stMfg.acEth0Mac)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MAC_ADDRESS_ETH1, stMfg.acEth1Mac)) {

    } else if (!CheckMacAddr(stMfg.acEth1Mac)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MAC_ADDRESS_ETH2, stMfg.acEth2Mac)) {

    } else if (!CheckMacAddr(stMfg.acEth2Mac)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MAC_ADDRESS_ETH3, stMfg.acEth3Mac)) {

    } else if (!CheckMacAddr(stMfg.acEth3Mac)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MAC_ADDRESS_ETH4, stMfg.acEth4Mac)) {

    } else if (!CheckMacAddr(stMfg.acEth4Mac)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MAC_ADDRESS_ETH5, stMfg.acEth5Mac)) {

    } else if (!CheckMacAddr(stMfg.acEth5Mac)) {
        goto ERR_LABEL;
    }
#ifdef __NR1004__
    /* fnc additional */
    if (!FindKeyWordValue(fpFile, MODULE_UNIT_NAME, stMfg.acUnitName)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_FC_PN, stMfg.acFCPN)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_FC_ISS, stMfg.acFCIss)) {
        goto ERR_LABEL;
    }

    if (!FindKeyWordValue(fpFile, MODULE_FUJITSU_PN, stMfg.acFujitsuPN)) {
        goto ERR_LABEL;
    }
#endif
    if (!FindKeyWordValue(fpFile, MODULE_CONFIG_TYPE, stMfg.acConfigType)) {
    }

    if (!FindKeyWordValue(fpFile, MODULE_FUJITSU_SN, stMfg.acFjtSn)) {
    }

    if (ferror(fpFile)) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to ferror %s", c_pcFilePath);
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(fpFile);

    return true;
ERR_LABEL:
    DEF_SAFE_FCLOSE(fpFile);
    return false;
}


bool
CMfgFile::ReadCali(const char *c_pcFilePath, CCali &stCali)
{
    FILE *fpFile = NULL;

    if (NULL == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "input ptr is null");
        return false;
    }

    fpFile = fopen(c_pcFilePath, "r");
    if (NULL == fpFile) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to fopen. FilePath = %s", c_pcFilePath);
        return false;
    }

    FindKeyWordShortValue(fpFile, "LINE_IN_TO_PA_IN", &stCali.sLineInToPaIn);
    FindKeyWordShortValue(fpFile, "LINE_IN_TO_BA_IN", &stCali.sLineInToBaIn);
    FindKeyWordShortValue(fpFile, "PA_OUT_TO_LINE_OUT", &stCali.sPaOutToLineOut);
    FindKeyWordShortValue(fpFile, "BA_OUT_TO_LINE_OUT", &stCali.sBaOutToLineOut);

    FindKeyWordShortValue(fpFile, "PA_IN_TO_OCM", &stCali.sPaInToOcm);
    FindKeyWordShortValue(fpFile, "PA_OUT_TO_OCM", &stCali.sPaOutToOcm);
    FindKeyWordShortValue(fpFile, "BA_IN_TO_OCM", &stCali.sBaInToOcm);
    FindKeyWordShortValue(fpFile, "BA_OUT_TO_OCM", &stCali.sBaOutToOcm);
    FindKeyWordShortValue(fpFile, "WSS1_IN_TO_OCM", &stCali.sWss1InToOcm);
    FindKeyWordShortValue(fpFile, "WSS1_OUT_TO_OCM", &stCali.sWss1OutToOcm);
    FindKeyWordShortValue(fpFile, "WSS2_IN_TO_OCM", &stCali.sWss2InToOcm);
    FindKeyWordShortValue(fpFile, "WSS2_OUT_TO_OCM", &stCali.sWss2OutToOcm);

    FindKeyWordShortValue(fpFile, "LINE_IN_TO_OSC1_DROP", &stCali.sLineInToOsc1Drop);
    FindKeyWordShortValue(fpFile, "OSC1_ADD_TO_LINE_OUT", &stCali.sOsc1AddToLineOut);
    FindKeyWordShortValue(fpFile, "LINE_IN_TO_OSC1_RX", &stCali.sLineInToOsc1Rx);
    FindKeyWordShortValue(fpFile, "OSC1_TX_TO_LINE_OUT", &stCali.sOsc1TxToLineOut);
    FindKeyWordShortValue(fpFile, "LINE_IN_TO_OSC2_DROP", &stCali.sLineInToOsc2Drop);
    FindKeyWordShortValue(fpFile, "OSC2_ADD_TO_LINE_OUT", &stCali.sOsc2AddToLineOut);
    FindKeyWordShortValue(fpFile, "LINE_IN_TO_OSC2_RX", &stCali.sLineInToOsc2Rx);
    FindKeyWordShortValue(fpFile, "OSC2_TX_TO_LINE_OUT", &stCali.sOsc2TxToLineOut);

    FindKeyWordShortValue(fpFile, "MIRROR_TX_TO_MUX_IN", &stCali.sMirrorTxToMuxIn);
    FindKeyWordShortValue(fpFile, "MIRROR_TX_TO_DEMUX_IN", &stCali.sMirrorTxToDemuxIn);
    FindKeyWordShortValue(fpFile, "MUX_IN_TO_MIRROR_RX", &stCali.sMuxInToMirrorRx);
    FindKeyWordShortValue(fpFile, "DEMUX_IN_TO_MIRROR_RX", &stCali.sDemuxInToMirrorRx);

    stCali.sOcmPortToOcm = 50;
    FindKeyWordShortValue(fpFile, "OCM_PORT_TO_OCM", &stCali.sOcmPortToOcm);

    if (ferror(fpFile)) {
        LOG_WRITE(EN_LOG_ERROR, "Fail to ferror %s", c_pcFilePath);
        return false;
    }

    fclose(fpFile);
    fpFile = NULL;

    return true;
}


