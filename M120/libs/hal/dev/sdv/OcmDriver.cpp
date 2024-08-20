/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include "File.h"
#include "OcmDriver.h"
#include "OcmComm.h"

#define OCM_CMD_RETRY_TMS 1

#define ITU_START_SLICE  109
#define ITU_SLICE_WIDTH  8

#define USER_START_SLICE 105
#define USER_SLICE_WIDTH 8


/*==============================function=========================*/

struct timeval m_stStartTime, m_stStopTime;

static void
__StartTime()
{
    gettimeofday(&m_stStartTime, NULL);
}

static bool
__StopTime()
{
    gettimeofday(&m_stStopTime, NULL);

    long elapsed = (m_stStopTime.tv_sec - m_stStartTime.tv_sec) * 1000000 + (m_stStopTime.tv_usec - m_stStartTime.tv_usec);
    elapsed /= 1000;

    LOG_WRITE(EN_LOG_DEBUG, "elapsed time : %ld", elapsed);
    return true;
}


bool
COcmSpiDriver::IsOnline(void)
{

    return true;
}

bool
COcmSpiDriver::GetMfg(CMfg &rstMfg)
{
    uint32_t ulLenR = sizeof(STModuleInfo) + 64;
    LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::GetMfg +++");
    __StartTime();


    memset(&rstMfg, 0, sizeof(rstMfg));

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);

    std::string staRsp(ulLenR, 0);  /* OCM_CMD_MAX_LEN */

    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(),
                                (uint8_t *)staRsp.data(), ulLenR)) {
        LOG_WRITE(EN_LOG_ERROR, "get mfg error. m_uiHwIdx = %u", m_uiHwIdx);

        return false;
    }

    if (false == m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "get mfg rsp error. m_uiHwIdx = %u", m_uiHwIdx);

        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSN = %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acPN = %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acModuleType = %s", rstMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acBootVer = %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSwVer = %s", rstMfg.acSwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFwVer = %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acHwVer = %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acCaliDate = %s", rstMfg.acCaliDate);

    LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::GetMfg ---");


    __StopTime();

    return true;

}

bool
COcmSpiDriver::GetStatusData(COcmStatusData &rstData)
{
#if 0
    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (m_bCommFailDebug) {
        return false;
    }

    CMfg stMfg = {0};
    return GetMfg(stMfg);
#else
    return true;
#endif
}

bool
COcmSpiDriver::GetCfgData(COcmCfgData &rstData)
{


    return true;

}

uint32_t
COcmSpiDriver::GetPortCnt()
{
    return m_uiPortCnt;
}


bool
COcmSpiDriver::GetChPower(uint32_t uiPortIdx, COcmChPower &rstData)
{
    uint32_t ulLenR = (OCM_CH_MAX_NUM * 2) + 64;
    int rc = OPLK_ERR;
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetChPowerCmd(staMsg, uiPortIdx);

    std::string staRsp(ulLenR, 0); /* OCM_CMD_MAX_LEN */
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), ulLenR);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get report data cmd error. port id = %u, m_uiHwIdx = %u", uiPortIdx, m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }


#if 0 /* for debug */
    if (1 == uiPortIdx) {
        COcmChPower stData;
        memset(&stData, 0, sizeof(stData));
        memcpy(&stData, (uint8_t *)staRsp.data() + 3, 192);

        LOG_WRITE(EN_LOG_NOTICE, "----------------------------------------");

#if 1
        LOG_WRITE(EN_LOG_NOTICE, "uiPortIdx: %d %d %d %d %d %d",
                  uiPortIdx,
                  stData.asPower[43],
                  stData.asPower[44],
                  stData.asPower[45],
                  stData.asPower[46],
                  stData.asPower[47]);
#endif

#if 0
        if (0 == stData.asPower[44]) {
            for (int i = 0; i < 200; ++i) {
                printf("0x%02x ", *((uint8_t *)staRsp.data() + i));
                if ((i != 0) && ((i % 16) == 0)) {
                    printf("\r\n");
                }
            }
            printf("\r\n");
        }
#endif

#if 0

        LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  stData.asPower[0],
                  stData.asPower[1],
                  stData.asPower[2],
                  stData.asPower[3],
                  stData.asPower[4],
                  stData.asPower[5],
                  stData.asPower[6],
                  stData.asPower[7]);

        LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  stData.asPower[8],
                  stData.asPower[9],
                  stData.asPower[10],
                  stData.asPower[11],
                  stData.asPower[12],
                  stData.asPower[13],
                  stData.asPower[14],
                  stData.asPower[15]);


        LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  stData.asPower[50],
                  stData.asPower[51],
                  stData.asPower[52],
                  stData.asPower[53],
                  stData.asPower[54],
                  stData.asPower[55],
                  stData.asPower[56],
                  stData.asPower[57]);

        LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  stData.asPower[58],
                  stData.asPower[59],
                  stData.asPower[60],
                  stData.asPower[61],
                  stData.asPower[62],
                  stData.asPower[63],
                  stData.asPower[64],
                  stData.asPower[65]);
#endif
    }
#endif

    if (false == m_stpProtoPtr->GetChPowerRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get channel power rsp error.");
        return false;
    }

#if 0
/*debug for ocm*/
    if (1 == uiPortIdx) {

        /*LOG_WRITE(EN_LOG_NOTICE, "----------------------------------------");*/


#if 1
        /*LOG_WRITE(EN_LOG_NOTICE, "uiPortIdx: %d %d %d %d %d %d",
                  uiPortIdx,
                  rstData.asPower[43],
                  rstData.asPower[44],
                  rstData.asPower[45],
                  rstData.asPower[46],
                  rstData.asPower[47]);*/
        printf("%s(%d):%s  uiPortIdx: %d %d %d %d %d\n", __FILE__, __LINE__, __func__, rstData.asPower[43],
               rstData.asPower[44],
               rstData.asPower[45],
               rstData.asPower[46],
               rstData.asPower[47]);
#endif



#if 1
        if (0 == rstData.asPower[44]) {
            for (int i = 0; i < 200; ++i) {
                printf("0x%02x ", *((uint8_t *)staRsp.data() + i));
                if ((i != 0) && ((i % 16) == 0)) {
                    printf("\r\n");
                }
            }
            printf("\r\n");
        }
#endif

#if 1

        /*LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  rstData.asPower[0],
                  rstData.asPower[1],
                  rstData.asPower[2],
                  rstData.asPower[3],
                  rstData.asPower[4],
                  rstData.asPower[5],
                  rstData.asPower[6],
                  rstData.asPower[7]);*/
        printf("%s(%d):%s  %d %d %d %d %d %d %d %d\n", __FILE__, __LINE__, __func__, rstData.asPower[0],
               rstData.asPower[1],
               rstData.asPower[2],
               rstData.asPower[3],
               rstData.asPower[4],
               rstData.asPower[5],
               rstData.asPower[6],
               rstData.asPower[7]);

        /*LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  rstData.asPower[8],
                  rstData.asPower[9],
                  rstData.asPower[10],
                  rstData.asPower[11],
                  rstData.asPower[12],
                  rstData.asPower[13],
                  rstData.asPower[14],
                  rstData.asPower[15]);*/
        printf("%s(%d):%s  %d %d %d %d %d %d %d %d\n", __FILE__, __LINE__, __func__, rstData.asPower[8],
               rstData.asPower[9],
               rstData.asPower[10],
               rstData.asPower[11],
               rstData.asPower[12],
               rstData.asPower[13],
               rstData.asPower[14],
               rstData.asPower[15]);

        /*LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  rstData.asPower[50],
                  rstData.asPower[51],
                  rstData.asPower[52],
                  rstData.asPower[53],
                  rstData.asPower[54],
                  rstData.asPower[55],
                  rstData.asPower[56],
                  rstData.asPower[57]);*/
        printf("%s(%d):%s  %d %d %d %d %d %d %d %d\n", __FILE__, __LINE__, __func__, rstData.asPower[50],
               rstData.asPower[51],
               rstData.asPower[52],
               rstData.asPower[53],
               rstData.asPower[54],
               rstData.asPower[55],
               rstData.asPower[56],
               rstData.asPower[57]);

        /*LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %d %d %d %d %d",
                  rstData.asPower[58],
                  rstData.asPower[59],
                  rstData.asPower[60],
                  rstData.asPower[61],
                  rstData.asPower[62],
                  rstData.asPower[63],
                  rstData.asPower[64],
                  rstData.asPower[65]);*/
        printf("%s(%d):%s  %d %d %d %d %d %d %d %d\n", __FILE__, __LINE__, __func__, rstData.asPower[58],
               rstData.asPower[59],
               rstData.asPower[60],
               rstData.asPower[61],
               rstData.asPower[62],
               rstData.asPower[63],
               rstData.asPower[64],
               rstData.asPower[65]);
#endif
    }
#endif


    LOG_WRITE(EN_LOG_DEBUG, "GetChPower -- %d", uiPortIdx);

    return true;
}

bool
COcmSpiDriver::GetRealTimeOsa(uint32_t uiPortIdx, COcmOsa &rstData)
{
    bool bTryAgain = false;
    uint16_t usLen = 0;
    uint16_t usFrameLen = 0;
    uint8_t ucCheckSum = 0;
    uint8_t *pucBufR = nullptr;
    uint32_t uiRetryTimes = 0;

    /* for debug */
    /* return true; */

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetChRawDataCmd(staMsg, uiPortIdx);

    std::string staRsp(OCM_CMD_MAX_LEN, 0);

#if 0
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get report data cmd error. port id = %d, m_uiHwIdx = %d", uiPortIdx, m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }
#endif

    for (uiRetryTimes = 0; uiRetryTimes < 5; uiRetryTimes++) {
        /* Write frame cmd into ocm                                                                                                                                                                             */
        if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN)) {
            LOG_WRITE(EN_LOG_ERROR, "Get Real Time Osa error.");
            return false;
        }

        /* Check the busy state                                                                                                                                                                                         */
        if (false == m_stpProtoPtr->CheckState((uint8_t *)staRsp.data(), bTryAgain)) {
            if (bTryAgain) {
                CDelay::Delay(200, MS);
                LOG_WRITE(EN_LOG_DEBUG, "Ocm ReadWrite Try Again. uiRetryTimes = %d", uiRetryTimes);
                continue;
            } else {
                LOG_WRITE(EN_LOG_ERROR, "Get Real Time Osa error.");
                return false;
            }
        }

        break;
    }

    if (uiRetryTimes >= 5) {
        LOG_WRITE(EN_LOG_ERROR, "Get Real Time Osa error. Retry Times = %d.", uiRetryTimes);
        return false;
    }

    pucBufR = (uint8_t *)staRsp.data();
    usLen = ((0x00FF & pucBufR[2]) << 8) | pucBufR[1];
    usFrameLen = usLen + 4;

    /* Frame length must be less then the input buffer length				*/
    if ((usLen >= OCM_CMD_MAX_LEN) || (usFrameLen > OCM_CMD_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "ocm response length %d > buffer %d", usFrameLen, OCM_CMD_MAX_LEN);
        return false;
    }

    /*checksum*/
    ucCheckSum = m_stpProtoPtr->CheckSum(pucBufR, usFrameLen - 1);
    if (ucCheckSum != pucBufR[usFrameLen - 1]) {
        LOG_WRITE(EN_LOG_ERROR, "ocm response check sum error! current is:0x%02X, expect is: 0x%02X",
                  ucCheckSum, pucBufR[usFrameLen - 1]);
        return false;
    }


    if (false == m_stpProtoPtr->GetChRawDataRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get channel power rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "GetRealTimeOsa -- %d", uiPortIdx);

    return true;
}

bool
COcmSpiDriver::GetOsa(uint32_t uiPortIdx, COcmOsa &rstData)
{
    int rc = OPLK_ERR;
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }


    LOG_WRITE(EN_LOG_DEBUG, "GetOsa +++ %d", uiPortIdx);

    std::string staMsg;
    m_stpProtoPtr->GetChRawDataCmd(staMsg, uiPortIdx);

    std::string staRsp(OCM_CMD_MAX_LEN, 0);
    rc = __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN);
    if (OPLK_ERR == rc) {
        LOG_WRITE(EN_LOG_ERROR, "get report data cmd error. port id = %d, m_uiHwIdx = %d", uiPortIdx, m_uiHwIdx);
        return false;
    } else if (ERR_CMD_DELAY == rc) {
        return false;
    }


    if (false == m_stpProtoPtr->GetChRawDataRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get channel power rsp error.");
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "GetOsa --- %d", uiPortIdx);

    return true;
}


bool
COcmSpiDriver::SetWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
{
#if 0
    int i = 0;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan ++ %d %d %d", uiPortIdx, m_uiHwIdx, uiChCnt);
        return false;
    }

#if 0
    printf("COcmSpiDriver::SetWavePlan uiChCnt = %d:\r\n", uiChCnt);
    for (int i = 0; i < sizeof(CWavePlan) * uiChCnt; ++i) {
        printf("%02x ", *((uint8_t *)pstWavePlan + i));
        if (((i + 1) % 30) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif

    std::string staMsg;
    m_stpProtoPtr->SetWavePlanCmd(staMsg, uiPortIdx, uiChCnt, pstWavePlan);

    std::string staRsp(OCM_CMD_MAX_LEN, 0);

#if 0
    printf("COcmSpiDriver::SetWavePlan uiChCnt = %d:\r\n", uiChCnt);
    for (int i = 0; i < sizeof(CWavePlan) * uiChCnt; ++i) {
        printf("%02x ", *((uint8_t *)pstWavePlan + i));
        if (((i + 1) % 30) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif

    for (i = 0; i < 3; i++) {
        if (OPLK_OK == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN)) {
            break;
        }
    }

    if (i >= 3) {
        LOG_WRITE(EN_LOG_ERROR, "set wave plan set cmd error. port id = %d base addr = 0X%02x uiChCnt= %d", uiPortIdx, m_uiHwIdx, uiChCnt);
        return false;
    }

    if (uiPortIdx > m_uiPortCnt) {
        m_uiPortCnt = uiPortIdx;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan -- %d", uiPortIdx);
#endif
    return true;
}

bool
COcmSpiDriver::SetOcmWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
{
    int i = 0;

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan ++ %d %d %d", uiPortIdx, m_uiHwIdx, uiChCnt);
        return false;
    }

#if 0
    printf("COcmSpiDriver::SetWavePlan uiChCnt = %d:\r\n", uiChCnt);
    for (int i = 0; i < sizeof(WAVEPLAN_ST) * uiChCnt; ++i) {
        printf("%02x ", *((uint8_t *)pstWavePlan + i));
        if (((i + 1) % 30) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif

    std::string staMsg;
    m_stpProtoPtr->SetOcmWavePlanCmd(staMsg, uiPortIdx, uiChCnt, pstWavePlan);

    std::string staRsp(OCM_CMD_MIN_LEN, 0);    /* OCM_CMD_MAX_LEN */

#if 0
    printf("COcmSpiDriver::SetWavePlan uiChCnt = %d:\r\n", uiChCnt);
    for (int i = 0; i < sizeof(CWavePlan) * uiChCnt; ++i) {
        printf("%02x ", *((uint8_t *)pstWavePlan + i));
        if (((i + 1) % 30) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif

    for (i = 0; i < 3; i++) {
        if (OPLK_OK == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MIN_LEN)) {
            break;
        }
    }

    if (i >= 3) {

        LOG_WRITE(EN_LOG_ERROR, "set wave plan set cmd error. port id = %d base addr = 0X%02x uiChCnt= %d", uiPortIdx, m_uiHwIdx, uiChCnt);
#if 0
        STChannelGrid stWavePlan;
        memset(&stWavePlan, 0, sizeof(stWavePlan));
        m_stpProtoPtr->GetWaveplanCmd(staMsg, uiPortIdx);
        __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN);
        m_stpProtoPtr->GetWaveplanRsp(staRsp, &stWavePlan);
#endif
        return false;
    }

    if (uiPortIdx > m_uiPortCnt) {
        m_uiPortCnt = uiPortIdx;
    }

    LOG_WRITE(EN_LOG_NOTICE, "SetWavePlan -- %d", uiPortIdx);


#if 0              /* for debug use */
    memset(&stWavePlan, 0, sizeof(stWavePlan));
    m_stpProtoPtr->GetWaveplanCmd(staMsg, uiPortIdx);
    __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN);
    m_stpProtoPtr->GetWaveplanRsp(staRsp, stWavePlan);
#endif
    return true;
}

bool
COcmSpiDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "COcmSpiDriver::Reset ++ %d %d", uiSubIdx, ucResetType);

    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        if (nullptr != m_stpResetBoardPtr) {
            m_stpResetBoardPtr->Reset(m_uiResetHwSubIdx);
        }
    } else {
        std::string staMsg;
        m_stpProtoPtr->SoftReset(staMsg, uiSubIdx);

        std::string staRsp(OCM_CMD_MAX_LEN, 0);
        if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN)) {
            LOG_WRITE(EN_LOG_ERROR, "set soft reset set cmd error. uiSubIdx = %d base addr = 0X%02x", uiSubIdx, m_uiHwIdx);
            return false;
        }
    }

    LOG_WRITE(EN_LOG_TRACE, "COaIoDriver::Reset --");

    return true;
}

bool
COcmSpiDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;
    uint8_t *pucDataPos = nullptr;
    uint32_t ulWriteLen = 0;
    uint32_t ulLeaveLen = 0;
    uint32_t ulCrc = 0;

    std::string staMsg;
    std::string staRsp(OCM_CMD_MAX_LEN, 0);

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr) || (nullptr == pcFilePath)) {
        goto ERR_LABEL;
    }

    pucBuffer = (uint8_t *)CFile::Malloc(pcFilePath, iFileLen);
    if (nullptr == pucBuffer) {
        LOG_WRITE(EN_LOG_ERROR, "File(%s) malloc buffer error. iFileLen = %d", pcFilePath, iFileLen);
        goto ERR_LABEL;
    }

    /* start upg command */
    ulCrc = UTIL_Crc32Calc(pucBuffer, iFileLen);
    m_stpProtoPtr->StartUpg(staMsg, iFileLen, ulCrc);

    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade start cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }


    /* send data */
    pucDataPos = pucBuffer;
    ulLeaveLen = iFileLen;
    ulWriteLen = 0;
    while (ulLeaveLen > 0) {
        if (ulLeaveLen > DEF_OCM_UPGD_PKG_LEN_MAX) {
            ulWriteLen = DEF_OCM_UPGD_PKG_LEN_MAX;
        } else {
            ulWriteLen = ulLeaveLen;
        }

        m_stpProtoPtr->SendData(staMsg, pucDataPos, ulWriteLen);
        if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
            LOG_WRITE(EN_LOG_ERROR, "upgrade send data cmd error. base addr = 0X%02x", m_uiHwIdx);
            goto ERR_LABEL;
        }

        ulLeaveLen -= ulWriteLen;
        pucDataPos += ulWriteLen;
    }

    LOG_WRITE(EN_LOG_NOTICE, "end command ulLeaveLen %d ulWriteLen %d", ulLeaveLen, ulWriteLen);

    CDelay::Delay(5, MS);

    m_stpProtoPtr->Verify(staMsg);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade verify cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }

    m_stpProtoPtr->Switch(staMsg);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade switch cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }
#if  0
    m_stpProtoPtr->SoftReset(staMsg, 0);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), 32)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade switch cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }

    /* end upg command */
    m_stpProtoPtr->EndUpg(staMsg);
    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(), (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "upgrade end cmd error. base addr = 0X%02x", m_uiHwIdx);
        goto ERR_LABEL;
    }
#endif

    CDelay::Delay(3, S);

    LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COcmSpiDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}

bool
COcmSpiDriver::Commit(uint32_t uiDevIdx)
{
    LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::Commit +++");
    __StartTime();

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->EndUpg(staMsg);

    std::string staRsp(OCM_CMD_MAX_LEN, 0);

    if (OPLK_ERR == __ReadWrite(m_uiHwIdx, (uint8_t *)staMsg.data(), staMsg.size(),
                                (uint8_t *)staRsp.data(), OCM_CMD_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "Commit error. m_uiHwIdx = %u", m_uiHwIdx);

        return false;
    }

    __StopTime();

    return true;

}


int
COcmSpiDriver::__ReadWrite(uint32_t uiChip, uint8_t *pucBufW, int32_t uiLenW, uint8_t *pucBufR, uint32_t ulLenR)
{
    bool bTryAgain = false;
    uint16_t usLen = 0;
    uint16_t usFrameLen = 0;
    uint8_t ucCheckSum = 0;

    if (nullptr == pucBufW || nullptr == pucBufR) {
        LOG_WRITE(EN_LOG_ERROR, "pucBufW is nullptr or pucBufR is nullptr.");
        goto ERR_LABEL;
    }

    /* Write frame cmd into ocm                                                                                         */
    if (false == m_stpDrvPtr->ReadWrite(m_uiHwIdx, pucBufW, uiLenW, pucBufR, std::make_shared<CRspHandler>(ulLenR))) {
        goto ERR_LABEL;
    }

    /* Check the busy state                                                                                             */
    if (false == m_stpProtoPtr->CheckState(pucBufR, bTryAgain)) {
        if (bTryAgain) {
            CDelay::Delay(200, MS);
            LOG_WRITE(EN_LOG_DEBUG, "Ocm ReadWrite Try Again.");
            if (false == m_stpDrvPtr->ReadWrite(m_uiHwIdx, pucBufW, uiLenW, pucBufR, std::make_shared<CRspHandler>(ulLenR))) {
                goto ERR_LABEL;
            } else {
                if (false == m_stpProtoPtr->CheckState(pucBufR, bTryAgain)) {
                    if (bTryAgain) {
                        /* Next loop try again */
                        return ERR_CMD_DELAY;
                    } else {
                        goto ERR_LABEL;
                    }
                }
            }
        } else {
            goto ERR_LABEL;
        }
    }

    usLen = ((0x00FF & pucBufR[2]) << 8) | pucBufR[1];
    usFrameLen = usLen + 4;

    /* Frame length must be less then the input buffer length               */
    if ((usLen >= OCM_CMD_MAX_LEN) || (usFrameLen > OCM_CMD_MAX_LEN)) {
        LOG_WRITE(EN_LOG_ERROR, "ocm response length %d > buffer %d", usFrameLen, OCM_CMD_MAX_LEN);
        return OPLK_ERR;
    }

    /*checksum*/
    ucCheckSum = m_stpProtoPtr->CheckSum(pucBufR, usFrameLen - 1);
    if (ucCheckSum != pucBufR[usFrameLen - 1]) {
        LOG_WRITE(EN_LOG_ERROR, "ocm response check sum error! current is:0x%02X, expect is: 0x%02X",
                  ucCheckSum, pucBufR[usFrameLen - 1]);
        return OPLK_ERR;
    }

    return OPLK_OK;


ERR_LABEL:

    LOG_WRITE(EN_LOG_ERROR, "send command to ocm failed! m_uiHwIdx = %d", m_uiHwIdx);

    return OPLK_ERR;

}

