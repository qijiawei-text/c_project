/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OcmMasterSpiDriver.cpp
   Author:
   Date:
   Version:       1.0
   Description:
   Function List:

   History:



*****************************************************************************************************/

/*=========================include head files====================*/
#include "File.h"
#include "OcmMasterSpiDriver.h"
#define OCM_CMD_RETRY_TMS 1

#define ITU_START_SLICE  109
#define ITU_SLICE_WIDTH  8

#define USER_START_SLICE 105
#define USER_SLICE_WIDTH 8


/*==============================function=========================*/

static struct timeval m_stStartTime, m_stStopTime;
std::mutex COcmMasterSpiDriver::ms_stMutex;

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
COcmMasterSpiDriver::IsOnline(void)
{

    return true;
}

bool
COcmMasterSpiDriver::GetMfg(CMfg &rstMfg)
{
    /* LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::GetMfg +++"); */
    __StartTime();

    /* printf("%s ++\n", __func__); */

    memset(&rstMfg, 0, sizeof(rstMfg));

    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetMfgCmd(staMsg);
    /*std::cout << staMsg << std::endl; */

    memset(&rstMfg, 0, sizeof(rstMfg));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(512);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetMfg error.");
        return false;
    }
    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (false == m_stpProtoPtr->GetMfgRsp(staRsp, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "get mfg rsp error. m_uiHwIdx = %d", m_uiHwIdx);
        return false;
    }

    /* printf("acSwVer = %s\n", rstMfg.acSwVer); */

    /*LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSN = %s", rstMfg.acSN);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acPN = %s", rstMfg.acPN);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acModuleType = %s", rstMfg.acModuleType);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acBootVer = %s", rstMfg.acBootVer);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acSwVer = %s", rstMfg.acSwVer);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acFwVer = %s", rstMfg.acFwVer);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acHwVer = %s", rstMfg.acHwVer);
       LOG_WRITE(EN_LOG_NOTICE, "rstMfg.acCaliDate = %s", rstMfg.acCaliDate);*/

    /* LOG_WRITE(EN_LOG_NOTICE, "COcmSpiDriver::GetMfg ---"); */


    /* printf("%s --\n", __func__); */

    __StopTime();

    return true;

}

bool
COcmMasterSpiDriver::GetStatusData(COcmStatusData &rstData)
{
    if ((nullptr == m_stpResetBoardPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (m_bCommFailDebug) {
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COcmMasterSpiDriver GetStatusData");
    CMfg stMfg = {0};
    return GetMfg(stMfg);
}


bool
COcmMasterSpiDriver::GetCfgData(COcmCfgData &rstData)
{


    return true;

}

uint32_t
COcmMasterSpiDriver::GetPortCnt()
{
    return m_uiPortCnt;
}


bool
COcmMasterSpiDriver::GetChPower(uint32_t uiPortIdx, COcmChPower &rstData)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetChPowerCmd(staMsg, uiPortIdx);

    memset(&rstData, 0, sizeof(rstData));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(OCM_CMD_MAX_LEN);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetChPower error.");
        return false;
    }
    std::string staRsp;
    stpRspHandler->Copy(staRsp);

#if 0 /* for debug */
    if (1 == uiPortIdx) {
        COcmChPower stData;
        memset(&stData, 0, sizeof(stData));
        memcpy(&stData, (uint8_t *)staRsp.data() + 3, 192);

        LOG_WRITE(EN_LOG_DEBUG, "----------------------------------------");

#if 1
        LOG_WRITE(EN_LOG_DEBUG, "uiPortIdx: %d %d %d %d %d %d",
                  uiPortIdx,
                  stData.asPower[43],
                  stData.asPower[44],
                  stData.asPower[45],
                  stData.asPower[46],
                  stData.asPower[47]);
#endif

#if 1
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

#if 1

        LOG_WRITE(EN_LOG_DEBUG, "%d %d %d %d %d %d %d %d",
                  stData.asPower[0],
                  stData.asPower[1],
                  stData.asPower[2],
                  stData.asPower[3],
                  stData.asPower[4],
                  stData.asPower[5],
                  stData.asPower[6],
                  stData.asPower[7]);

        LOG_WRITE(EN_LOG_DEBUG, "%d %d %d %d %d %d %d %d",
                  stData.asPower[8],
                  stData.asPower[9],
                  stData.asPower[10],
                  stData.asPower[11],
                  stData.asPower[12],
                  stData.asPower[13],
                  stData.asPower[14],
                  stData.asPower[15]);


        LOG_WRITE(EN_LOG_DEBUG, "%d %d %d %d %d %d %d %d",
                  stData.asPower[50],
                  stData.asPower[51],
                  stData.asPower[52],
                  stData.asPower[53],
                  stData.asPower[54],
                  stData.asPower[55],
                  stData.asPower[56],
                  stData.asPower[57]);

        LOG_WRITE(EN_LOG_DEBUG, "%d %d %d %d %d %d %d %d",
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

    /* LOG_WRITE(EN_LOG_NOTICE, "GetChPower -- %d", uiPortIdx); */

    return true;
}

bool
COcmMasterSpiDriver::GetOsa(uint32_t uiPortIdx, COcmOsa &rstData)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "GetOsa +++ %d", uiPortIdx); */

    std::string staMsg;
    m_stpProtoPtr->GetChRawDataCmd(staMsg, uiPortIdx);

    memset(&rstData, 0, sizeof(rstData));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(OCM_CMD_MAX_LEN);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetChPower error.");
        return false;
    }
    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (false == m_stpProtoPtr->GetChRawDataRsp(staRsp, rstData)) {
        LOG_WRITE(EN_LOG_ERROR, "get channel power rsp error.");
        return false;
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "GetOsa --- %d", uiPortIdx); */

    return true;
}

bool
COcmMasterSpiDriver::GetRealTimeOsa(uint32_t uiPortIdx, COcmOsa &rstData)
{
    return true;
}

bool
COcmMasterSpiDriver::SetWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, CWavePlanSlice *pstWavePlan)
{
    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_DEBUG, "SetWavePlan ++ %d %d %d", uiPortIdx, m_uiHwIdx, uiChCnt);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COcmSpiDriver::SetWavePlan uiPortIdx:%d, uiChCnt = %d", uiPortIdx, uiChCnt);
    printf("COcmSpiDriver::SetWavePlan uiPortIdx:%d, uiChCnt = %d:\r\n", uiPortIdx, uiChCnt);
    for (uint32_t i = 0; i < sizeof(CWavePlan) * uiChCnt; ++i) {
        printf("%02x ", *((uint8_t *)pstWavePlan + i));
        if (((i + 1) % 30) == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    std::string staMsg;
    m_stpProtoPtr->SetWavePlanCmd(staMsg, uiPortIdx, uiChCnt, pstWavePlan);

    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(0 + 1);
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "COcmMasterSpiDriver SetWavePlan error.");
        return false;
    }
    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (uiPortIdx > m_uiPortCnt) {
        m_uiPortCnt = uiPortIdx;
    }

    LOG_WRITE(EN_LOG_DEBUG, "SetWavePlan -- %d", uiPortIdx);

    return true;
}

bool
COcmMasterSpiDriver::SetOcmWavePlan(uint32_t uiPortIdx, uint32_t uiChCnt, STChannelGrid *pstWavePlan)
{
    return true;
}

bool
COcmMasterSpiDriver::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
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

        std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(OCM_CMD_MAX_LEN);
        if (!__SendCmd(staMsg, stpRspHandler)) {
            LOG_WRITE(EN_LOG_ERROR, "COcmMasterSpiDriver Reset error.");
            return false;
        }
        std::string staRsp;
        stpRspHandler->Copy(staRsp);

    }

    LOG_WRITE(EN_LOG_TRACE, "COaIoDriver::Reset --");

    return true;
}

bool
COcmMasterSpiDriver::Upgrade(uint32_t uiDevIdx, const char *pcFilePath)
{
    LOG_WRITE(EN_LOG_DEBUG, "COcmMasterSpiDriver::Upgrade ++ %d %s", uiDevIdx, pcFilePath);

    int iFileLen = 0;
    uint8_t *pucBuffer = NULL;
    uint8_t *pucDataPos = nullptr;
    uint32_t ulWriteLen = 0;
    uint32_t ulLeaveLen = 0;
    uint32_t ulCrc = 0;

    std::string staMsg;

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

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "COcmMasterSpiDriver SendDataCmd1 error.");
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

        if (!__SendSetCmd(staMsg)) {
            LOG_WRITE(EN_LOG_ERROR, "COcmMasterSpiDriver SendDataCmd2 error.");
            goto ERR_LABEL;
        }

        ulLeaveLen -= ulWriteLen;
        pucDataPos += ulWriteLen;
    }

    LOG_WRITE(EN_LOG_DEBUG, "end command ulLeaveLen %d ulWriteLen %d", ulLeaveLen, ulWriteLen);

    CDelay::Delay(5, MS);

    /* end upg command */
    m_stpProtoPtr->EndUpg(staMsg);

    if (!__SendSetCmd(staMsg)) {
        LOG_WRITE(EN_LOG_ERROR, "COcmMasterSpiDriver SendDataCmd3 error.");
        goto ERR_LABEL;
    }

    CDelay::Delay(3, S);

    LOG_WRITE(EN_LOG_DEBUG, "COcmMasterSpiDriver::Upgrade ---");

    DEF_SAFE_FREE(pucBuffer);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "COcmMasterSpiDriver::Upgrade error ---");

    DEF_SAFE_FREE(pucBuffer);
    return false;

}

bool
COcmMasterSpiDriver::Commit(uint32_t uiDevIdx)
{
    return true;
}

bool
COcmMasterSpiDriver::GetTemp(uint32_t uiPortIdx, int16_t &rsData)
{
    LOG_WRITE(EN_LOG_DEBUG, "COcmSpiDriver::GetTemp +++");
    __StartTime();


    if ((nullptr == m_stpDrvPtr) || (nullptr == m_stpProtoPtr)) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr or m_stpProtoPtr is null");
        return false;
    }

    std::string staMsg;
    m_stpProtoPtr->GetTempCmd(staMsg, uiPortIdx);
    /*std::cout << staMsg << std::endl; */

    int16_t stemp = 0;
    memset(&stemp, 0, sizeof(stemp));
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(2 + 1);  /* <= 512 Bytes  qingy */
    if (!__SendCmd(staMsg, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "GetTemp error.");
        return false;
    }
    std::string staRsp;
    stpRspHandler->Copy(staRsp);

    if (false == m_stpProtoPtr->GetTempRsp(staRsp, stemp)) {
        LOG_WRITE(EN_LOG_ERROR, "get temp rsp error. m_uiHwIdx = %d", m_uiHwIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "COcmSpiDriver::GetTemp ---");
    rsData = stemp;

    __StopTime();
    sleep(1);
    return true;

}


int
COcmMasterSpiDriver::Switch(uint32_t uiChip)
{
    uint8_t ucDataW = 0;
    uint8_t ucDataSubW = 0;
    uint32_t uiOffset = 0;

    m_stpLogicDrvPtr->Read(0x44, &ucDataW, 1);
    ucDataW &= ~(1 << uiChip);
    m_stpLogicDrvPtr->Write(0x44, &ucDataW, 1);
    usleep(10 * 1000);

#if 1
    ucDataSubW = 0;
    uiOffset = uiChip * 0x100;
    m_stpLogicDrvPtr->Read(0x3150 + uiOffset, &ucDataSubW, 1);
    ucDataSubW &= 0xcf;                                           /*bit4 bit5 00  11001111 */
    m_stpLogicDrvPtr->Write(0x3150 + uiOffset, &ucDataSubW, 1);
#endif

    usleep(10 * 1000);
    ucDataW |= 1 << uiChip;
    m_stpLogicDrvPtr->Write(0x44, &ucDataW, 1);

    return OPLK_OK;
}

std::shared_ptr<CRspHandler>
COcmMasterSpiDriver::__RspHandler(uint32_t uiLen, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = std::make_shared<CRspHandler>(sizeof(ST_FRM_READ_HEAD) + uiLen + DEF_FRM_CRC_LEN);
    stpRspHandler->SetProto(m_stpProtoPtr);
    stpRspHandler->SetRetryTimes(uiTimeOut);

    return stpRspHandler;
}

bool
COcmMasterSpiDriver::__SendCmd(std::string staMsg, std::shared_ptr<CRspHandler> stpRspHandler)
{
    /* todo  get hwidx */
    /* GetMutex().lock(); */
    pthread_mutex_lock(&m_spi_mutex[m_uiHwSubIdx]);
    /* Switch(m_uiHwSubIdx); */

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr.");

        /* GetMutex().unlock(); */
        pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]);
        return false;
    }
    if (!m_stpDrvPtr->ReadWrite(m_uiHwSubIdx, (uint8_t *)staMsg.data(), staMsg.size(), nullptr, stpRspHandler)) {
        LOG_WRITE(EN_LOG_ERROR, "ReadWrite error: %u.", m_uiHwSubIdx);
        /* GetMutex().unlock(); */
        pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]);
        return false;
    }
    /* GetMutex().unlock(); */
    pthread_mutex_unlock(&m_spi_mutex[m_uiHwSubIdx]);

    return true;
}

bool
COcmMasterSpiDriver::__SendSetCmd(std::string staMsg, uint32_t uiTimeOut)
{
    std::shared_ptr<CRspHandler> stpRspHandler = __RspHandler(0, uiTimeOut);

    return __SendCmd(staMsg, stpRspHandler);
}
