/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaDriver.h
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
#include "DevDriver.h"
#include "UartDriver.h"
#include "DevData.h"
#include "BoardCmd.h"

#include "OaProto.h"
#include "RspHandler.h"

/*************************************************/

struct EDFA_ALLINFO_ST
{
    /* Module Status :MST,ÿ��BITλ����BIT0-OK/DIS/ES/LIM/EOP/NOTRD, ����MST���غܶ�״̬������BITλ����ʾ*/
    int16_t asStatus[2];                               /* PUMP ON or OFF status */

    /* EDFA Control Mode :Mode,0-D��1-APC,2- AGC ��3-M */
    int16_t asModeSet[2];                            /* Operating Mode */

    /* EDFA Gain :GAIN*/
    int16_t asGain[2];                            /* Actual gain 100 times */

    /* Edfa Output Pwr Signal */
    int16_t asOsp[2];                             /* Output Signal Power (Excluding ASE) ,amplify 100 times */

    /* PLIM/GLIM */
    int16_t asPlim[2];
    int16_t asGlim[2];

    /* PD */
    int16_t asPD[16];                                /* PD Power, amplify 100 times */

    /* Alarm info, Ϊ�˼����Ժ��SST״̬���Ը澯��2�飬ÿ��澯����E1��E2����һ����������Ĳ�ͬBITλ*/
    int16_t asAlarm1[2];                            /* ��ӦPD1~16��[0]STA��ǰ�澯״̬��[1]SST-STICK�澯״̬ */
    int16_t asAlarm2[2];                            /* PUMP 1~4 TMP/ILD;MTH/MTL/COT */
                                                    /*PA��[0]BIT0-E1 PUMP1��[0]BIT1-E1 PUMP2��[0]BIT8-MTH��[0]BIT9-MTL��[0]BIT10-COT */
                                                    /*BA��[0]BIT0-E1 PUMP1��[0]BIT1-E2 PUMP1��[0]BIT8-MTH��[0]BIT9-MTL��[0]BIT10-COT */
    int16_t asEXTAlarm[2];

    /* ASE poower */
    /* sASEPwr[0] : E1 PDin  ASE, Unit��dB, reserved */
    /* sASEPwr[1] : E1 PDout ASE, Unit��dB */
    /* sASEPwr[2] : E2 PDin  ASE, Unit��dB, reserved */
    /* sASEPwr[3] : E2 PDout ASE, Unit��dB */
    int16_t asASEPwr[4];                            /* ASE Power, amplify 100 times */

    /* Back reflected delta pwr */
    int16_t asRflD[2];                              /* |RFL Power - OTP|, amplify 100 times */

    /* EDFA VOA:VOA */
    /* PA��[0] -E1VOA1 */
    /* BA��[0] -E1VOA1 */
    int16_t asVOASet[6];                            /* VOA SET result */
    int16_t asVOAAct[6];                            /* VOA ACT result */
    int16_t asVOAStatus[6];                         /* VOA STA result: 0-OK */

    /* Case Temperature */
    int16_t sCtValue;                              /* amplify 10 times */

    /* Heater Temperature */
    int16_t sHtValue;                              /* amplify 10 times */

    /* Pump info*/
    /* PA��[0]-E1 pump1��[1]-E1 pump2 */
    /* BA��[0]-E1 pump1��[1]-E2 pump1 */
    int16_t asPtValue[4];                           /* Pump Temperature, amplify 10 times */
    int16_t asPcValue[4];                           /* Pump Current, amplify 10 times */

    /* Tilt info*/
    /* PA��[0]-E1 pump1 */
    /* BA��[0]-E1 pump1��[1]-E1 pump2 */
    int16_t asTiltSet[2];                           /* tilt set */
    int16_t asTiltAct[2];                           /* tilt actual */
    /* all BYTE 136+8: */
};


struct EDFA_DATA_ST
{
    int16_t sHead;                                 /*Frame head=0xFEFE */
    int16_t sSTVer;                                /*struct version=0x0001 */

    EDFA_ALLINFO_ST stEDFA;

    int16_t sReserve;                              /* for word Data alignment */
    int16_t sCheckSum;                             /*check sum */
};

#ifdef __400GZR__

enum
{
    PD_IN = 0,
    PD_OUT = 1,
    PD_VOA2_IN = 1,
    PD_VOA2_OUT,
    PD_OSC_IN,
    PD_OSC_OUT,
    PD_REF = 4,
};

#else

enum
{
    PD_IN = 0,
    PD_OUT = 1,
    PD_VOA2_IN = 1,
    PD_VOA2_OUT,
    PD_OSC_IN,
    PD_OSC_OUT,
    PD_REF = 4,

    PD_RATIO = 14,
};


#endif

#define NO_POWER_6000   (-6000)

class COaDriver : public CDevDriver
{
public:
    /*COaDriver(uint32_t uiHwIdx) : CDevDriver(uiHwIdx)
       {
       }
       COaDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : CDevDriver(uiHwIdx, uiHwSubIdx)
       {
       }*/

    COaDriver(uint32_t uiHwIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }

    COaDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx, uint32_t uiResetHwIdx, uint32_t uiResetHwSubIdx) :
        CDevDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx)
    {

    }

    virtual
    ~COaDriver()
    {
    }

    virtual bool IsOnline(void) = 0;

    virtual bool GetMfg(CMfg &rstMfg) = 0;
    virtual bool GetStatusData(std::vector<COaStatusData> &rstaData) = 0;
    virtual bool GetCfgData(COaCfgData &rstData, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetGain(int16_t &rsGain, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetTilt(int16_t &rsTilt, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetRxLosThr(int16_t &rsThr, int16_t &rsHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetDebugInfo(const char *pcFilePath) = 0;
    virtual bool GetMcuDebugInfo(const char *pcFilePath, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetAutolosEnable(bool &rbEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetPumpEnable(bool &rbEnable, uint32_t uiSubIdx = 0) = 0;
    /* OA */
    virtual bool SetGainRange(uint32_t uiGainRange, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetGain(uint32_t uiGain, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetTilt(int16_t sTilt, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetMode(uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetRxLosThr(int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetPumpEnable(bool bEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetAprEnable(bool bEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetAutolosEnable(bool bEnable, uint32_t uiSubIdx = 0) = 0;

    /* VOA */
    virtual bool SetAttn(uint32_t uiAttn, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetTarget(uint32_t uiTarget, uint32_t uiSubIdx = 0) = 0;

    /* VOA get */
    virtual bool GetAttn(int16_t &rsAttn, uint32_t uiSubIdx = 0) = 0;

    /* Shutter */
    virtual bool SetShutterState(bool block, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetShutterState(bool &block, uint32_t uiSubIdx = 0) = 0;

    /* reset */
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) = 0;

    /* upgrade */
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) = 0;
    virtual bool Swap(uint32_t uiSubIdx = 0) = 0;
    virtual bool Commit(uint32_t uiSubIdx = 0) = 0;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) = 0;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) = 0;
    virtual bool SetDfbEnable(bool bEnable, int32_t iPower, uint32_t uiSubIdx) = 0;
    virtual bool SetOscEnable(bool bEnable, uint32_t uiSubIdx) = 0;
    virtual bool SetOtdrSwitchPos(uint32_t ulPos, uint32_t uiSubIdx) = 0;
    virtual bool SetOscAddThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) = 0;
    virtual bool SetOscDropThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) = 0;
    virtual bool SetOscRxThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) = 0;
    virtual bool SetApsdEnable(bool bEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetUpgState(uint32_t uiState, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetLedState(uint32_t uiState, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetLedAlm(uint32_t uiState, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetCaliInfo(COaCaliData &rstCaliData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetOtdrCali(char *c_pcFilePath, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetOcmCali(char *c_pcFilePath, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetManualVoaEnable(bool bEnable, uint32_t uiSubIdx) = 0;
    virtual bool SetPaddingVoaConfigPower(uint32_t uiPower, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetManualGainEnable(bool bEnable, uint32_t uiSubIdx) = 0;
    virtual bool SetThr(uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetPumpCurrent(uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetAlmMask(int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx = 0) = 0;
    virtual bool GetEventLog(COaEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetReadyTrigger(uint32_t uiState, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetAutoGainMode(uint32_t uiMode, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetRemoteInterlockEnable(bool bEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetDummyAdminKey(bool bEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetPanelSafeCmpt(bool bEnable, uint32_t uiSubIdx = 0) = 0;
    virtual bool SetOscAmpEnable(bool bEnable, uint32_t uiSubIdx) = 0;
    virtual bool SetReadyPower(uint32_t uiPower, uint32_t uiSubIdx) = 0;
protected:
    std::shared_ptr<CResetBoard> m_stpResetBoardPtr = nullptr;
};

class COaIoDriver : public COaDriver
{
public:
    /*COaIoDriver(uint32_t uiHwIdx) : COaDriver(uiHwIdx)
       {
       }
       COaIoDriver(uint32_t uiHwIdx, uint32_t uiHwSubIdx) : COaDriver(uiHwIdx, uiHwSubIdx)
       {
       }*/
    COaIoDriver(uint32_t uiHwIdx,
                uint32_t uiHwSubIdx,
                uint32_t uiResetHwIdx,
                uint32_t uiResetHwSubIdx,
                const std::shared_ptr<CInterfaceDriver> &c_rstpDrvPtr,
                const std::shared_ptr<COaSwProto> &c_rstpProtoPtr) :
        COaDriver(uiHwIdx, uiHwSubIdx, uiResetHwIdx, uiResetHwSubIdx), m_stpDrvPtr(c_rstpDrvPtr), m_stpProtoPtr(c_rstpProtoPtr)
    {
        /* TBD: Logic Driver */
        m_stpResetBoardPtr = CBoardCmd::GetResetBoardPtr(uiResetHwIdx);
    }
    virtual
    ~COaIoDriver()
    {
    }

    virtual bool IsOnline(void) override;

    virtual bool GetMfg(CMfg &rstMfg) override;
    virtual bool GetStatusData(std::vector<COaStatusData> &rstaData) override;
    virtual bool GetCfgData(COaCfgData &rstData, uint32_t uiSubIdx = 0) override;
    virtual bool GetGain(int16_t &rsGain, uint32_t uiSubIdx = 0) override;
    virtual bool GetTilt(int16_t &rsTilt, uint32_t uiSubIdx = 0) override;
    virtual bool GetRxLosThr(int16_t &rsThr, int16_t &rsHys, uint32_t uiSubIdx = 0) override;
    virtual bool GetDebugInfo(const char *pcFilePath) override;
    virtual bool GetMcuDebugInfo(const char *pcFilePath, uint32_t uiSubIdx = 0) override;
    virtual bool GetAutolosEnable(bool &rbEnable, uint32_t uiSubIdx = 0) override;
    virtual bool GetPumpEnable(bool &rbEnable, uint32_t uiSubIdx = 0) override;
    /* OA */
    virtual bool SetGainRange(uint32_t uiGainRange, uint32_t uiSubIdx = 0) override;
    virtual bool SetGain(uint32_t uiGain, uint32_t uiSubIdx = 0) override;
    virtual bool SetTilt(int16_t sTilt, uint32_t uiSubIdx = 0) override;
    virtual bool SetMode(uint8_t ucMode, int16_t sVal, uint32_t uiSubIdx = 0) override;
    virtual bool SetRxLosThr(int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0) override;
    virtual bool SetPumpEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetAprEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetAutolosEnable(bool bEnable, uint32_t uiSubIdx = 0) override;

    /* VOA */
    virtual bool SetAttn(uint32_t uiAttn, uint32_t uiSubIdx = 0) override;
    virtual bool SetTarget(uint32_t uiTarget, uint32_t uiSubIdx = 0) override;

    /* VOA get */
    virtual bool GetAttn(int16_t &rsAttn, uint32_t uiSubIdx = 0) override;

    /* Shutter */
    virtual bool SetShutterState(bool block, uint32_t uiSubIdx = 0) override;
    virtual bool GetShutterState(bool &block, uint32_t uiSubIdx = 0) override;

    /* reset */
    virtual bool Reset(uint8_t ucResetType, uint32_t uiSubIdx = 0) override;

    /* upgrade */
    virtual bool Upgrade(uint32_t uiDevIdx, const char *pcFilePath) override;
    virtual bool Swap(uint32_t uiSubIdx = 0) override;
    virtual bool Commit(uint32_t uiSubIdx = 0) override;

    /* debug */
    virtual bool DbgRead(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool DbgWrite(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen) override;
    virtual bool SetDfbEnable(bool bEnable, int32_t iPower, uint32_t uiSubIdx) override;
    virtual bool SetOscEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetOtdrSwitchPos(uint32_t ulPos, uint32_t uiSubIdx) override;
    virtual bool SetOscAddThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) override;
    virtual bool SetOscDropThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) override;
    virtual bool SetOscRxThrHys(int16_t sThr, int16_t sHys, uint32_t uiSubIdx) override;
    virtual bool SetApsdEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetUpgState(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool SetLedState(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool SetLedAlm(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool GetCaliInfo(COaCaliData &rstCaliData, uint32_t uiSubIdx = 0) override;
    virtual bool SetOtdrCali(char *c_pcFilePath, uint32_t uiSubIdx = 0) override;
    virtual bool SetOcmCali(char *c_pcFilePath, uint32_t uiSubIdx = 0) override;
    virtual bool SetManualVoaEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetPaddingVoaConfigPower(uint32_t uiPower, uint32_t uiSubIdx = 0) override;
    virtual bool SetManualGainEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetThr(uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys, uint32_t uiSubIdx = 0) override;
    virtual bool SetPumpCurrent(uint32_t uiPumpIdx, int16_t sCurrent, uint32_t uiSubIdx = 0) override;
    virtual bool SetAlmMask(int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx = 0) override;
    virtual bool GetEventLog(COaEventLogAllData &rstEventLogAllData, uint32_t uiSubIdx = 0) override;
    virtual bool SetReadyTrigger(uint32_t uiState, uint32_t uiSubIdx = 0) override;
    virtual bool SetAutoGainMode(uint32_t uiMode, uint32_t uiSubIdx = 0) override;
    virtual bool SetRemoteInterlockEnable(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetDummyAdminKey(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetPanelSafeCmpt(bool bEnable, uint32_t uiSubIdx = 0) override;
    virtual bool SetOscAmpEnable(bool bEnable, uint32_t uiSubIdx) override;
    virtual bool SetReadyPower(uint32_t uiPower, uint32_t uiSubIdx) override;
private:
    std::shared_ptr<CInterfaceDriver> m_stpDrvPtr = nullptr;
    std::shared_ptr<COaSwProto> m_stpProtoPtr = nullptr;


    std::shared_ptr<CRspHandler> __RspHandler(uint32_t uiTimeOut = 50);
    bool __SendCmd(std::string &rstaMsg, std::shared_ptr<CRspHandler> stpRspHandler);
    bool __SendSetCmd(std::string &rstaMsg, uint32_t uiTimeOut = 50);
    bool __UpgradeEnd();

    EDFA_DATA_ST m_stOaData;
};










