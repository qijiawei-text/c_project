#include "EEpromOperator.h"
#include "util_crc.h"
#include "MfgFile.h"
#include "File.h"

#define EEPROM_ADDR_MFG_INFO                (0x00000000)
#define EEPROM_ADDR_RPC_MFG_INFO            (0x00001000)
#define EEPROM_ADDR_UPG_INFO                (0x00002000)
#define EEPROM_ADDR_SYS_INFO                (0x00002400)
#define EEPROM_ADDR_COM_CALI                (0x00002800)
/* #define EEPROM_ADDR_WSS_CALI                (0x00003000) */
#define EEPROM_ADDR_CFG_INFO                (0x00003000)
#define EEPROM_ADDR_BDL_HEAD                (0x0001E000)

#define ADDR_SLICE_MAX                      (108)
#define DEF_SLICE_SIZE_UNIT                 (1 * 1024)

bool
CEEpromOperator::ReadMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, CMfg &rstMfg)
{
    /* LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadMfg +++"); */


    uint8_t aucBuf[EEPROM_MFG_BUF_LEN] = {0};

    if (false == Read(c_rstpEEprom, EEPROM_ADDR_MFG_INFO, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Mfg error.");
        return false;
    }

    memcpy((uint8_t *)&rstMfg, aucBuf, sizeof(rstMfg));

    /* LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadMfg ---"); */

    return true;

}

bool
CEEpromOperator::ReadMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, CMfg &rstMfg, uint32_t uiIdx)
{
    /* LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadMfg +++"); */

    uint8_t aucBuf[EEPROM_MFG_BUF_LEN] = {0};

    if (1 != uiIdx) {
        LOG_WRITE(EN_LOG_ERROR, "Idx error. Idx %d", uiIdx);
        return false;        
    }

    if (false == Read(c_rstpEEprom, EEPROM_ADDR_RPC_MFG_INFO, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Mfg error.");
        return false;
    }

    memcpy((uint8_t *)&rstMfg, aucBuf, sizeof(rstMfg));

    /* LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadMfg ---"); */

    return true;

}

bool
CEEpromOperator::WriteMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcFilePath)
{
    CMfg stMfg;
    CMfgFile stMfgFile;

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteMfg +++");

    if (NULL == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFilePath is null");
        return false;
    }

    memset(&stMfg, 0, sizeof(stMfg));
    if (false == stMfgFile.ReadMfg(c_pcFilePath, stMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "read mfg file error. FilePath = %s", c_pcFilePath);
        return false;
    }

    if (false == Write(c_rstpEEprom, EEPROM_ADDR_MFG_INFO, (uint8_t *)&stMfg, sizeof(stMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Write Mfg error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteMfg ---");

    return true;


}

bool
CEEpromOperator::WriteMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcFilePath, uint32_t uiIdx)
{
    /* for RPC MFG */
    CMfg stMfg;
    CMfgFile stMfgFile;

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteMfg +++");

    if (NULL == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFilePath is null");
        return false;
    }

    memset(&stMfg, 0, sizeof(stMfg));
    if (false == stMfgFile.ReadMfg(c_pcFilePath, stMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "read mfg file error. FilePath = %s", c_pcFilePath);
        return false;
    }

    if (1 != uiIdx) {
        LOG_WRITE(EN_LOG_ERROR, "Idx error. File path %s Idx %d", c_pcFilePath, uiIdx);
        return false;
    }

    if (false == Write(c_rstpEEprom, EEPROM_ADDR_RPC_MFG_INFO, (uint8_t *)&stMfg, sizeof(stMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Write Mfg error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteMfg ---");

    return true;
}

bool
CEEpromOperator::WriteMfgConfigType(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcConfigType)
{
    CMfg stMfg;
    uint8_t aucBuf[EEPROM_MFG_BUF_LEN] = {0};

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteMfgConfigType +++");

    if (NULL == c_pcConfigType) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFilePath is null");
        return false;
    }

    memset(&stMfg, 0, sizeof(stMfg));
    if (false == Read(c_rstpEEprom, EEPROM_ADDR_MFG_INFO, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Mfg error.");
        return false;
    }

    memcpy((uint8_t *)&stMfg, aucBuf, sizeof(stMfg));
    LOG_WRITE(EN_LOG_NOTICE, "Current config type: %s, target: %s",
              stMfg.acConfigType, c_pcConfigType);

    strncpy(stMfg.acConfigType, c_pcConfigType, sizeof(stMfg.acConfigType) - 1);

    if (false == Write(c_rstpEEprom, EEPROM_ADDR_MFG_INFO, (uint8_t *)&stMfg, sizeof(stMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Write Mfg error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteMfg ---");

    return true;


}

int
CEEpromOperator::ReadCfg(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddrSlice, const char *pcFilePath)
{
    int iRet = 0;
    FILE *fp = NULL;
    uint8_t *pcBuffer = NULL;
    CONF_FILE_HEADER_ST stFileHeader;
    char acTarPathTmp[128];
    uint32_t uiAddrOffset = 0;
    int iResult = OPLK_ERR;
    uint32_t uiBodyCrc = 0;

    if (NULL == pcFilePath) {
        return iResult;
    }

    LOG_WRITE(EN_LOG_DEBUG, "++++ Restore configuration data from eeprom to filepath[%s] start", pcFilePath);

    if (false == GetAddrOffset(uiAddrSlice, uiAddrOffset)) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom addr error");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "uiAddrSlice = %d uiAddrOffset = 0x%02X", uiAddrSlice, uiAddrOffset);

    if (false == c_rstpEEprom->Read(uiAddrOffset, (uint8_t *)&stFileHeader, sizeof(stFileHeader))) {
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    if (false == CFile::CheckHeader(stFileHeader)) {
        LOG_WRITE(EN_LOG_ERROR, "**** Check header file error");
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    if (stFileHeader.iFileLen < 0 || (uint32_t)stFileHeader.iFileLen > 32 * DEF_1KB) {
        LOG_WRITE(EN_LOG_ERROR, "stFileHeader.iFileLen error. uiDataLen = %d", stFileHeader.iFileLen);
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    if (0 == stFileHeader.iFileLen) {
        LOG_WRITE(EN_LOG_ERROR, "The eeprom had been written, but no file in it");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "stFileHeader.iFileLen = %d", stFileHeader.iFileLen);

    /* read from eeprom */
    pcBuffer = (uint8_t *)malloc(stFileHeader.iFileLen);
    if (NULL == pcBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "**** Malloc buffer error, len %d", stFileHeader.iFileLen);
        goto ERR_LABEL;
    }
    memset(pcBuffer, 0x00, stFileHeader.iFileLen);

    if (false == c_rstpEEprom->Read(uiAddrOffset + sizeof(CONF_FILE_HEADER_ST), pcBuffer, stFileHeader.iFileLen)) {
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    uiBodyCrc = UTIL_Crc32Calc((uint8_t *)pcBuffer, (uint32_t)stFileHeader.iFileLen);
    if (stFileHeader.astInfo[0].uiBodyCrc != uiBodyCrc) {
        LOG_WRITE(EN_LOG_ERROR, "Read file body crc error: %08x, %08x", stFileHeader.astInfo[0].uiBodyCrc, uiBodyCrc);
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    memset(acTarPathTmp, 0, sizeof(acTarPathTmp));
    strncpy(acTarPathTmp, pcFilePath, (sizeof(acTarPathTmp) - 1));

    if (FILE_TYPE_TAR == stFileHeader.iType) {
        fp = fopen(acTarPathTmp, "w+");
        if (NULL == fp) {
            LOG_WRITE(EN_LOG_ERROR, "**** Fopen file %s error", acTarPathTmp);
            goto ERR_LABEL;
        }

        iRet = fwrite(pcBuffer, sizeof(uint8_t), stFileHeader.iFileLen, fp);
        if (iRet != stFileHeader.iFileLen) {
            LOG_WRITE(EN_LOG_ERROR, "**** Fwrite error, ret %d, len %d", iRet, stFileHeader.iFileLen);
            goto ERR_LABEL;
        }

        DEF_SAFE_FCLOSE(fp);

        if (false == CheckCfgTar(acTarPathTmp)) {
            goto ERR_LABEL;
        }
    } else {
        /*tbd*/
    }

    LOG_WRITE(EN_LOG_DEBUG, "---- Get board configuration data from eeprom to filepath[%s] succ", pcFilePath);
    DEF_SAFE_FCLOSE(fp);
    DEF_SAFE_FREE(pcBuffer);
    return OPLK_OK;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "**** Get board configuration data from eeprom error");
    DEF_SAFE_FCLOSE(fp);
    DEF_SAFE_FREE(pcBuffer);
    return iResult;
}


int
CEEpromOperator::WriteCfg(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddrSlice, uint32_t uiSize, const char *pcFilePath)
{
    int iFileLen = 0;
    char *pcBuffer = NULL;
    CONF_FILE_HEADER_ST stFileHeader;
    uint32_t uiAddrOffset = 0;
    int iResult = OPLK_ERR;

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteCfg +++");

    if (nullptr == pcFilePath || nullptr == c_rstpEEprom) {
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Save file name:%s slice:%d size:%d start", pcFilePath, uiAddrSlice, uiSize);

    CFile::InitHeader(stFileHeader);

    pcBuffer = CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pcBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    if ((uiSize > 0) &&
        (iFileLen + sizeof(stFileHeader)) > (uiSize * DEF_SLICE_SIZE_UNIT)) {
        LOG_WRITE(EN_LOG_ERROR, "File = %s size out of range, filesize = %d, headerlen = %lu, maxsize = %d",
                  pcFilePath, iFileLen, sizeof(stFileHeader), (uiSize * DEF_SLICE_SIZE_UNIT));
        goto ERR_LABEL;
    }

    stFileHeader.iFileCount = 1;
    stFileHeader.iFileLen = iFileLen;
    stFileHeader.iType = FILE_TYPE_TAR;
    stFileHeader.astInfo[0].iAddr = 0;
    stFileHeader.astInfo[0].iLen = iFileLen;
    stFileHeader.astInfo[0].uiBodyCrc = UTIL_Crc32Calc((uint8_t *)pcBuffer, (uint32_t)iFileLen);
    stFileHeader.uiHeaderCrc = UTIL_Crc32Calc((uint8_t *)&stFileHeader, sizeof(stFileHeader) - sizeof(uint32_t));

    if (false == GetAddrOffset(uiAddrSlice, uiAddrOffset)) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom addr error");
        goto ERR_LABEL;
    }


    if (false == c_rstpEEprom->Write(uiAddrOffset, (UCHAR *)&stFileHeader, sizeof(stFileHeader))) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom cfg write header error");
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    if (false == c_rstpEEprom->Write(uiAddrOffset + sizeof(CONF_FILE_HEADER_ST),
                                     (UCHAR *)pcBuffer,
                                     iFileLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Eeprom cfg write buffer error");
        iResult = ERR_OPERATION_FAILED;
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_DEBUG, "Save board file name %s succ", pcFilePath);

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteCfg ---");

    DEF_SAFE_FREE(pcBuffer);
    return OPLK_OK;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CEEpromOperator::WriteCfg error ---");

    DEF_SAFE_FREE(pcBuffer);
    return iResult;
}


bool
CEEpromOperator::ReadComCali(const std::shared_ptr<CEEprom> c_rstpEEprom, CCali &rstCali)
{
    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadComCali +++");

    uint8_t aucBuf[EEPROM_MFG_BUF_LEN] = {0};

    if (false == Read(c_rstpEEprom, EEPROM_ADDR_COM_CALI, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Com Cali error.");
        return false;
    }

    memcpy((uint8_t *)&rstCali, aucBuf, sizeof(rstCali));

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadComCali ---");

    return true;


}


bool
CEEpromOperator::WriteComCali(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcFilePath)
{
    CCali stCali;
    CMfgFile stMfgFile;

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteComCali +++");

    if (nullptr == c_pcFilePath) {
        LOG_WRITE(EN_LOG_ERROR, "c_pcFilePath is nullptr.");
        return false;
    }

    memset(&stCali, 0, sizeof(stCali));
    if (false == stMfgFile.ReadCali(c_pcFilePath, stCali)) {
        LOG_WRITE(EN_LOG_ERROR, "read cali file error. FilePath = %s", c_pcFilePath);
        return false;
    }

    if (false == Write(c_rstpEEprom, EEPROM_ADDR_COM_CALI, (uint8_t *)&stCali, sizeof(stCali))) {
        LOG_WRITE(EN_LOG_ERROR, "Write Com Cali error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteComCali ---");

    return true;

}


bool
CEEpromOperator::ReadUpgInfo(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_NOTICE, "CEEpromOperator::ReadUpgInfo +++");

    if (false == Read(c_rstpEEprom, EEPROM_ADDR_UPG_INFO, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read Upgrade Info error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CEEpromOperator::ReadUpgInfo ---");

    return true;
}


bool
CEEpromOperator::WriteUpgInfo(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_NOTICE, "CEEpromOperator::WriteUpgInfo +++");

    if (false == Write(c_rstpEEprom, EEPROM_ADDR_UPG_INFO, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write Upgrade Info error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CEEpromOperator::WriteUpgInfo ---");

    return true;

}


bool
CEEpromOperator::ReadBdlHead(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadBdlHead +++");

    if (false == Read(c_rstpEEprom, EEPROM_ADDR_BDL_HEAD, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Read Bundle Head error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::ReadBdlHead ---");

    return true;

}


bool
CEEpromOperator::WriteBdlHead(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen)
{
    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteBdlHead +++");

    if (false == Write(c_rstpEEprom, EEPROM_ADDR_BDL_HEAD, pucBuf, uiLen)) {
        LOG_WRITE(EN_LOG_ERROR, "Write Bundle Head error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteBdlHead ---");

    return true;
}



bool
CEEpromOperator::GetAddrOffset(uint32_t uiAddrSlice, uint32_t &uiEEpromAddr)
{
    uint32_t uiAddrOffset = 0;

    if (uiAddrSlice > ADDR_SLICE_MAX) {
        return false;
    }

    uiAddrOffset = uiAddrSlice * DEF_SLICE_SIZE_UNIT;
    uiEEpromAddr = EEPROM_ADDR_CFG_INFO + uiAddrOffset;

    return true;
}

bool
CEEpromOperator::Read(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    if (nullptr == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "pucBuf is nullptr.");
        return false;
    }

    memset(pucBuf, 0, uiLen);

    EEPROM_CFG_HEAD_ST stEeHead;
    memset(&stEeHead, 0, sizeof(stEeHead));

    if (false == c_rstpEEprom->Read(uiAddr, (uint8_t *)&stEeHead, sizeof(stEeHead))) {
        LOG_WRITE(EN_LOG_ERROR, "read header error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "uiHeadCrc32 = 0x%02X uiDataLen = 0x%02X uiDataCrc32 = 0x%02X uiCfgVersion = 0x%02X",
              stEeHead.uiHeadCrc32, stEeHead.uiDataLen, stEeHead.uiDataCrc32, stEeHead.uiCfgVersion);

    if (stEeHead.uiHeadCrc32 != UTIL_Crc32Calc((UCHAR *)&stEeHead + sizeof(stEeHead.uiHeadCrc32), sizeof(stEeHead) - sizeof(stEeHead.uiHeadCrc32))) {
        LOG_WRITE(EN_LOG_ERROR, "header crc(0x%0X) is error", stEeHead.uiHeadCrc32);
        return false;
    }

    if (stEeHead.uiDataLen > uiLen) {
        LOG_WRITE(EN_LOG_ERROR, "uiDataLen > uiLen. uiDataLen = %d, uiLen = %d", stEeHead.uiDataLen, uiLen);
        return false;
    }

    if (false == c_rstpEEprom->Read(uiAddr + sizeof(stEeHead), pucBuf, stEeHead.uiDataLen)) {
        LOG_WRITE(EN_LOG_ERROR, "read error.");
        return false;
    }

    if (stEeHead.uiDataCrc32 != UTIL_Crc32Calc(pucBuf, stEeHead.uiDataLen)) {
        LOG_WRITE(EN_LOG_ERROR, "crc(0x%0X) is error", stEeHead.uiDataCrc32);
        return false;
    }

    return true;
}

bool
CEEpromOperator::Write(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen)
{
    EEPROM_CFG_HEAD_ST stEeHead;

    LOG_WRITE(EN_LOG_DEBUG, "CEEpromOperator::WriteUpgInfo +++");

    if (nullptr == pucBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input is nullptr");
        return false;
    }

    memset(&stEeHead, 0, sizeof(stEeHead));
    stEeHead.uiDataLen = uiLen;
    stEeHead.uiDataCrc32 = UTIL_Crc32Calc((uint8_t *)pucBuf, stEeHead.uiDataLen);
    stEeHead.uiHeadCrc32 = UTIL_Crc32Calc((uint8_t *)&stEeHead + sizeof(stEeHead.uiHeadCrc32), sizeof(stEeHead) - sizeof(stEeHead.uiHeadCrc32));

    LOG_WRITE(EN_LOG_DEBUG, "uiHeadCrc32 = 0x%02X uiDataCrc32 = 0x%02X", stEeHead.uiDataCrc32, stEeHead.uiHeadCrc32);
    if (false == c_rstpEEprom->Write(uiAddr, (uint8_t *)&stEeHead, sizeof(stEeHead))) {
        LOG_WRITE(EN_LOG_ERROR, "write header is error");
        return false;
    }

    if (false == c_rstpEEprom->Write(uiAddr + sizeof(stEeHead), (uint8_t *)pucBuf, stEeHead.uiDataLen)) {
        LOG_WRITE(EN_LOG_ERROR, "write body is error");
        return false;
    }

    return true;
}

bool
CEEpromOperator::CheckCfgTar(char *pcTarFileName)
{
    char acBuffer[1024] = {0};
    int iResult = 0;

    if (NULL == pcTarFileName) {
        return false;
    }

    snprintf(acBuffer, sizeof(acBuffer), "tar -tf %s >/dev/null 2>&1", pcTarFileName);

    iResult = system(acBuffer);
    if (iResult != 0) {
        LOG_WRITE(EN_LOG_ERROR, "invalid .tgz file: %s", pcTarFileName);
        return false;
    }
    return true;
}

