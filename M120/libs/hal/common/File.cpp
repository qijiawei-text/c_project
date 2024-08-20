#include "File.h"
#include "util_crc.h"
#include "RspHandler.h"

int
CFile::GetLength(const char *pcFilePath)
{
    FILE *pstFp = NULL;
    struct stat stFileStat;
    uint32_t uiFileLen = 0;
    int iRet = 0;


    /* check input point and init data  */
    if (NULL == pcFilePath) {
        goto ERR_LABEL;
    }
    memset(&stFileStat, 0, sizeof(stFileStat));

    /* open file by file name                   */
    if (NULL == (pstFp = fopen(pcFilePath, "r"))) {
        goto ERR_LABEL;
    }

    /* get file information                       */
    if (0 > fstat(fileno(pstFp), &stFileStat)) {
        goto ERR_LABEL;
    }

    /* get file lengh and return                 */
    uiFileLen = stFileStat.st_size;
    DEF_SAFE_FCLOSE(pstFp);

    return uiFileLen;

ERR_LABEL:
    DEF_SAFE_FCLOSE(pstFp);
    return iRet;
}

char *
CFile::Malloc(const char *pcFileName, int &iFileLen)
{
    char *pcBuffer = NULL;
    FILE *pstFile = NULL;


    if (nullptr == pcFileName) {
        goto ERR_LABEL;
    }

    /* Get manifest length for malloc RAM. */
    iFileLen = GetLength(pcFileName);
    if (iFileLen <= 0) {
        LOG_WRITE(EN_LOG_ERROR, "iFileLen <= 0. iFileLen = %d", iFileLen);
        goto ERR_LABEL;
    }

    /* malloc RAM */
    if (nullptr == (pcBuffer = (char *)malloc(iFileLen))) {
        LOG_WRITE(EN_LOG_ERROR, "malloc error: %d", iFileLen);
        goto ERR_LABEL;
    }

    /* Copy the file into RAM buffer.*/
    if (nullptr == (pstFile = fopen(pcFileName, "rb"))) {
        LOG_WRITE(EN_LOG_ERROR, "fopen error. pcFileName = %s", pcFileName);
        goto ERR_LABEL;
    }

    /*Copy manifest file to RAM.*/
    memset(pcBuffer, 0x00, iFileLen);
    if (iFileLen != (int)fread(pcBuffer, sizeof(uint8_t), iFileLen, pstFile)) {
        LOG_WRITE(EN_LOG_ERROR, "fread error. iFileLen = %d", iFileLen);
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(pstFile);

    return pcBuffer;

ERR_LABEL:

    DEF_SAFE_FCLOSE(pstFile);
    DEF_SAFE_FREE(pcBuffer);
    return nullptr;
}

bool
CFile::Write(const char *pcFileName, uint8_t *pucBuf, uint32_t ulLen)
{
    FILE *pstFile = NULL;

    if (nullptr == (pstFile = fopen(pcFileName, "wb"))) {
        LOG_WRITE(EN_LOG_ERROR, "fopen error. pcFileName = %s", pcFileName);
        goto ERR_LABEL;
    }

    if (ulLen != (uint32_t)fwrite(pucBuf, sizeof(uint8_t), ulLen, pstFile)) {
        LOG_WRITE(EN_LOG_ERROR, "fwrite error. iFileLen = %d", ulLen);
        goto ERR_LABEL;
    }

    DEF_SAFE_FCLOSE(pstFile);

    return true;

ERR_LABEL:

    DEF_SAFE_FCLOSE(pstFile);
    return false;
}

bool
CFile::InitHeader(CONF_FILE_HEADER_ST &rstHeader)
{
    memset(&rstHeader, 0x00, sizeof(CONF_FILE_HEADER_ST));
    strncpy(rstHeader.acKeyWord, HEADER_KEYWORD, sizeof(rstHeader.acKeyWord) - 1);
    strncpy(rstHeader.acVersion, HEADER_VERSION, sizeof(rstHeader.acVersion) - 1);
    rstHeader.iFileLen = 0;
    rstHeader.iFileCount = 0;
    rstHeader.iType = FILE_TYPE_TAR;
    rstHeader.uiHeaderCrc = UTIL_Crc32Calc((uint8_t *)&rstHeader, sizeof(CONF_FILE_HEADER_ST) - sizeof(rstHeader.uiHeaderCrc));

    return true;
}

bool
CFile::CheckHeader(const CONF_FILE_HEADER_ST &c_rstHeader)
{
    uint32_t uiCrc = 0;

    uiCrc = UTIL_Crc32Calc((uint8_t *)&c_rstHeader, sizeof(CONF_FILE_HEADER_ST) - sizeof(c_rstHeader.uiHeaderCrc));
    if (uiCrc != c_rstHeader.uiHeaderCrc) {
        goto ERR_LABEL;
    }

    if (c_rstHeader.iFileCount > MAX_FILE_COUNT || c_rstHeader.iFileCount < 0) {
        goto ERR_LABEL;
    }

    if (c_rstHeader.iType < FILE_TYPE_ORG && c_rstHeader.iType >= FILE_TYPE_MAX) {
        goto ERR_LABEL;
    }

    if (c_rstHeader.iFileLen < 0) {
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:

    return false;
}

#if 0
bool
CFile::XModem(const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, const std::string &c_rstaStartCmd, uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{

    uint8_t aucXmdPkg[XMD_PKG_LEN] = {0};
    uint8_t aucBuf[XMD_PKG_LEN] = {0};
    uint32_t ulDataLen = 0;
    uint32_t ulPkgCnt = 0;
    uint16_t usCrc16;
    uint8_t ucPackNum = 0;
    uint32_t i = 0, j = 0;
    uint8_t *pucRspData = nullptr;
    std::shared_ptr<CRspHandler> stpRspHandler = nullptr;
    std::shared_ptr<CEndToken> stpEndToken = nullptr;
    std::string staRsp;
    uint32_t uiRetryTimes = 0;

    for ( uiRetryTimes = 0; uiRetryTimes < 5; uiRetryTimes++) {
        LOG_WRITE(EN_LOG_NOTICE, "StartCmd = %s", c_rstaStartCmd.c_str());
        stpRspHandler = std::make_shared<CRspHandler>(1);
        if (!c_rstpDrvPtr->ReadWriteUnlock(uiIdx, (uint8_t *)c_rstaStartCmd.c_str(), c_rstaStartCmd.size(), aucBuf, stpRspHandler)) {
            LOG_WRITE(EN_LOG_ERROR, "Receive 'C' from module error.");
            goto Retry_LABEL;
        }

        pucRspData = stpRspHandler->Data();
        if (XMD_CHK_TYPE_CRC_16 != pucRspData[0]) {
            LOG_WRITE(EN_LOG_ERROR, "Receive 'C' from module error.");
            goto Retry_LABEL;;
        }

        /* Start count the package number                                           */
        ucPackNum = 1;
        ulPkgCnt = (ulLen % XMD_PKG_SIZE_1KB) ? ((ulLen / XMD_PKG_SIZE_1KB) + 1) : (ulLen / XMD_PKG_SIZE_1KB);
        for (i = 0; i < ulPkgCnt; i++) {
            memset(aucXmdPkg, 0x00, sizeof(aucXmdPkg));
            memset(aucBuf, 0x00, sizeof(aucBuf));

            /* Creates the Xmodem package header                                    */
            aucXmdPkg[XMD_PKG_IDX_TYPE] = XMD_STX;
            aucXmdPkg[XMD_PKG_IDX_SEQ] = ucPackNum;
            aucXmdPkg[XMD_PKG_IDX_SEQCK] = ~ucPackNum;

            /* Copies the package data                                              */
            if (i < (ulLen / XMD_PKG_SIZE_1KB)) {
                ulDataLen = XMD_PKG_SIZE_1KB;
            } else {
                ulDataLen = (ulLen % XMD_PKG_SIZE_1KB);
            }
            memcpy(&aucXmdPkg[XMD_PKG_IDX_DATA], &pucBuf[i * XMD_PKG_SIZE_1KB], ulDataLen);

            /* Appends the CRC16                                                    */
            usCrc16 = UTIL_Crc16Calc(&aucXmdPkg[XMD_PKG_IDX_DATA], XMD_PKG_SIZE_1KB);
            aucXmdPkg[XMD_PKG_IDX_1KB_CK0] = usCrc16 >> 8;
            aucXmdPkg[XMD_PKG_IDX_1KB_CK1] = usCrc16 & 0xFF;

            /* Sends the Xmodem package to the module, then wait for a response     */
            for (j = 0; j < XMD_RETRY_MAX; j++) {

                LOG_WRITE(EN_LOG_NOTICE, "HwId[%d] ucPackNum = %d.", uiIdx, ucPackNum);
                /* Sends the Xmodem package to the module                           */
                stpRspHandler = std::make_shared<CRspHandler>(1);

                if (!c_rstpDrvPtr->ReadWriteUnlock(uiIdx, aucXmdPkg, XMD_PKG_LEN, nullptr, stpRspHandler)) {
                    LOG_WRITE(EN_LOG_ERROR, "HwId[%d] Send Command error.", uiIdx);
                    CDelay::Delay(20, MS);
                    continue;
                }

                pucRspData = (uint8_t *)stpRspHandler->Data();

                if (XMD_ACK == pucRspData[0]) {
                    break;
                } else if (XMD_CAN == pucRspData[0]) {
                    LOG_WRITE(EN_LOG_ERROR, "Response CAN.");
                    goto Retry_LABEL;
                } else {
                    LOG_WRITE(EN_LOG_ERROR, "Invalid response [%X]", (uint32_t)pucRspData[0]);
                    printf("Invalid response [%X]\n", (uint32_t)pucRspData[0]);
                    CDelay::Delay(20, MS);
                    continue;
                }

            }
            if (j >= XMD_RETRY_MAX) {
                /* Send EOT if upgrade failed                                       */
                aucXmdPkg[XMD_EOT_IDX_0] = XMD_EOT;
                aucXmdPkg[XMD_EOT_IDX_1] = XMD_EOT;
                aucXmdPkg[XMD_EOT_IDX_2] = XMD_EOT;
                c_rstpDrvPtr->WriteUnlock(uiIdx, aucXmdPkg, XMD_EOT_IDX_MAX);

                LOG_WRITE(EN_LOG_ERROR, "Send data is error");
                goto Retry_LABEL;;
            }

            /* Update package sequence number                                       */
            ucPackNum++;
        }

        break;

Retry_LABEL:

        LOG_WRITE(EN_LOG_ERROR, "Xmode error. Retry Times = %d", uiRetryTimes);
        CDelay::Delay(20, MS);
        continue;
    }

    if (uiRetryTimes >= 5) {
        LOG_WRITE(EN_LOG_ERROR, "Xmode error. Retry Times = %d", uiRetryTimes);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Xmode End.");
    return true;

}

#else

bool
CFile::XModem(const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr, const std::string &c_rstaStartCmd, uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen)
{

    uint8_t aucXmdPkg[XMD_PKG_LEN] = {0};
    uint8_t aucBuf[XMD_PKG_LEN] = {0};
    uint32_t ulDataLen = 0;
    uint32_t ulPkgCnt = 0;
    uint16_t usCrc16;
    uint8_t ucPackNum = 0;
    uint32_t i = 0, j = 0;
    uint8_t *pucRspData = nullptr;
    std::shared_ptr<CRspHandler> stpRspHandler = nullptr;
    std::shared_ptr<CEndToken> stpEndToken = nullptr;
    std::string staRsp;
    uint32_t uiRetryTimes = 0;

    for ( uiRetryTimes = 0; uiRetryTimes < 5; uiRetryTimes++) {
        LOG_WRITE(EN_LOG_NOTICE, "StartCmd = %s", c_rstaStartCmd.c_str());
        stpRspHandler = std::make_shared<CRspHandler>(1);
        if (!c_rstpDrvPtr->ReadWrite(uiIdx, (uint8_t *)c_rstaStartCmd.c_str(), c_rstaStartCmd.size(), aucBuf, stpRspHandler)) {
            LOG_WRITE(EN_LOG_ERROR, "Receive 'C' from module error.");
            goto Retry_LABEL;
        }

        pucRspData = stpRspHandler->Data();
        if (XMD_CHK_TYPE_CRC_16 != pucRspData[0]) {
            LOG_WRITE(EN_LOG_ERROR, "Receive 'C' from module error.");
            goto Retry_LABEL;;
        }

        /* Start count the package number                                           */
        ucPackNum = 1;
        ulPkgCnt = (ulLen % XMD_PKG_SIZE_1KB) ? ((ulLen / XMD_PKG_SIZE_1KB) + 1) : (ulLen / XMD_PKG_SIZE_1KB);
        for (i = 0; i < ulPkgCnt; i++) {
            memset(aucXmdPkg, 0x00, sizeof(aucXmdPkg));
            memset(aucBuf, 0x00, sizeof(aucBuf));

            /* Creates the Xmodem package header                                    */
            aucXmdPkg[XMD_PKG_IDX_TYPE] = XMD_STX;
            aucXmdPkg[XMD_PKG_IDX_SEQ] = ucPackNum;
            aucXmdPkg[XMD_PKG_IDX_SEQCK] = ~ucPackNum;

            /* Copies the package data                                              */
            if (i < (ulLen / XMD_PKG_SIZE_1KB)) {
                ulDataLen = XMD_PKG_SIZE_1KB;
            } else {
                ulDataLen = (ulLen % XMD_PKG_SIZE_1KB);
            }
            memcpy(&aucXmdPkg[XMD_PKG_IDX_DATA], &pucBuf[i * XMD_PKG_SIZE_1KB], ulDataLen);

            /* Appends the CRC16                                                    */
            usCrc16 = UTIL_Crc16Calc(&aucXmdPkg[XMD_PKG_IDX_DATA], XMD_PKG_SIZE_1KB);
            aucXmdPkg[XMD_PKG_IDX_1KB_CK0] = usCrc16 >> 8;
            aucXmdPkg[XMD_PKG_IDX_1KB_CK1] = usCrc16 & 0xFF;

            /* Sends the Xmodem package to the module, then wait for a response     */
            for (j = 0; j < XMD_RETRY_MAX; j++) {

                LOG_WRITE(EN_LOG_DEBUG, "HwId[%d] ucPackNum = %d.", uiIdx, ucPackNum);
                /* Sends the Xmodem package to the module                           */
                stpRspHandler = std::make_shared<CRspHandler>(1);

                if (!c_rstpDrvPtr->ReadWrite(uiIdx, aucXmdPkg, XMD_PKG_LEN, nullptr, stpRspHandler)) {
                    LOG_WRITE(EN_LOG_ERROR, "HwId[%d] Send Command error.", uiIdx);
                    CDelay::Delay(20, MS);
                    continue;
                }

                pucRspData = (uint8_t *)stpRspHandler->Data();

                if (XMD_ACK == pucRspData[0]) {
                    break;
                } else if (XMD_CAN == pucRspData[0]) {
                    LOG_WRITE(EN_LOG_ERROR, "Response CAN.");
                    goto Retry_LABEL;
                } else {
                    LOG_WRITE(EN_LOG_ERROR, "Invalid response [%X]", (uint32_t)pucRspData[0]);
                    printf("Invalid response [%X]\n", (uint32_t)pucRspData[0]);
                    CDelay::Delay(20, MS);
                    continue;
                }

            }
            if (j >= XMD_RETRY_MAX) {
                /* Send EOT if upgrade failed                                       */
                aucXmdPkg[XMD_EOT_IDX_0] = XMD_EOT;
                aucXmdPkg[XMD_EOT_IDX_1] = XMD_EOT;
                aucXmdPkg[XMD_EOT_IDX_2] = XMD_EOT;
                c_rstpDrvPtr->Write(uiIdx, aucXmdPkg, XMD_EOT_IDX_MAX);

                LOG_WRITE(EN_LOG_ERROR, "Send data is error");
                goto Retry_LABEL;;
            }

            /* Update package sequence number                                       */
            ucPackNum++;
        }

        break;

Retry_LABEL:

        LOG_WRITE(EN_LOG_ERROR, "Xmode error. Retry Times = %d", uiRetryTimes);
        CDelay::Delay(20, MS);
        continue;
    }

    if (uiRetryTimes >= 5) {
        LOG_WRITE(EN_LOG_ERROR, "Xmode error. Retry Times = %d", uiRetryTimes);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "Xmode End.");
    return true;

}


#endif
