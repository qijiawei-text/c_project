/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      ShmOperator.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "ShareMemory.h"
#include "Queue.h"
#include "Semaphore.h"
#include "DevData.h"
#include <cstddef>

/*************************************************/

enum EShmType
{
    SHM_TYPE_STA = 0,
    SHM_TYPE_CMD,
    SHM_TYPE_DATA,

    SHM_TYPE_MAX

};

enum EShmKey
{
    SHM_KEY_HAL_STA = 0x0F000000,
    SHM_KEY_HAL_CMD,
    SHM_KEY_HAL_DATA,

    SHM_KEY_SLOT_STA = 0x0F000001,
    SHM_KEY_SLOT_CMD,
    SHM_KEY_SLOT_DATA,

    SHM_KEY_BOARD_STA = 0x0F000101,
    SHM_KEY_BOARD_CMD,
    SHM_KEY_BOARD_DATA,

    SHM_KEY_OA_STA = 0x0F000201,
    SHM_KEY_OA_CMD,
    SHM_KEY_OA_DATA,

    SHM_KEY_OCM_STA = 0x0F000301,
    SHM_KEY_OCM_CMD,
    SHM_KEY_OCM_DATA,

    SHM_KEY_WSS_STA = 0x0F000401,
    SHM_KEY_WSS_CMD,
    SHM_KEY_WSS_DATA,

    SHM_KEY_TDCM_STA = 0x0F000501,
    SHM_KEY_TDCM_CMD,
    SHM_KEY_TDCM_DATA,

    SHM_KEY_PAM4_STA = 0x0F000601,
    SHM_KEY_PAM4_CMD,
    SHM_KEY_PAM4_DATA,

    SHM_KEY_OPS_STA = 0x0F000701,
    SHM_KEY_OPS_CMD,
    SHM_KEY_OPS_DATA,

    SHM_KEY_DCE_STA = 0x0F000801,
    SHM_KEY_DCE_CMD,
    SHM_KEY_DCE_DATA,

    SHM_KEY_OTDR_STA = 0x0F000901,
    SHM_KEY_OTDR_CMD,
    SHM_KEY_OTDR_DATA,

    SHM_KEY_DGE_STA = 0x0F000A01,
    SHM_KEY_DGE_CMD,
    SHM_KEY_DGE_DATA,

    SHM_KEY_MCS_STA = 0x0F000B01,
    SHM_KEY_MCS_CMD,
    SHM_KEY_MCS_DATA,

    SHM_KEY_OSC_STA = 0x0F000C01,
    SHM_KEY_OSC_CMD,
    SHM_KEY_OSC_DATA,

    SHM_KEY_MUX_STA = 0x0F000D01,
    SHM_KEY_MUX_CMD,
    SHM_KEY_MUX_DATA,

    SHM_KEY_EXTEND_STA = 0x0F000E01,
    SHM_KEY_EXTEND_CMD,
    SHM_KEY_EXTEND_DATA,

    SHM_KEY_OCS_STA = 0x0F000F01,
    SHM_KEY_OCS_CMD,
    SHM_KEY_OCS_DATA

};

enum EShmSemKey
{
    SHM_SEM_KEY_HAL_STA = 0x0E000000,
    SHM_SEM_KEY_HAL_CMD,
    SHM_SEM_KEY_HAL_DATA,

    SHM_SEM_KEY_SLOT_STA = 0x0E000001,
    SHM_SEM_KEY_SLOT_CMD,
    SHM_SEM_KEY_SLOT_DATA,

    SHM_SEM_KEY_BOARD_STA = 0x0E000101,
    SHM_SEM_KEY_BOARD_CMD,
    SHM_SEM_KEY_BOARD_DATA,

    SHM_SEM_KEY_OA_STA = 0x0E000201,
    SHM_SEM_KEY_OA_CMD,
    SHM_SEM_KEY_OA_DATA,

    SHM_SEM_KEY_OCM_STA = 0x0E000301,
    SHM_SEM_KEY_OCM_CMD,
    SHM_SEM_KEY_OCM_DATA,

    SHM_SEM_KEY_WSS_STA = 0x0E000401,
    SHM_SEM_KEY_WSS_CMD,
    SHM_SEM_KEY_WSS_DATA,

    SHM_SEM_KEY_TDCM_STA = 0x0E000501,
    SHM_SEM_KEY_TDCM_CMD,
    SHM_SEM_KEY_TDCM_DATA,

    SHM_SEM_KEY_PAM4_STA = 0x0E000601,
    SHM_SEM_KEY_PAM4_CMD,
    SHM_SEM_KEY_PAM4_DATA,

    SHM_SEM_KEY_OPS_STA = 0x0E000701,
    SHM_SEM_KEY_OPS_CMD,
    SHM_SEM_KEY_OPS_DATA,

    SHM_SEM_KEY_DCE_STA = 0x0E000801,
    SHM_SEM_KEY_DCE_CMD,
    SHM_SEM_KEY_DCE_DATA,

    SHM_SEM_KEY_OTDR_STA = 0x0E000901,
    SHM_SEM_KEY_OTDR_CMD,
    SHM_SEM_KEY_OTDR_DATA,

    SHM_SEM_KEY_DGE_STA = 0x0E000A01,
    SHM_SEM_KEY_DGE_CMD,
    SHM_SEM_KEY_DGE_DATA,

    SHM_SEM_KEY_MCS_STA = 0x0E000B01,
    SHM_SEM_KEY_MCS_CMD,
    SHM_SEM_KEY_MCS_DATA,

    SHM_SEM_KEY_OSC_STA = 0x0E000C01,
    SHM_SEM_KEY_OSC_CMD,
    SHM_SEM_KEY_OSC_DATA,

    SHM_SEM_KEY_MUX_STA = 0x0E000D01,
    SHM_SEM_KEY_MUX_CMD,
    SHM_SEM_KEY_MUX_DATA,

    SHM_SEM_KEY_EXTEND_STA = 0x0E000E01,
    SHM_SEM_KEY_EXTEND_CMD,
    SHM_SEM_KEY_EXTEND_DATA,

    SHM_SEM_KEY_OCS_STA = 0x0E000F01,
    SHM_SEM_KEY_OCS_CMD,
    SHM_SEM_KEY_OCS_DATA
};

enum EShmSize
{
    SHM_SIZE_HAL_STA = (1024),
    SHM_SIZE_HAL_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_HAL_DATA = (1024),

    SHM_SIZE_SLOT_STA = (1024),
    SHM_SIZE_SLOT_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_SLOT_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_BOARD_STA = (1024),
    SHM_SIZE_BOARD_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_BOARD_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_OA_STA = (1024),
    SHM_SIZE_OA_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_OA_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_OCM_STA = (1024),
    SHM_SIZE_OCM_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_OCM_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_WSS_STA = (1024),
    SHM_SIZE_WSS_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_WSS_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_TDCM_STA = (1024),
    SHM_SIZE_TDCM_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_TDCM_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_PAM4_STA = (1024),
    SHM_SIZE_PAM4_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_PAM4_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_OPS_STA = (1024),
    SHM_SIZE_OPS_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_OPS_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_DCE_STA = (1024),
    SHM_SIZE_DCE_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_DCE_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_OTDR_STA = (1024),
    SHM_SIZE_OTDR_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_OTDR_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_DGE_STA = (1024),
    SHM_SIZE_DGE_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_DGE_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_MCS_STA = (1024),
    SHM_SIZE_MCS_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_MCS_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_OSC_STA = (1024),
    SHM_SIZE_OSC_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_OSC_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_MUX_STA = (1024),
    SHM_SIZE_MUX_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_MUX_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_EXTEND_STA = (1024),
    SHM_SIZE_EXTEND_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_EXTEND_DATA = ((1024 + 256) * 1024),

    SHM_SIZE_OCS_STA = (1024),
    SHM_SIZE_OCS_CMD = ((1024 + 256) * 1024),
    SHM_SIZE_OCS_DATA = ((1024 + 256) * 1024)
};



/* shared memory */

/* state */
/* --------------------- */
/* | dev state           | */
/* --------------------- */


/* cmd */
/* --------------------- */
/* |     API status      | */
/* --------------------- */
/* |        queue        | */
/* --------------------- */
/* |      response       | */
/* --------------------- */


/* data */
/* --------------------- */
/* | dev Mfg             | */
/* --------------------- */
/* | dev status data     | */
/* --------------------- */
/* | dev config data     | */
/* --------------------- */

enum EShmStatus
{
    SHM_STATUS_NONE = 0,
    SHM_STATUS_OK,
    SHM_STATUS_BUSY,
    SHM_STATUS_FREE,
    SHM_STATUS_UNREG
};

struct CShmDevSta
{
    int32_t iInitSta;
    int32_t iUpgdSta;
    int32_t iLoadSta;
};

struct CShmSubSta
{
    uint32_t uiType;
    uint32_t uiNum;
};

struct CSlotDbg
{
    uint8_t aucVal[256];
};

struct CSlotData
{
    uint32_t uiSlotNum;
    CMfg astMfg[SLOT_MAX_NUM];
    CSlotInfo astInfo[SLOT_MAX_NUM];
    CSlotStatusData stStaData[SLOT_MAX_NUM];
    CCali astCali[SLOT_MAX_NUM];
    CSlotDbg stDbgData[SLOT_MAX_NUM];
};

struct CBoardFunc
{
    uint32_t uiDevType;
};

struct CBoardData
{
    CBoardMfg stMfg;
    CBoardStatusData stStaData;
    CBoardRealTimeData stRealTimeData;
    CBoardCfgData stCfgData;
    CBoardCali stCali;
    CBoardFunc stFunc;
};


struct COaData
{
    CMfg stMfg;
    COaStatusData stStaData;
    COaCfgData stCfgData;
    COaEventLogAllData stEventLogAllData;
};

struct CWssData
{
    CMfg stMfg;
    CWssStatusData stStaData;
    CWssAllCfg stCfgData;

    uint8_t aucUserData[4 * 1024];
    uint8_t aucPmUserData[64 * 1024];

};

struct COcmData
{
    CMfg stMfg;
    COcmStatusData stStaData;
    COcmCfgData stCfgData;
    COcmChPower astReport[OCM_PORT_MAX_NUM];
    COcmOsa astOsaData[OCM_PORT_MAX_NUM];
};

struct CTdcmData
{
    CMfg stMfg;
    CTdcmStatusData stStaData;
    CTdcmCfgData stCfgData;
};

struct CPam4Data
{
    CMfg stMfg;
    CPam4StatusData stStaData;
    CPam4CfgData stCfgData;
};

struct COpsData
{
    CMfg stMfg;
    COpsCommStatusData stCommStaData;
    COpsStatusData stStaData;
    COpsCfgData stCfgData;
    COpsHistoryData stHistoryData;
    COscSwitchPos stOscSwitchPos;
    COpsEventLogAllData stEventLogAllData;
};

struct CDceData
{
    CMfg stMfg;
    CDceStatusData stStaData;
    CDceCfgData stCfgData;
};

struct CDgeData
{
    CMfg stMfg;
    CDgeStatusData stStaData;
    CDgeCfgData stCfgData;
};

struct CMcsData
{
    CMfg stMfg;
    CMcsStatusData stStaData;
    CMcsCfgData stCfgData;
};

struct COscData
{
    CMfg stMfg;
    COscStatusData stStaData;
    COscCfgData stCfgData;
};

struct COtdrData
{
    CMfg stMfg;
    COtdrStatusData stStaData;
    COtdrCfgData stCfgData;
};

struct CMuxData
{
    CMfg stMfg;
    CMuxStatusData stStaData;
    CMuxCfgData stCfgData;
    CMuxCaliData stCaliData;
};

struct CExtendData
{
    CMfg stMfg;
    CExtendStatusData stStaData;
    CExtendCfgData stCfgData;
};

struct COcsData
{
    CMfg stMfg;
    COcsStatusData stStaData;
    COcsCfgData stCfgData;
};

struct CShmCfg
{
    EShmKey eShmKey;
    EShmSemKey eSemKey;
    EShmSize eShmSize;
};

class CShmOperator
{
public:

    static bool Get(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, uint8_t *pucBuf, uint32_t uiPos, uint32_t uiLen);
    static bool Set(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, uint8_t *pucBuf, uint32_t uiPos, uint32_t uiLen);

    static bool SetSyncCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNodeReq,
                           CQueueNode *pstQNodeRsp, uint32_t uiPrd, uint32_t uiTimeout);
    static bool SetAsyncCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNodeReq);
    static bool GetCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNode);
    static bool GetFastCmd(const CShareMemory &c_rstShm, const CSemaphore &c_rstSem, CQueueNode &rstQNode);

    static uint32_t
    GetStaDevStaPos(void)
    {
        return 0;
    }
    static uint32_t
    GetStaSubStaPos(uint32_t uiSubIdx)
    {
        return sizeof(CShmDevSta) + sizeof(CShmSubSta) * uiSubIdx;
    }

    static uint32_t
    GetCmdApiStaPos(uint32_t uiIdx)
    {
        return offsetof(CCmdQueue, astCmdStatus[uiIdx].uiCmdStatus);
    }
    static uint32_t
    GetCmdRespPos(void)
    {
        return offsetof(CCmdQueue, stResp);
    }

    static uint32_t
    GetDataSlotNumPos(void)
    {
        return offsetof(CSlotData, uiSlotNum);
    }
    static uint32_t
    GetDataSlotInfoPos(uint32_t uiIdx)
    {
        return offsetof(CSlotData, astInfo[uiIdx]);
    }
    static uint32_t
    GetDataSlotMfgPos(uint32_t uiIdx)
    {
        return offsetof(CSlotData, astMfg[uiIdx]);
    }
    static uint32_t
    GetDataSlotCaliPos(uint32_t uiIdx)
    {
        return offsetof(CSlotData, astCali[uiIdx]);
    }

    static uint32_t
    GetDataSlotStatusPos(uint32_t uiIdx)
    {
        return offsetof(CSlotData, stStaData[uiIdx]);
    }

    static uint32_t
    GetDataSlotDbgStaPos(uint32_t uiIdx)
    {
        return offsetof(CSlotData, stDbgData[uiIdx]);
    }

    static uint32_t
    GetDataSlotTempStatusPos(uint32_t uiIdx)
    {
        return GetDataSlotStatusPos(uiIdx) + offsetof(CSlotStatusData, iTemp);
    }

    static uint32_t
    GetDataSlotLedStatusPos(uint32_t uiIdx)
    {
        return GetDataSlotStatusPos(uiIdx) + offsetof(CSlotStatusData, astLed);
    }

    static uint32_t
    GetDataBoardMfgPos(void)
    {
        return offsetof(CBoardData, stMfg);
    }

    static uint32_t
    GetDataChassisMfgPos(void)
    {
        return GetDataBoardMfgPos() + offsetof(CBoardMfg, stChassis);
    }

    static uint32_t
    GetDataChassisCaliPos(void)
    {
        return offsetof(CBoardData, stCali) + offsetof(CBoardCali, stChassis);
    }

    static uint32_t
    GetDataSccMfgPos(void)
    {
        return GetDataBoardMfgPos() + offsetof(CBoardMfg, stScc);
    }

    static uint32_t
    GetDataRpcMfgPos(void)
    {
        return GetDataBoardMfgPos() + offsetof(CBoardMfg, stRpc);
    }

    static uint32_t
    GetDataBoardStaPos(void)
    {
        return offsetof(CBoardData, stStaData);
    }
    static uint32_t
    GetDataBoardCfgPos(void)
    {
        return offsetof(CBoardData, stCfgData);
    }

    static uint32_t
    GetDataFanMfgPos(uint32_t uiDevIdx)
    {
        return GetDataBoardMfgPos() + offsetof(CBoardMfg, astFan) + sizeof(CMfg) * uiDevIdx;
    }
    static uint32_t
    GetDataFanStaPos(uint32_t uiDevIdx)
    {
        return offsetof(CBoardData, stStaData) + offsetof(CBoardStatusData, astFan) + sizeof(CFanSta) * uiDevIdx;
    }
    static uint32_t
    GetDataFanCfgPos(uint32_t uiDevIdx)
    {
        return offsetof(CBoardData, stCfgData) + offsetof(CBoardCfgData, astFan) + sizeof(CFanCfg) * uiDevIdx;
    }

    static uint32_t
    GetDataPowerMfgPos(uint32_t uiDevIdx)
    {
        return GetDataBoardMfgPos() + offsetof(CBoardMfg, astPower) + sizeof(CMfg) * uiDevIdx;
    }
    static uint32_t
    GetDataPowerStaPos(uint32_t uiDevIdx)
    {
        return offsetof(CBoardData, stStaData) + offsetof(CBoardStatusData, astPower) + sizeof(CPowerSta) * uiDevIdx;
    }
    static uint32_t
    GetDataPowerCfgPos(uint32_t uiDevIdx)
    {
        return offsetof(CBoardData, stCfgData) + offsetof(CBoardCfgData, astPower) + sizeof(CPowerCfg) * uiDevIdx;
    }

    static uint32_t
    GetDataLogicStaPos()
    {
        return offsetof(CBoardData, stRealTimeData) + offsetof(CBoardRealTimeData, aucLogicVal);
    }

    static uint32_t
    GetDataUpgInfoPos(void)
    {
        return offsetof(CBoardData, stRealTimeData) + offsetof(CBoardRealTimeData, aucUpgInfo);
    }


    static uint32_t
    GetDataBdlHeadPos(void)
    {
        return offsetof(CBoardData, stRealTimeData) + offsetof(CBoardRealTimeData, aucBdlHead);
    }

    static uint32_t
    GetDataFuncDevType()
    {
        return offsetof(CBoardData, stFunc) + offsetof(CBoardFunc, uiDevType);
    }

    static uint32_t
    GetDataOaMfgPos(void)
    {
        return offsetof(COaData, stMfg);
    }

    static uint32_t
    GetDataOaStaPos(uint32_t uiSubIdx = 0)
    {
        return offsetof(COaData, stStaData) + (sizeof(COaStatusData) + sizeof(COaCfgData) + sizeof(COaEventLogAllData)) * uiSubIdx;
    }

    static uint32_t
    GetDataOaEventLogAllPos(uint32_t uiSubIdx = 0)
    {
        return offsetof(COaData, stEventLogAllData) + (sizeof(COaStatusData) + sizeof(COaCfgData) + sizeof(COaEventLogAllData)) * uiSubIdx;
    }

    static uint32_t
    GetDataOaAlmPos(uint32_t uiSubIdx = 0)
    {
        return GetDataOaStaPos(uiSubIdx) + offsetof(COaStatusData, stOaAlm);
    }

    static uint32_t
    GetDataOaDbgPos(uint32_t uiSubIdx = 0)
    {
        return offsetof(COaData, stStaData) + offsetof(COaStatusData, aucDbgBuf) + (sizeof(COaStatusData) + sizeof(COaCfgData) + sizeof(COaEventLogAllData)) * uiSubIdx;
    }

    static uint32_t
    GetDataOaCfgPos(uint32_t uiSubIdx = 0)
    {
        return offsetof(COaData, stCfgData) + (sizeof(COaStatusData) + sizeof(COaCfgData) + sizeof(COaEventLogAllData)) * uiSubIdx;
    }

    static uint32_t
    GetDataOaCaliPos(uint32_t uiSubIdx = 0)
    {
        return offsetof(COaData, stCfgData) + offsetof(COaCfgData, stCaliData) + (sizeof(COaStatusData) + sizeof(COaCfgData) + sizeof(COaEventLogAllData)) * uiSubIdx;
    }

    static uint32_t
    GetDataOaShutterPos(uint32_t uiSubIdx = 0)
    {
        return offsetof(COaData, stStaData) + offsetof(COaStatusData, stOaAnnexData) + offsetof(COaAnnexData, ucShutterSta) + (sizeof(COaStatusData) + sizeof(COaCfgData) + sizeof(COaEventLogAllData)) * uiSubIdx;
    }


    static uint32_t
    GetDataWssMfgPos(void)
    {
        return offsetof(CWssData, stMfg);
    }
    static uint32_t
    GetDataWssStaPos(void)
    {
        return offsetof(CWssData, stStaData);
    }
    static uint32_t
    GetDataWssCfgPos(void)
    {
        return offsetof(CWssData, stCfgData);
    }

    static uint32_t
    GetDataWssUserDataPos(void)
    {
        return offsetof(CWssData, aucUserData);
    }

    static uint32_t
    GetDataPmUserDataPos(void)
    {
        return offsetof(CWssData, aucPmUserData);
    }

    static uint32_t
    GetDataOcmMfgPos(void)
    {
        return offsetof(COcmData, stMfg);
    }
    static uint32_t
    GetDataOcmStaPos(void)
    {
        return offsetof(COcmData, stStaData);
    }
    static uint32_t
    GetDataOcmCfgPos(void)
    {
        return offsetof(COcmData, stCfgData);
    }
    static uint32_t
    GetDataOcmRptPos(uint32_t uiPortIdx)
    {
        return offsetof(COcmData, astReport[uiPortIdx]);
    }

    static uint32_t
    GetDataOcmOsaPos(uint32_t uiPortIdx)
    {
        return offsetof(COcmData, astOsaData[uiPortIdx]);
    }

    static uint32_t
    GetDataTdcmMfgPos(void)
    {
        return offsetof(CTdcmData, stMfg);
    }
    static uint32_t
    GetDataTdcmStaPos(void)
    {
        return offsetof(CTdcmData, stStaData);
    }
    static uint32_t
    GetDataTdcmCfgPos(void)
    {
        return offsetof(CTdcmData, stCfgData);
    }

    static uint32_t
    GetDataPam4MfgPos(void)
    {
        return offsetof(CPam4Data, stMfg);
    }
    static uint32_t
    GetDataPam4StaPos(void)
    {
        return offsetof(CPam4Data, stStaData);
    }
    static uint32_t
    GetDataPam4CfgPos(void)
    {
        return offsetof(CPam4Data, stCfgData);
    }

    static uint32_t
    GetDataOpsMfgPos(void)
    {
        return offsetof(COpsData, stMfg);
    }

    static uint32_t
    GetCommonDataOpsStaPos(void)
    {
        return offsetof(COpsData, stCommStaData);
    }

    static uint32_t
    GetDataOpsStaPos(uint32_t uiSubIdx = 1)
    {
        return offsetof(COpsData, stStaData) + (sizeof(COpsStatusData) + sizeof(COpsCfgData) + sizeof(COpsHistoryData) + sizeof(COscSwitchPos) + sizeof(COpsEventLogAllData)) * (uiSubIdx - 1);
    }
    static uint32_t
    GetDataOpsCfgPos(uint32_t uiSubIdx = 1)
    {
        return offsetof(COpsData, stCfgData) + (sizeof(COpsStatusData) + sizeof(COpsCfgData) + sizeof(COpsHistoryData) + sizeof(COscSwitchPos) + sizeof(COpsEventLogAllData)) * (uiSubIdx - 1);
    }

    static uint32_t
    GetDataOpsLedStaPos(void)
    {
        return GetCommonDataOpsStaPos() + offsetof(COpsCommStatusData, stLed);
    }

    static uint32_t
    GetDataOpsSwitchThrPos(uint32_t uiSubIdx = 1)
    {
        return GetDataOpsCfgPos(uiSubIdx) + offsetof(COpsCfgData, stSwitchThr);
    }

    static uint32_t
    GetDataOpsSwitchHysPos(uint32_t uiSubIdx = 1)
    {
        return GetDataOpsCfgPos(uiSubIdx) + offsetof(COpsCfgData, stSwitchHys);
    }

    static uint32_t
    GetDataOpsHistoryPos(uint32_t uiSubIdx = 1)
    {
        return offsetof(COpsData, stHistoryData) + (sizeof(COpsStatusData) + sizeof(COpsCfgData) + sizeof(COpsHistoryData) + sizeof(COscSwitchPos) + sizeof(COpsEventLogAllData)) * (uiSubIdx - 1);
    }

    static uint32_t
    GetDataOpsWssStaPos(uint32_t uiSubIdx = 1)
    {
        /* return offsetof(COpsData, stWssStaData); */
        return GetDataOpsStaPos(uiSubIdx) + offsetof(COpsStatusData, stWssStaData);
    }

    static uint32_t
    GetDataOpsOscSwitchPos(uint32_t uiSubIdx = 1)
    {
        return offsetof(COpsData, stOscSwitchPos) + (sizeof(COpsStatusData) + sizeof(COpsCfgData) + sizeof(COpsHistoryData) + sizeof(COscSwitchPos) + sizeof(COpsEventLogAllData)) * (uiSubIdx - 1);
    }

    static uint32_t
    GetDataOpsBidiStaPos(uint32_t uiSubIdx = 1)
    {
        return GetDataOpsStaPos(uiSubIdx) + offsetof(COpsStatusData, stBidiStatus);
    }

    static uint32_t
    GetDataOpsEventLogAllPos(uint32_t uiSubIdx = 1)
    {
        return offsetof(COpsData, stEventLogAllData) + (sizeof(COpsStatusData) + sizeof(COpsCfgData) + sizeof(COpsHistoryData) + sizeof(COscSwitchPos) + sizeof(COpsEventLogAllData)) * (uiSubIdx - 1);
    }

    static uint32_t
    GetDataDceMfgPos(void)
    {
        return offsetof(CDceData, stMfg);
    }
    static uint32_t
    GetDataDceStaPos(void)
    {
        return offsetof(CDceData, stStaData);
    }
    static uint32_t
    GetDataDceCfgPos(void)
    {
        return offsetof(CDceData, stCfgData);
    }

    static uint32_t
    GetDataDgeMfgPos(void)
    {
        return offsetof(CDgeData, stMfg);
    }
    static uint32_t
    GetDataDgeStaPos(void)
    {
        return offsetof(CDgeData, stStaData);
    }
    static uint32_t
    GetDataDgeCfgPos(void)
    {
        return offsetof(CDgeData, stCfgData);
    }

    static uint32_t
    GetDataMcsMfgPos(void)
    {
        return offsetof(CMcsData, stMfg);
    }
    static uint32_t
    GetDataMcsStaPos(void)
    {
        return offsetof(CMcsData, stStaData);
    }
    static uint32_t
    GetDataMcsCfgPos(void)
    {
        return offsetof(CMcsData, stCfgData);
    }

    static uint32_t
    GetDataOscMfgPos(void)
    {
        return offsetof(COscData, stMfg);
    }
    static uint32_t
    GetDataOscStaPos(void)
    {
        return offsetof(COscData, stStaData);
    }
    static uint32_t
    GetDataOscCfgPos(void)
    {
        return offsetof(COscData, stCfgData);
    }

    static uint32_t
    GetDataOtdrMfgPos(void)
    {
        return offsetof(COtdrData, stMfg);
    }

    static uint32_t
    GetDataOtdrStaPos(void)
    {
        return offsetof(COtdrData, stStaData);
    }

    static uint32_t
    GetDataOtdrTempPos(void)
    {
        return offsetof(COtdrData, stStaData) + offsetof(COtdrStatusData, stTemp);
    }

    static uint32_t
    GetDataOtdrCfgPos(void)
    {
        return offsetof(COtdrData, stCfgData);
    }

    static uint32_t
    GetDataOtdrDbgPos()
    {
        return offsetof(COtdrData, stStaData) + offsetof(COtdrStatusData, aucDbgBuf);
    }

    static uint32_t
    GetDataMuxMfgPos(void)
    {
        return offsetof(CMuxData, stMfg);
    }
    static uint32_t
    GetDataMuxStaPos(void)
    {
        return offsetof(CMuxData, stStaData);
    }
    static uint32_t
    GetDataMuxCfgPos(void)
    {
        return offsetof(CMuxData, stCfgData);
    }
    static uint32_t
    GetDataMuxCaliPos(void)
    {
        return offsetof(CMuxData, stCaliData);
    }

    static uint32_t
    GetDataExtendMfgPos(void)
    {
        return offsetof(CExtendData, stMfg);
    }
    static uint32_t
    GetDataExtendStaPos(void)
    {
        return offsetof(CExtendData, stStaData);
    }
    static uint32_t
    GetDataExtendCfgPos(void)
    {
        return offsetof(CExtendData, stCfgData);
    }

    static uint32_t
    GetDataExtendDbgPos()
    {
        return offsetof(CExtendData, stStaData) + offsetof(CExtendStatusData, aucDbgBuf);
    }

    static uint32_t
    GetDataOtdrStartOffsetPos(void)
    {
        return offsetof(COtdrData, stStaData) + offsetof(COtdrStatusData, stStartOffset);
    }

    static uint32_t
    GetDataOcsMfgPos(void)
    {
        return offsetof(COcsData, stMfg);
    }

    static uint32_t
    GetDataOcsStaPos(void)
    {
        return offsetof(COcsData, stStaData);
    }


    static uint32_t
    GetDataOcsAlmPos(void)
    {
        return GetDataOcsStaPos() + offsetof(COcsStatusData, stAlm);
    }

    static uint32_t
    GetDataOcsStaConnsPos(void)
    {
        return GetDataOcsStaPos() + offsetof(COcsStatusData, stConnsSta);
    }

    static uint32_t
    GetDataOcsStaLedPos(void)
    {
        return GetDataOcsStaPos() + offsetof(COcsStatusData, stLedSta);
    }

    static uint32_t
    GetDataOcsCfgPos(void)
    {
        return offsetof(COcsData, stCfgData);
    }

private:

};

