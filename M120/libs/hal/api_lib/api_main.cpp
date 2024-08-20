/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Api_main.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "DevInternal.h"
#include "DevApiManager.h"
#include "HalApi.h"
#include "OaApi.h"
#include "DgeApi.h"
#include "OcmApi.h"
#include "OcsApi.h"

#include "DevData.h"
#include "Cmd.h"
#include "Delay.h"

/*==============================function=========================*/

#define PA_IDX 1
#define BA_IDX 0

struct timeval startTime, stopTime;

void
__PrintUnknownCmd(uint32_t uiCmd)
{
    std::cout << "Unknown Sub Command. uiCmd = " << uiCmd << std::endl;
}

static void
__StartTime()
{
    gettimeofday(&startTime, NULL);
}

static void
__StopTime()
{
    gettimeofday(&stopTime, NULL);

    long elapsed = (stopTime.tv_sec - startTime.tv_sec) * 1000000 + (stopTime.tv_usec - startTime.tv_usec);
    elapsed /= 1000;

    std::cout << "elapsed time : " << elapsed << "ms" << std::endl;
    std::cout << std::endl;
}

int32_t
__CmdIdx(char *pcCmd, std::vector<std::string> &rstaCmdVec)
{
    std::string staCmdString(pcCmd);

    for (int32_t iIdx = 0; iIdx < (int32_t)rstaCmdVec.size(); iIdx++) {
        if ( 0 == staCmdString.compare(rstaCmdVec[iIdx])) {
            return iIdx;
        }
    }

    return -1;
}



void
__OtdrStartScan()
{

    int32_t iSta = -1;

    iSta = CDevApiManager::GetInstance().GetOtdrApi().StartScan(0, DEV_TYPE_OTDR);

    if (OPLK_OK == iSta) {
        std::cout << "start scan done." << std::endl;
    } else {
        std::cout << "start scan error. iSta = " << iSta << std::endl;
    }
}

void
__OtdrSetPara()
{
    int32_t iSta = -1;

    __StartTime();
    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().SetPara(0, 25, 20, 2.5))) {
        std::cout << "otdr set para ok. " << iSta << std::endl;
    } else {
        std::cout << "otdr set para error. " << iSta << std::endl;
    }
    __StopTime();
}

void
__OtdrSetUserIOR()
{
    int32_t iSta = -1;

    __StartTime();
    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().SetUserIOR(0, 1.456789))) {
        std::cout << "otdr set user ior ok. " << iSta << std::endl;
    } else {
        std::cout << "otdr set user ior error. " << iSta << std::endl;
    }
    __StopTime();
}

void
__OtdrSetThr()
{
    int32_t iSta = -1;

    __StartTime();
    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().SetThr(0, -50, 2.16, 3))) {
        std::cout << "otdr set thr ok. " << iSta << std::endl;
    } else {
        std::cout << "otdr set thr error. " << iSta << std::endl;
    }
    __StopTime();
}

void
__OtdrGetMfg()
{
    CMfg stMfg;
    int32_t iSta;

    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().GetMfg(0, stMfg))) {
        std::cout << "OTDR mfg: " << std::endl;
        std::cout << "MFG:" << stMfg.acManufacturer << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Sw Ver:" << stMfg.acSwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "otdr mfg error. " << iSta << std::endl;
        std::cout << std::endl;
    }
}

void
__OtdrReset(uint32_t uiType)
{
    int32_t iSta;

    if (CDevApiManager::GetInstance().GetOtdrApi().Reset(0, uiType) != OPLK_OK) {
        std::cout << "Upgrade error." << std::endl;
        std::cout << std::endl;
    }

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOtdrApi().GetUpgadeStatus(0);
        /* std::cout << "Otdr " << "iSta = " << iSta << std::endl; */

        CDelay::Delay(1, S);
        continue;
    }
}

void
__OtdrUpgrade(const char *c_pcFilePath)
{
    int32_t iSta;

    __StartTime();

    __OtdrGetMfg();

    if ((iSta = CDevApiManager::GetInstance().GetOtdrApi().Upgrade(0, c_pcFilePath)) != OPLK_OK) {
        std::cout << "Otdr upgrade error." << iSta << std::endl;
    } else {
        std::cout << "Otdr upgrade done." << std::endl;
    }

    CDelay::Delay(3, S);

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOtdrApi().GetUpgadeStatus(0);
        /* std::cout << "Otdr iSta = " << iSta << std::endl; */

        CDelay::Delay(1, S);
        continue;
    }

    __OtdrReset(RST_MODE_WARM);

    __OtdrGetMfg();

    __StopTime();
}

void
__OtdrTemp(uint32_t uiInterval)
{
    int32_t iSta;

    while (1) {

        COtdrTemp stTemp = {0};
        iSta = CDevApiManager::GetInstance().GetOtdrApi().GetTemp(0, stTemp);

        if (iSta != OPLK_OK) {
            std::cout << "Otdr temp error." << iSta << std::endl;
            return;
        } else {
            std::cout << "Otdr temp done." << std::endl;
        }

        std::cout << "fTecTemp = " << stTemp.fTecTemp << std::endl;
        std::cout << "fModuleTemp = " << stTemp.fModuleTemp << std::endl;
        std::cout << "fFpgaTemp = " << stTemp.fFpgaTemp << std::endl;
        std::cout << std::endl;

        CDelay::Delay(uiInterval, MS);
    }
}

void
__OtdrCommFail()
{
    int32_t iSta = CDevApiManager::GetInstance().GetOtdrApi().DbgSetCommFail(0);

    if (iSta != OPLK_OK) {
        std::cout << "Otdr DbgSetCommFail error." << iSta << std::endl;
    } else {
        std::cout << "Otdr DbgSetCommFail done." << std::endl;
    }
}

void
__OtdrClearCommFail()
{
    int32_t iSta = CDevApiManager::GetInstance().GetOtdrApi().DbgClearCommFail(0);

    if (iSta != OPLK_OK) {
        std::cout << "Otdr DbgClearCommFail error." << iSta << std::endl;
    } else {
        std::cout << "Otdr DbgClearCommFail done." << std::endl;
    }
}

void
__OtdrGetStartOffset()
{
    int32_t iSta;


    COtdrStartOffset stStartOffset = {0};
    iSta = CDevApiManager::GetInstance().GetOtdrApi().GetStartOffset(0, stStartOffset);

    if (iSta != OPLK_OK) {
        std::cout << "Otdr get startoffset error." << iSta << std::endl;
        return;
    } else {
        std::cout << "Otdr get startoffset done." << std::endl;
    }

    std::cout << "uiPositionOffset = " << stStartOffset.uiPositionOffset << std::endl;
    std::cout << "fLossOffset = " << stStartOffset.fLossOffset << std::endl;
    std::cout << std::endl;

}

void
__OtdrSetStartOffset(uint32_t uiPositionOffset, float fLossOffset)
{
    int32_t iSta;


    COtdrStartOffset stStartOffset = {uiPositionOffset, fLossOffset};

    iSta = CDevApiManager::GetInstance().GetOtdrApi().SetStartOffset(0, &stStartOffset);

    if (iSta != OPLK_OK) {
        std::cout << "Otdr set startoffset error." << iSta << std::endl;
        return;
    } else {
        std::cout << "Otdr set startoffset done." << std::endl;
    }

}


void
__OtdrTest(char *argv[])
{
    std::vector<std::string> staCmdVec = {"mfg", "scan", "set-para", "set-user-ior", "set-thr", "reset",
                                          "upgrade", "temp", "comm-fail", "clear-comm-fail", "setso", "getso"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);
    int32_t uiPositionOffset = 0;
    float fLossOffset = 0.0;

    switch (iCmdIdx) {
    case 0:
        __OtdrGetMfg();
        break;

    case 1:
        __OtdrStartScan();
        break;
    case 2:
        __OtdrSetPara();
        break;

    case 3:
        __OtdrSetUserIOR();
        break;

    case 4:
        __OtdrSetThr();
        break;

    case 5:
        __OtdrReset(RST_MODE_COLD);
        break;

    case 6:
        __OtdrUpgrade(argv[3]);
        break;

    case 7:
        __OtdrTemp(atoi(argv[3]));
        break;

    case 8:
        __OtdrCommFail();
        break;

    case 9:
        __OtdrClearCommFail();
        break;

    case 10:
        uiPositionOffset = (atoi(argv[3]));
        fLossOffset = (atof(argv[4]));
        __OtdrSetStartOffset(uiPositionOffset, fLossOffset);
        break;

    case 11:
        __OtdrGetStartOffset();
        break;

    default:
        std::cout << "Unknown Command." << std::endl;
    }

    return;

    /* return; */
    /* CMfg stMfg; */
    int32_t iSta;


    COtdrStatusData stStatus;
    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().GetStatusData(0, stStatus))) {
        std::cout << "otdr get status ok. " << iSta << std::endl;
        std::cout << "otdr scan status " << stStatus.ulScanStatus << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "otdr get status error. " << iSta << std::endl;
        std::cout << std::endl;
    }

    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().StartScan(0, DEV_TYPE_OTDR))) {
        std::cout << "otdr start scan ok. " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "otdr start scan error. " << iSta << std::endl;
        std::cout << std::endl;
    }


    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().GetStatusData(0, stStatus))) {
        std::cout << "otdr get status ok. " << iSta << std::endl;
        std::cout << "otdr scan status " << stStatus.ulScanStatus << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "otdr get status error. " << iSta << std::endl;
        std::cout << std::endl;
    }

    sleep(30);
    if (OPLK_OK == (iSta = CDevApiManager::GetInstance().GetOtdrApi().GetStatusData(0, stStatus))) {
        std::cout << "otdr get status ok. " << iSta << std::endl;
        std::cout << "otdr scan status " << stStatus.ulScanStatus << std::endl;
        std::cout << "event cnt " << stStatus.stEventTop.ulCnt << std::endl;
        std::cout << "event dFiberLen " << stStatus.stEventTop.dFiberLen << std::endl;
        std::cout << "event dTotalLoss " << stStatus.stEventTop.dTotalLoss << std::endl;
        std::cout << "event dReturnLoss " << stStatus.stEventTop.dReturnLoss << std::endl;
    } else {
        std::cout << "otdr get status error. " << iSta << std::endl;
    }

}
#if 0
enum OaMode
{
    POWER = 0,
    AGC,
    APC,

};

std::map<uint32_t, std::string> OaMode = {{POWER, "POWER"},
                                          {AGC, "AGC"},
                                          {APC, "APC"}};
#else
enum OaMode
{
    ACC = 0,
    APC,
    AGC,

};
std::map<uint32_t, std::string> OaMode = {{ACC, "ACC"},
                                          {APC, "APC"},
                                          {AGC, "AGC"}};
#endif

void
__OaStatusData(uint32_t uiIdx, uint32_t uiSubIdx)
{
    __StartTime();

    COaStatusData stEdfaStatus;
    int32_t iSta;
    std::string staInService = (stEdfaStatus.stOaCurData.bIsInService ? "In-Service" : "Out-Of-Service");
    std::string staMode = OaMode[stEdfaStatus.stOaCurData.ucModeActual];

    memset(&stEdfaStatus, 0x00, sizeof(stEdfaStatus));
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiIdx, stEdfaStatus, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " get oa data fail " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " :" << std::endl;
        std::cout << "gain value: " << (int16_t)stEdfaStatus.stOaCurData.sGainActual << std::endl;
        std::cout << "tilt value: " << (int16_t)stEdfaStatus.stOaCurData.sTiltActual << std::endl;
        /* std::cout << "bIsInService: " << staInService << std::endl; */
        std::cout << "mode value: " << staMode << std::endl;
        std::cout << "input power:" << stEdfaStatus.stOaCurData.sInPower << std::endl;
        std::cout << "sig out power:" << stEdfaStatus.stOaCurData.sOutSigCPower << std::endl;
        std::cout << "output power:" << stEdfaStatus.stOaCurData.sOutTotalPower << std::endl;
        std::cout << "asPumpTemp:" << stEdfaStatus.stOaCurData.asPumpTemp[0] << std::endl;
        std::cout << "CaseTmep:" << stEdfaStatus.stOaCurData.asCaseTemp[0] << std::endl;
        std::cout << "PumpTempAlarm:" << (uint32_t)stEdfaStatus.stOaAlm.abPumpTemp[0] << std::endl;
        std::cout << "CaseTmepAlarm:" << (uint32_t)stEdfaStatus.stOaAlm.bCaseTemp << std::endl;
        std::cout << "asPumpCurrent:" << stEdfaStatus.stOaCurData.asPumpCurrent[0] << std::endl;
        std::cout << "voa 0:" << stEdfaStatus.stOaCurData.asVoaAttnActual[0] << std::endl;
    }

    __StopTime();
}


void
__OaSetGain(uint32_t uiIdx, uint32_t uiGain, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetGain(uiIdx, uiGain, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set gain error. uiGain = " << uiGain << " iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set gain done. uiGain = " << uiGain << std::endl;
    }
    __StopTime();
}

void
__OaSetAttn(uint32_t uiIdx, uint32_t uiAttn, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetAttn(uiIdx, uiAttn, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set attn error. uiAttn = " << uiAttn << " iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set attn done. uiAttn = " << uiAttn << std::endl;
    }
    __StopTime();
}

void
__OaSetTilt(uint32_t uiIdx, int32_t iTilt, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();

    std::cout << "Set Tilt = " << iTilt << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetTilt(uiIdx, iTilt, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set tilt error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set tilt done. " << std::endl;
    }
    __StopTime();
}

void
__OaSetAprEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Apr Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetAprEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set apr error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set apr done. " << std::endl;
    }
    __StopTime();
}

void
__OaSetPumpEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Pump Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetPumpEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set pump error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set pump done." << std::endl;
    }
    __StopTime();
}

void
__OaSetAutoLosEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set AutoLos Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetAutolosEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set autolos error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set autolos done." << std::endl;
    }
    __StopTime();

    return;
}

void
__OaSetAutoLosThr(uint32_t uiIdx, int32_t iThr, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetRxLosThr(uiIdx, iThr, iHys, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " __OaSetAutoLosThr error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " __OaSetAutoLosThr done." << std::endl;
    }
    __StopTime();
}


void
__OaSetMode(uint32_t uiIdx, uint32_t uiMode, uint32_t uiVal, uint32_t uiSubIdx)
{
    int32_t iSta;
    std::string staMode = OaMode[uiMode];

    __StartTime();
    std::cout << "Set Mode = " << staMode << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetMode(uiIdx, uiMode, uiVal, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set mode error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set mode done." << std::endl;
    }
    __StopTime();
}

void
__OaSetDfbEnable(uint32_t uiIdx, bool bEnable, int32_t iPower, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Pump Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetDfbEnable(uiIdx, bEnable, iPower, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set dfb error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set dfb done." << std::endl;
    }
    __StopTime();
}

void
__OaSetOscEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Pump Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetOscEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set osc error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set osc done." << std::endl;
    }
    __StopTime();
}

void
__OaSetOtdrSwitchPos(uint32_t uiIdx, uint32_t uiPos, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    std::cout << "Set Otdr SwitchPos = " << uiPos << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetOtdrSwitchPos(uiIdx, uiPos, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set otdr switchpos error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set otdr switchpos done." << std::endl;
    }
    __StopTime();
}

void
__OaSetOscAddThrHys(uint32_t uiIdx, int32_t iThr, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetOscAddThrHys(uiIdx, iThr, iHys, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " __OaSetOscAddThrHys error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " __OaSetOscAddThrHys done." << std::endl;
    }
    __StopTime();
}

void
__OaSetOscDropThrHys(uint32_t uiIdx, int32_t iThr, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetOscDropThrHys(uiIdx, iThr, iHys, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " __OaSetOscDropThrHys error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " __OaSetOscDropThrHys done." << std::endl;
    }
    __StopTime();
}

void
__OaSetOscRxThrHys(uint32_t uiIdx, int32_t iThr, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetOscRxThrHys(uiIdx, iThr, iHys, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " __OaSetOscRxThrHys error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " __OaSetOscRxThrHys done." << std::endl;
    }
    __StopTime();
}

void
__OaSetApsdEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Pump Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetApsdEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set apsd error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set apsd done." << std::endl;
    }
    __StopTime();
}


void
__OaSetUpgState(uint32_t uiIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    std::cout << "Set UpgState = " << uiState << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetUpgState(uiIdx, uiState, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set __OaSetUpgState error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set __OaSetUpgState done." << std::endl;
    }
    __StopTime();
}

void
__OaSetLedState(uint32_t uiIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    std::cout << "Set LedState = " << uiState << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetLedState(uiIdx, uiState, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set __OaSetLedState error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set __OaSetLedState done." << std::endl;
    }
    __StopTime();
}

void
__OaSetLedAlm(uint32_t uiIdx, uint32_t uiState, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    std::cout << "Set __OaSetLedAlm = " << uiState << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetLedAlm(uiIdx, uiState, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set __OaSetLedAlm error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set __OaSetLedAlm done." << std::endl;
    }
    __StopTime();
}

void
__OaReset(uint32_t uiIdx, uint32_t uiResetType)
{
    int32_t iSta;
    std::string staResetType = (uiResetType == RST_MODE_COLD ? "Cold" : "Warm");

    std::cout << staResetType << " Reset." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().Reset(uiIdx, uiResetType)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " reset error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " reset done." << std::endl;
    }

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOaApi().GetUpgadeStatus(uiIdx);
        /* std::cout << iSta << std::endl; */

        CDelay::Delay(1, S);
        continue;

    }

}

void
__OaUpgrade(uint32_t uiIdx, const char *c_pcFilePath)
{
    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetOaApi().Upgrade(uiIdx, c_pcFilePath)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " upgrade error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " upgrade done." << std::endl;
    }


#if 0
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().Upgrade(uiIdx, "/tmp/edfa_fpga_0.07.0001.bin")) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " upgrade error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " upgrade done." << std::endl;
    }
#endif

    CDelay::Delay(3, S);

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOaApi().GetUpgadeStatus(uiIdx);
        std::cout << "OA" << uiIdx << " iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }

}

void
__OaSwap(uint32_t uiIdx)
{
    int32_t iSta;

    while (1) {
        if ((iSta = CDevApiManager::GetInstance().GetOaApi().Swap(uiIdx)) != OPLK_OK) {
            std::cout << "OA" << uiIdx << " swap error." << iSta << std::endl;
            CDelay::Delay(1, S);
            continue;

        } else {
            /* std::cout << "OA" << uiIdx << " swap done." << std::endl; */
            break;
        }
    }
}

void
__OaCommit(uint32_t uiIdx)
{
    int32_t iSta;

    while (1) {
        if ((iSta = CDevApiManager::GetInstance().GetOaApi().Commit(uiIdx)) != OPLK_OK) {
            std::cout << "OA" << uiIdx << " commit error." << iSta << std::endl;
            CDelay::Delay(1, S);
            continue;

        } else {
            std::cout << "OA" << uiIdx << " commit done." << std::endl;
            break;
        }
    }
}

void
__OaGetMfg(uint32_t uiIdx, uint32_t uiSubIdx)
{
    CMfg stMfg;
    int32_t iSta;

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetMfg(uiIdx, stMfg)) != OPLK_OK) {
        std::cout << "OA: " << uiIdx << " get mfg err. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA: " << uiIdx << std::endl;
        std::cout << "SN: " << stMfg.acSN << std::endl;
        /* std::cout << "PN: " << stMfg.acPN << std::endl; */
        /* std::cout << "Mod Type: " << stMfg.acModuleType << std::endl; */
        /* std::cout << "Boot Ver: " << stMfg.acBootVer << std::endl; */
        std::cout << "Main Ver: " << stMfg.acFwVer << std::endl;
        std::cout << "acFpgaRunVer: " << stMfg.acFpgaRunVer << std::endl;
        /* std::cout << "HW Ver: " << stMfg.acHwVer << std::endl; */
        /* std::cout << "Cali Date: " << stMfg.acCaliDate << std::endl; */
        std::cout << std::endl;
    }
}


void
__OaUpgradeAndReset(uint32_t uiIdx, char *pcFpgaFilePath, char *pcMcuFilePath)
{
    __OaGetMfg(uiIdx, 0);

    __StartTime();

    __OaUpgrade(uiIdx, pcFpgaFilePath);
    /* __OaUpgrade(uiIdx, pcMcuFilePath); */

    __OaSwap(uiIdx);

    CDelay::Delay(1, S);

    __OaReset(uiIdx, RST_MODE_WARM);

    CDelay::Delay(90, S);

    __OaCommit(uiIdx);
    CDelay::Delay(1, S);

    __StopTime();

    __OaGetMfg(uiIdx, 0);
}



void
__OaGetCfg(uint32_t uiIdx)
{
    COaCfgData stOaCfg;
    int32_t iSta;

    memset(&stOaCfg, 0x00, sizeof(stOaCfg));
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetCfgData(uiIdx, stOaCfg)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " get oa data fail " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << std::endl;
        std::cout << "Gain: " << stOaCfg.sGain << std::endl;
        std::cout << "Tilt: " << stOaCfg.sTilt << std::endl;
    }
}

void
__OaSetTest()
{

#if 0
    std::cout << std::endl << std::endl;

    __OaSetGain(PA_IDX, 2000, 0);
    __OaSetMode(PA_IDX, AGC, 2000, 0);
    __OaSetAttn(PA_IDX, 700, 2);
    __OaSetTilt(PA_IDX, -150, 0);


    __OaSetAutoLosEnable(PA_IDX, true, 0);
    __OaSetPumpEnable(PA_IDX, false, 0);
    __OaSetAprEnable(PA_IDX, true, 0);
    __OaSetAutoLosThr(PA_IDX, -20, 1, 0);

    __OaStatusData(PA_IDX, 0);


    __OaSetGain(BA_IDX, 2000, 0);
    __OaSetMode(BA_IDX, AGC, 2000, 0);
    __OaSetAttn(BA_IDX, 700, 2);
    __OaSetTilt(BA_IDX, -150, 0);


    __OaSetAutoLosEnable(BA_IDX, true, 0);
    __OaSetPumpEnable(BA_IDX, false, 0);
    __OaSetAprEnable(BA_IDX, true, 0);
    __OaSetAutoLosThr(PA_IDX, -20, 1, 0);

    __OaStatusData(BA_IDX, 0);


#if 0

    __OaSetAutoLosEnable(PA_IDX, false, 0);
    CDelay::Delay(1, S);
    __OaStatusData(PA_IDX, 0);


    __OaSetPumpEnable(PA_IDX, true, 0);
    CDelay::Delay(1, S);
    __OaStatusData(PA_IDX, 0);

    __OaSetGain(PA_IDX, 2000, 0);
    __OaSetMode(PA_IDX, AGC, 2000, 0);
    __OaSetGain(PA_IDX, 2000, 0);
    __OaSetAttn(PA_IDX, 700, 2);
    __OaSetTilt(PA_IDX, -150);
    __OaSetAprEnable(PA_IDX, true);
    CDelay::Delay(1, S);
    __OaStatusData(PA_IDX, 0);


    /* __OaUpgrade(PA_IDX, "/tmp/edfa_pa_ba_sw_0.01.0001.bin"); */

    __OaSetMode(BA_IDX, APC, 500);
    __OaSetAutoLosEnable(BA_IDX, true, 0);
    __OaSetPumpEnable(BA_IDX, false, 0);
    CDelay::Delay(1, S);
    __OaStatusData(BA_IDX, 0);

    __OaSetAutoLosEnable(BA_IDX, false, 0);
    CDelay::Delay(1, S);
    __OaStatusData(BA_IDX, 0);

    __OaSetPumpEnable(BA_IDX, true, 0);
    CDelay::Delay(1, S);
    __OaStatusData(BA_IDX, 0);

    __OaSetGain(BA_IDX, 2000, 0);
    __OaSetMode(BA_IDX, AGC, 2000);
    __OaSetGain(BA_IDX, 2000, 0);
    __OaSetAttn(BA_IDX, 600, 2);
    __OaSetTilt(BA_IDX, -150);
    __OaSetAprEnable(BA_IDX, true);
    CDelay::Delay(1, S);
    __OaStatusData(BA_IDX, 0);


    /* __OaReset(PA_IDX, RST_MODE_WARM); */
    __OaReset(PA_IDX, RST_MODE_COLD);


    /* __OaReset(BA_IDX, RST_MODE_WARM); */
    __OaReset(BA_IDX, RST_MODE_COLD);
    /* __OaUpgrade(BA_IDX, "/tmp/edfa_pa_ba_sw_0.01.0002.bin"); */
#endif
#endif


}

void
__OaAliTest()
{
#if 0
    __OaSetAutoLosEnable(PA_IDX, true, 0);

    __OaSetAutoLosEnable(PA_IDX, true, 1);
#endif

#if 0

    __OaSetAutoLosThr(PA_IDX, -2000, -1000, 0);

    __OaSetAutoLosThr(PA_IDX, -3000, -1000, 1);


    __OaSetAttn(PA_IDX, 100, 0);

    __OaSetAttn(PA_IDX, 500, 1);


    __OaSetPumpEnable(PA_IDX, true, 0);

    __OaSetPumpEnable(PA_IDX, true, 1);


    __OaStatusData(PA_IDX, 0);

    __OaStatusData(PA_IDX, 1);
#endif
}

void
__OaAllTest()
{
    std::cout << std::endl << std::endl;

    __OaGetMfg(PA_IDX, 0);
    __OaStatusData(PA_IDX, 0);

    __OaSetGain(PA_IDX, 2000, 0);
    __OaSetMode(PA_IDX, AGC, 2000, 0);
    __OaSetAttn(PA_IDX, 700, 2);
    __OaSetTilt(PA_IDX, -150, 0);

    __OaSetAutoLosEnable(PA_IDX, true, 0);
    __OaSetPumpEnable(PA_IDX, false, 0);
    __OaSetAprEnable(PA_IDX, true, 0);
    __OaSetAutoLosThr(PA_IDX, -20, 1, 0);

    __OaStatusData(PA_IDX, 0);

    __OaGetMfg(BA_IDX, 0);
    __OaStatusData(BA_IDX, 0);

    __OaSetGain(BA_IDX, 2000, 0);
    __OaSetMode(BA_IDX, AGC, 2000, 0);
    __OaSetAttn(BA_IDX, 700, 2);
    __OaSetTilt(BA_IDX, -150, 0);

    __OaSetAutoLosEnable(BA_IDX, true, 0);
    __OaSetPumpEnable(BA_IDX, false, 0);
    __OaSetAprEnable(BA_IDX, true, 0);
    __OaSetAutoLosThr(PA_IDX, -20, 1, 0);

    __OaStatusData(BA_IDX, 0);

}

void
__OaCommFail(uint32_t uiDevIdx)
{
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().DbgSetCommFail(uiDevIdx);

    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " DbgSetCommFail error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " DbgSetCommFail done." << std::endl;
    }
}

void
__OaClearCommFail(uint32_t uiDevIdx)
{
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().DbgClearCommFail(uiDevIdx);

    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " DbgClearCommFail error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " DbgClearCommFail done." << std::endl;
    }
}

void
__OaModuleAlarm(uint32_t uiDevIdx)
{
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().DbgSetModuleAlarm(uiDevIdx);

    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " DbgSetModuleAlarm error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " DbgSetModuleAlarm done." << std::endl;
    }
}

void
__OaClearModuleAlarm(uint32_t uiDevIdx)
{
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().DbgClearModuleAlarm(uiDevIdx);

    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " DbgClearModuleAlarm error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " DbgClearModuleAlarm done." << std::endl;
    }
}

void
__OaDebugInfo(uint32_t uiDevIdx)
{
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().GetDebugInfo(uiDevIdx, "/tmp/debug-info-ba");

    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " __OaDebugInfo error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " __OaDebugInfo done." << std::endl;
    }
}

int32_t
__OaDebug(uint32_t uiDevIdx, uint8_t *pucBuf, uint32_t uiLen)
{
    /* std::string staMsg = "adr2862\r\n"; */
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().DbgWrite(uiDevIdx, pucBuf, uiLen);

    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " DbgWrite error." << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " DbgWrite done." << std::endl;
    }

    return iSta;
}

void
__OaPumpMax(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
#if 0
    std::string staMsg = "adr2862\r\n";
    int32_t iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "adr2862 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 20403c 1\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 20403c 1 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "mode m\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "mode m error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204000 3f\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204000 3f error." << std::endl;
    }

    staMsg = "wfpga 204208 3fff\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204208 3fff error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204408 3fff\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204408 3fff error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204508 3fff\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204508 3fff error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "fpga 20420c 4\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "fpga 20420c 4 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "fpga 20440c 4\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "fpga 20420c 4 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "fpga 20450c 4\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "fpga 20420c 4 error." << std::endl;
    }
#endif
    int32_t iSta;

    __StartTime();
    std::cout << "SetPumpEnable = " << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetPumpEnable(uiDevIdx, true, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " set SetPumpEnable error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " set SetPumpEnable done." << std::endl;
    }
    __StopTime();

}

void
__OaPumpOff(uint32_t uiDevIdx, uint32_t uiSubIdx)
{
#if 0
    std::string staMsg = "adr2862\r\n";
    int32_t iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "adr2862 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 20403c 1\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 20403c 1 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "mode m\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "mode m error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204000 3f\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204000 3f error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204208 0\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204208 3fff error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204408 0\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204408 3fff error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "wfpga 204508 0\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "wfpga 204508 3fff error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "fpga 20420c 4\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "fpga 20420c 4 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "fpga 20440c 4\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "fpga 20420c 4 error." << std::endl;
    }

    CDelay::Delay(100, MS);

    staMsg = "fpga 20450c 4\r\n";
    iSta = __OaDebug(uiDevIdx, (uint8_t *)staMsg.c_str(), staMsg.length());
    if (iSta != OPLK_OK) {
        std::cout << "fpga 20420c 4 error." << std::endl;
    }
#endif
    int32_t iSta;

    __StartTime();
    std::cout << "Set LedState = " << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetPumpEnable(uiDevIdx, false, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiDevIdx << " set __OaPumpOff error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiDevIdx << " set __OaPumpOff done." << std::endl;
    }
    __StopTime();

}

void
__OaSetManualVoaEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set ManualVoa Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetManualVoaEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set osc error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set osc done." << std::endl;
    }
    __StopTime();
}

void
__OaSetPaddingVoaConfigPower(uint32_t uiIdx, uint32_t uiPower, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetPaddingVoaConfigPower(uiIdx, uiPower, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set attn error. uiPower = " << uiPower << " iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set attn done. uiPower = " << uiPower << std::endl;
    }
    __StopTime();
}

void
__OaSetManualGainEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set ManualGain Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetManualGainEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set osc error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set osc done." << std::endl;
    }
    __StopTime();
}

void
__OaSetThr(uint32_t uiIdx, uint32_t uiPortIdx, uint32_t uiType, int32_t iThr, int32_t iHys, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetThrHys(uiIdx, uiPortIdx, uiType, iThr, iHys, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " __OaSetThr error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " __OaSetThr done." << std::endl;
    }
    __StopTime();
}

void
__OaSetPumpCurrent(uint32_t uiIdx, uint32_t uiPumpIdx, int32_t iCurrent, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();

    std::cout << "Set Pump" << uiPumpIdx << " Current = " << iCurrent << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetPumpCurrent(uiIdx, uiPumpIdx, iCurrent, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set iCurrent error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set iCurrent done. " << std::endl;
    }
    __StopTime();
}

void
__OaSetAlmMask(uint32_t uiIdx, int32_t iMaskIdx, int32_t iMaskVal, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    std::cout << "Set AlmMask = " << iMaskIdx << iMaskVal << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetAlmMask(uiIdx, iMaskIdx, iMaskVal, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set alm mask error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set alm mask done." << std::endl;
    }
    __StopTime();
}

void
__OaShutter(uint32_t uiIdx, uint32_t uiSubIdx)
{
    __StartTime();

    bool block[] = {false, true};
    for (size_t i = 0; i < sizeof(block) / sizeof(bool); i++) {
        std::cout << "set shutter block = " << (block[i] ? "true" : "false") << std::endl;
        int32_t iSta = CDevApiManager::GetInstance().GetOaApi().SetShutterState(uiIdx, block[i], uiSubIdx);
        if (iSta != OPLK_OK) {
            std::cout << "OA" << uiIdx << " set shutter error. iSta = " << iSta << std::endl;
        } else {
            std::cout << "OA" << uiIdx << " set shutter ok" << std::endl;
        }

        std::cout << "get shutter status" << std::endl;
        iSta = CDevApiManager::GetInstance().GetOaApi().GetShutterState(uiIdx, block[i], uiSubIdx);
        if (iSta != OPLK_OK) {
            std::cout << "OA" << uiIdx << " get shutter error. iSta = " << iSta << std::endl;
        } else {
            std::cout << "OA" << uiIdx << " get shutter, block = " << (block[i] ? "true" : "false") << std::endl;
        }
    }

    __StopTime();
}

void
__OaTrig(uint32_t uiIdx, uint32_t uiSubIdx)
{
    __StartTime();

    std::cout << "Set Trig " << std::endl;
    int32_t iSta = CDevApiManager::GetInstance().GetOaApi().SetReadyTrigger(uiIdx, 1, uiSubIdx);
    if (iSta != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set trig error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set trig ok" << std::endl;
    }

    __StopTime();
}

void
__OaSetFsmMode(uint32_t uiIdx, uint32_t uiMode, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    std::cout << "Set Fsm Mode = " << uiMode << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetAutogainMode(uiIdx, uiMode, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set fsm mode error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set fsm mode done." << std::endl;
    }
    __StopTime();
}

void
__OaSetRomoteInterlockEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Remote interlock Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetRemoteInterlockEnable(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set remote interlock error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set remote interlock done." << std::endl;
    }
    __StopTime();
}

void
__OaSetDummyAdminkeyEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Dummy Adminkey Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetDummyAdminKey(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set dummy adminkey error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set dummy adminkey done." << std::endl;
    }
    __StopTime();
}

void
__OaSetPanelSafeCmptEnable(uint32_t uiIdx, bool bEnable, uint32_t uiSubIdx)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "TRUE" : "FALSE";

    __StartTime();
    std::cout << "Set Panel Safe Cmpt Enable = " << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetPanelSafeCmpt(uiIdx, bEnable, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set panel safe cmpt error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set panel safe cmpt done." << std::endl;
    }
    __StopTime();
}

void
__OaSetReadyPower(uint32_t uiIdx, uint32_t uiPower, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetReadyPower(uiIdx, uiPower, uiSubIdx)) != OPLK_OK) {
        std::cout << "OA" << uiIdx << " set ready power error. uiPower = " << uiPower << " iSta = " << iSta << std::endl;
    } else {
        std::cout << "OA" << uiIdx << " set ready power done. uiPower = " << uiPower << std::endl;
    }
    __StopTime();
}

void
__OaTest(char *argv[])
{
    uint32_t uiAttn = 0;
    int32_t iTilt = 0;
    uint32_t iGain = 0;
    bool bEnable = 0;
    uint32_t uiSubIdx = 0;
    int32_t iThr = 0;
    int32_t iHys = 0;
    uint32_t uiMode = 0;
    uint32_t uiVal = 0;
    uint32_t uiPos = 0;
    uint32_t uiSta = 0;
    int32_t iPower = 0;
    int32_t uiPumpIdx = 0;
    uint32_t uiPortIdx = 0;
    uint32_t uiType = 0;
    int32_t iMaskIdx = 0;
    int32_t iMaskVal = 0;

    std::vector<std::string> staCmdVec = {"mfg", "data", "comm-fail", "clear-comm-fail", "upgrade",
                                          "module-alarm", "clear-module-alarm", "debug-info", "reset",
                                          "pump-max", "pump-off",
                                          "attn", "gain", "tilt", "apr", "pump", "al", "althr", "mode",
                                          "dfb", "osc", "otdrpos", "oscaddthr", "oscdropthr", "oscrxthr", "apsd", "upgsta", "ledsta", "ledalm",
                                          "voaman", "ppower", "gainman", "current", "thr", "all", "mask", "shutter",
                                          "trig", "fsm", "interlock", "dummyadmin", "panel", "readypower"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);

    uint32_t uiDevIdx = atoi(argv[3]);

    switch (iCmdIdx) {
    case 0:
        __OaGetMfg(uiDevIdx, 0);
        break;

    case 1:
        __OaStatusData(uiDevIdx, 0);
        break;

    case 2:
        __OaCommFail(uiDevIdx);
        break;

    case 3:
        __OaClearCommFail(uiDevIdx);
        break;

    case 4:
        __OaUpgradeAndReset(uiDevIdx, argv[4], argv[5]);
        break;

    case 5:
        __OaModuleAlarm(uiDevIdx);
        break;

    case 6:
        __OaClearModuleAlarm(uiDevIdx);
        break;

    case 7:
        __OaDebugInfo(uiDevIdx);
        break;

    case 8:
        __OaReset(uiDevIdx, RST_MODE_WARM);
        break;

    case 9:
        uiSubIdx = atoi(argv[4]);
        __OaPumpMax(uiDevIdx, uiSubIdx);
        break;

    case 10:
        uiSubIdx = atoi(argv[4]);
        __OaPumpOff(uiDevIdx, uiSubIdx);
        break;

    case 11:
        /* ./api_main oa 0 attn 5 0 */
        uiAttn = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetAttn(uiDevIdx, uiAttn, uiSubIdx);
        break;

    case 12:
        /* ./api_main oa gain 0 300 0  done*/
        iGain = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetGain(uiDevIdx, iGain, uiSubIdx);
        break;

    case 13:
        /* ./api_main oa tilt 0 100 0  done*/
        iTilt = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetTilt(uiDevIdx, iTilt, uiSubIdx);
        break;

    case 14:
        /* ./api_main oa apr 0 1 0  done*/
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetAprEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 15:
        /* ./api_main oa  pump 0 1 0   done*/
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetPumpEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 16:
        /* ./api_main oa al 0 1 0  done*/
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetAutoLosEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 17:
        /* ./api_main oa althr 0 100 1 0  done*/
        iThr = atoi(argv[4]);
        iHys = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetAutoLosThr(uiDevIdx, iThr, iHys, uiSubIdx);
        break;

    case 18:
        /* ./api_main oa mode 0 2 200 0  done*/
        uiMode = atoi(argv[4]);
        uiVal = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetMode(uiDevIdx, uiMode, uiVal, uiSubIdx);
        break;

    case 19:
        /* api_main oa dfb 0 1 2 0 */
        bEnable = atoi(argv[4]);
        iPower = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetDfbEnable(uiDevIdx, bEnable, iPower, uiSubIdx);
        break;

    case 20:
        /* api_main oa  osc 0 1 0 */
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetOscEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 21:
        /* api_main oa  otdrpos 0 1 0 */
        uiPos = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetOtdrSwitchPos(uiDevIdx, uiPos, uiSubIdx);
        break;

    case 22:
        /* api_main oa oscaddthr 0 3 1 0 */
        iThr = atoi(argv[4]);
        iHys = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetOscAddThrHys(uiDevIdx, iThr, iHys, uiSubIdx);
        break;

    case 23:
        /* api_main oa oscdropthr 0 3 1 0 */
        iThr = atoi(argv[4]);
        iHys = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetOscDropThrHys(uiDevIdx, iThr, iHys, uiSubIdx);
        break;

    case 24:
        /* api_main oa oscrxthr 0 3 1 0 */
        iThr = atoi(argv[4]);
        iHys = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetOscRxThrHys(uiDevIdx, iThr, iHys, uiSubIdx);
        break;

    case 25:
        /* api_main oa apsd 0 1 0 */
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetApsdEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 26:
        /* api_main oa	upgsta 0 1 0 */
        uiSta = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetUpgState(uiDevIdx, uiSta, uiSubIdx);
        break;

    case 27:
        /* api_main oa	ledsta 0 1 0 */
        uiSta = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetLedState(uiDevIdx, uiSta, uiSubIdx);
        break;

    case 28:
        /* api_main oa	ledalm 0 1 0 */
        uiSta = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetLedAlm(uiDevIdx, uiSta, uiSubIdx);
        break;

    case 29:
        /* ./api_main oa voaman 0 0 1/./api_main oa voaman 0 1 1 */
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetManualVoaEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 30:
        /* ./api_main oa 0 ppower 5 0 */
        iPower = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetPaddingVoaConfigPower(uiDevIdx, iPower, uiSubIdx);
        break;

    case 31:
        /* api_main oa  gainman 0 1 0 */
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetManualGainEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 32:
        /* api_main oa  current 0 1 200 0 */
        uiPumpIdx = atoi(argv[4]);
        uiVal = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetPumpCurrent(uiDevIdx, uiPumpIdx, uiVal, uiSubIdx);
        break;

    case 33:
        /* api_main oa thr 0 0 0 300 100 0 */
        uiPortIdx = atoi(argv[4]);
        uiType = atoi(argv[5]);
        iThr = atoi(argv[6]);
        iHys = atoi(argv[7]);
        uiSubIdx = atoi(argv[8]);
        __OaSetThr(uiDevIdx, uiPortIdx, uiType, iThr, iHys, uiSubIdx);
        break;

    case 34:
        /* api_main oa all get and set test*/
        __OaAllTest();
        break;

    case 35:
        /* ./api_main oa mask 0 2 200 0  done*/
        iMaskIdx = atoi(argv[4]);
        iMaskVal = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OaSetAlmMask(uiDevIdx, iMaskIdx, iMaskVal, uiSubIdx);
        break;

    case 36:
        __OaShutter(uiDevIdx, 0);
        break;

    case 37:
        uiSubIdx = atoi(argv[4]);
        __OaTrig(uiDevIdx, uiSubIdx);
        break;

    case 38:
        uiMode = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetFsmMode(uiDevIdx, uiMode, uiSubIdx);
        break;

    case 39:
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetRomoteInterlockEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 40:
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetDummyAdminkeyEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 41:
        bEnable = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetPanelSafeCmptEnable(uiDevIdx, bEnable, uiSubIdx);
        break;

    case 42:
        /* ./api_main oa ready power 0 700 0  done*/
        iPower = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OaSetReadyPower(uiDevIdx, iPower, uiSubIdx);
        break;


    default:
        std::cout << "Unknown Command." << std::endl;
    }

    return;

}

void
__OcmMfg()
{
    CMfg stMfg;
    /* COaStatusData stEdfaStatus; */
    /* COaCfgData stOaCfg; */
    int32_t iSta;

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetOcmApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "OCM: get mfg err. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "OCM MFG: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }


    iSta = CDevApiManager::GetInstance().GetOcmApi().GetUpgadeStatus(0);
    std::cout << "Ocm iSta = " << iSta << std::endl;
}

void
__OcmSetWavePlan()
{
#if 1

    int iPort = 5;
    int iChannelCnt = 65;
    uint32_t uiStartFreq = 191262.5 / 6.25;
    uint16_t usSliceCount = 12;
    CWavePlanSlice astWaveplan[65];
    int rc = 0;

    for (int i = 0; i < iChannelCnt; i++) {
        astWaveplan[i].ucChId = i + 1;
        astWaveplan[i].usStartSlice = uiStartFreq + (i * usSliceCount);
        astWaveplan[i].usEndSlice = astWaveplan[i].usStartSlice + usSliceCount - 1;
        printf("%d %d %d\n",
               astWaveplan[i].ucChId,
               astWaveplan[i].usStartSlice,
               astWaveplan[i].usEndSlice);
    }

    for (int i = 1; i <= iPort; i++) {
        rc = CDevApiManager::GetInstance().GetOcmApi().SetWavePlan(0, i, iChannelCnt, astWaveplan);
        if (rc != OPLK_OK) {
            std::cout << "set waveplan error. rc = " << rc << std::endl;
            std::cout << std::endl;
        } else {
            std::cout << "set waveplan ok." << std::endl;
            std::cout << std::endl;
        }
    }
#endif
}

void
__OcmSetOcmWavePlan()
{

    int iPort = 1;
    int iChannelCnt = 194;
    uint32_t uiStartFreq = 191300;  /* 191262.5 + 6.25 * 6 */
    /* uint16_t usSliceCount = 12; */
    int rc = 0;
    STChannelGrid *pstOcmFlexgridChan = NULL;
    int i = 0;
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "waveplan count: %d", iPort);
    printf("waveplan count: %d\n", iPort);
    pstOcmFlexgridChan = (STChannelGrid *)malloc(sizeof(STChannelGrid) * iChannelCnt);
    if (NULL == pstOcmFlexgridChan) {
        LOG_WRITE(EN_LOG_ERROR, "Not enough memory");
        printf("Not enough memory\n");
        goto cleanup;
    }
#endif
    #define CHANNEL_NUMBER_THRESHOLD               194
    STChannelGrid astChannelPlan[CHANNEL_NUMBER_THRESHOLD];
    pstOcmFlexgridChan = astChannelPlan;

    memset(pstOcmFlexgridChan, 0x00, sizeof(STChannelGrid) * iChannelCnt);
    for (i = 0; i < 65; i++) {
                #if 0
        pstOcmFlexgridChan[i].uCentralFreq = (uint32_t)(uiStartFreq * 1000 + i * 75 * 1000 + 37500);    /* GHz to MHz */
        pstOcmFlexgridChan[i].uBandwidth = 75 * 1000;        /* GHz to MHz */
                #endif
        pstOcmFlexgridChan[i].uCentralFreq = (uint32_t)(uiStartFreq * 1000 + i * 50 * 1000);  /* GHz to MHz */
        pstOcmFlexgridChan[i].uBandwidth = 25 * 1000;      /* GHz to MHz */
    }

    for ( i = 1; i <= iPort; i++) {
        printf("i=%d\n", i);
        rc = CDevApiManager::GetInstance().GetOcmApi().SetOcmWavePlan(0, i, iChannelCnt, astChannelPlan);
        if (rc != OPLK_OK) {
            std::cout << "set waveplan error. rc = " << rc << std::endl;
            std::cout << std::endl;
        } else {
            std::cout << "set waveplan ok." << std::endl;
            std::cout << std::endl;
        }
    }
#if 0
cleanup:
    DEF_SAFE_FREE(pstOcmFlexgridChan);
#endif
}


void
__OcmReset(uint32_t uiResetType)
{
    /* int32_t iSta; */

    CDevApiManager::GetInstance().GetOcmApi().Reset(0, uiResetType);
#if 0
    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOcmApi().GetUpgadeStatus(0);
        std::cout << "Ocm iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }
#endif
}

void
__OcmCommit(uint32_t uiIdx)
{
    int32_t iSta;

    while (1) {
        if ((iSta = CDevApiManager::GetInstance().GetOcmApi().Commit(uiIdx)) != OPLK_OK) {
            std::cout << "OCM" << uiIdx << " commit error." << iSta << std::endl;
            CDelay::Delay(1, S);
            continue;

        } else {
            std::cout << "OCM" << uiIdx << " commit done." << std::endl;
            break;
        }
    }
}

void
__OcmUpgrade(char *pcFilePath)
{
    int32_t iSta;
#if 1
#if 1
    /* CDevApiManager::GetInstance().GetOcmApi().Upgrade(0, "/home/oplink/log/ocm_0.01.0002.bin"); */
    if ( NULL == pcFilePath) {
        std::cout << "Ocm upgrade fail. NULL == pcFilePath" << std::endl;
    }
    std::cout << "Ocm upgrade start, pcFilePath" << pcFilePath << std::endl;
    CDevApiManager::GetInstance().GetOcmApi().Upgrade(0, pcFilePath);

    /* CDelay::Delay(20, S); */
    for (int i = 0; i < 30; i++) {
        CDelay::Delay(1, S);
        std::cout << "Ocm sleep" << i << std::endl;
    }
#endif

    std::cout << "Ocm reset" << std::endl;
    __OcmReset(RST_MODE_WARM);
    for (int i = 0; i < 30; i++) {
        CDelay::Delay(1, S);
        std::cout << "Ocm sleep" << i << std::endl;
    }
    std::cout << "Ocm commit" << std::endl;
    __OcmCommit(0);
    CDelay::Delay(1, S);

#else

    CDevApiManager::GetInstance().GetOcmApi().Upgrade(0, "/home/oplink/log/ocm_9.01.0001.bin");
#endif

    CDelay::Delay(3, S);

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOcmApi().GetUpgadeStatus(0);
        std::cout << "Ocm iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }

    /* __OcmReset(RST_MODE_COLD); */

    __OcmMfg();
}

void
__OcmCommFail()
{
    int32_t iSta = CDevApiManager::GetInstance().GetOcmApi().DbgSetCommFail(0);

    if (iSta != OPLK_OK) {
        std::cout << "Ocm DbgSetCommFail error." << iSta << std::endl;
    } else {
        std::cout << "Ocm DbgSetCommFail done." << std::endl;
    }
}

void
__OcmClearCommFail()
{
    int32_t iSta = CDevApiManager::GetInstance().GetOcmApi().DbgClearCommFail(0);

    if (iSta != OPLK_OK) {
        std::cout << "Ocm DbgClearCommFail error." << iSta << std::endl;
    } else {
        std::cout << "Ocm DbgClearCommFail done." << std::endl;
    }
}

void
__OcmPortPower(uint32_t uiPortId)
{
    COcmChPower stPower = {0};
    int32_t iSta = CDevApiManager::GetInstance().GetOcmApi().GetChPower(0, uiPortId, stPower);

    if (iSta != OPLK_OK) {
        std::cout << "__OcmPortPower error." << iSta << std::endl;
    } else {
        for (uint32_t uiIdx = 0; uiIdx < OCM_CH_MAX_NUM; uiIdx++) {
            std::cout << "asPower[" << uiIdx << "]: " << stPower.asPower[uiIdx] << std::endl;
        }
        std::cout << "__OcmPortPower done." << std::endl;
    }
}

void
__OcmOsaPortPower(uint32_t uiPortId)
{
    COcmOsa stPower = {0};
    int32_t iSta = CDevApiManager::GetInstance().GetOcmApi().GetOsaData(0, uiPortId, stPower);

    if (iSta != OPLK_OK) {
        std::cout << "__OcmOsaPortPower error." << iSta << std::endl;
    } else {
        for (uint32_t uiIdx = 0; uiIdx < OCM_SLICE_MAX_NUM; uiIdx++) {
            std::cout << "asOsaPower[" << uiIdx << "]: " << stPower.asOsa[uiIdx] << std::endl;
        }
        std::cout << "__OcmOsaPortPower done." << std::endl;
    }
}


void
__OcmAllTest()
{

    std::cout << std::endl << std::endl;

    __OcmMfg();

    __OcmPortPower(1);
    __OcmPortPower(2);
    __OcmSetOcmWavePlan();

}

void
__OcmTest(char *argv[])
{
    std::vector<std::string> staCmdVec = {"mfg", "comm-fail", "clear-comm-fail", "reset", "port-power", "osa", "waveplan", "upgrade", "all"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);


    switch (iCmdIdx) {
    case 0:
        __OcmMfg();
        break;

    case 1:
        __OcmCommFail();
        break;

    case 2:
        __OcmClearCommFail();
        break;

    case 3:
    {
        __OcmReset(atoi(argv[3]));
    }
    break;

    case 4:
    {
        __OcmPortPower(atoi(argv[3]));
    }
    break;

    case 5:
    {
        __OcmOsaPortPower(atoi(argv[3]));
    }
    break;

    case 6:
    {
        __OcmSetOcmWavePlan();
    }
    break;

    case 7:
    {
        __OcmUpgrade(argv[3]);
    }
    break;

    case 8:
    {
        __OcmAllTest();
    }
    break;

    default:
        std::cout << "Unknown Command." << std::endl;
    }

    return;


#if 0
    __OcmMfg();
#endif

#if 0
    __OcmSetWavePlan();
#endif

#if 0
    __OcmUpgrade();
#endif

#if 0
    __OcmReset(RST_MODE_COLD);
#endif
}

void
__OpsMfg(uint32_t uiDevIdx)
{
    CMfg stMfg;

    /* ops mfg */
    if (CDevApiManager::GetInstance().GetOpsApi().GetMfg(uiDevIdx, stMfg) != OPLK_OK) {
        std::cout << "get ops module mfg error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ops module mfg done." << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << "Mfg Data:" << stMfg.acMfgDate << std::endl;

        std::cout << std::endl;
    }

}

void
__OpsCommonStatusData(uint32_t uiDevIdx)
{

    COpsCommStatusData stData;
    char *pAlmMask = NULL;
    memset(&stData, 0, sizeof(stData));

    /* ops mfg */
    if (CDevApiManager::GetInstance().GetOpsApi().GetCommStatusData(uiDevIdx, stData) != OPLK_OK) {
        std::cout << "GetCommonStatusData error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "GetCommonStatusData done." << uiDevIdx << std::endl;
        std::cout << std::endl;
        std::cout << "Alarm:" << std::endl;
        for (uint32_t uiPortCnt = 0; uiPortCnt < OPS_PORT_CNT; uiPortCnt++) {
            std::cout << "OpsPortAlarm[" << uiPortCnt << "]" << stData.stAlarm.aaucPortAlarm[PORT_ALARM_TYPE_LOS][uiPortCnt] << " " << std::endl;
        }
        std::cout << "stAlarm.bSwitchFailed:" << (uint32_t)stData.stAlarm.ucSwitchFailed << std::endl;
        std::cout << "stAlarm.bRelative:" << (uint32_t)stData.stAlarm.ucRelative << std::endl;
        std::cout << "stAlarm.bMfgFailed:" << (uint32_t)stData.stAlarm.ucMfgFailed << std::endl;
        std::cout << "stAlarm.bComInLoss:" << (uint32_t)stData.stAlarm.ucComInLoss << std::endl;
        std::cout << "stAlarm.bPowerDiffer:" << (uint32_t)stData.stAlarm.ucPowerDiffer << std::endl;

        std::cout << std::endl;
        std::cout << "Led:" << std::endl;
        for (uint32_t i = 0; i < OPS_LED_CNT; i++) {
            std::cout << "led[" << i << "]: " << (uint32_t)stData.stLed.aucStatus[i] << " " << std::endl;
        }

        std::cout << std::endl;
        std::cout << "Alarm Mask:" << std::endl;

        pAlmMask = (char *)&stData.stAlmMask;
        for (uint32_t i = 0; i < sizeof(stData.stAlmMask); i++) {
            std::cout << "Mask[" << i << "]: " << (uint32_t)pAlmMask[i] << " " << std::endl;
        }
        std::cout << std::endl;
    }
}

void
__OpsStatusData(uint32_t uiDevIdx, uint32_t uiSubIdx)
{

    COpsStatusData stData;

    memset(&stData, 0, sizeof(stData));

    /* ops mfg */
    if (CDevApiManager::GetInstance().GetOpsApi().GetStatusData(uiDevIdx, stData, uiSubIdx) != OPLK_OK) {
        std::cout << "GetStatusData error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "GetStatusData done." << uiDevIdx << uiSubIdx << std::endl;
        for (uint32_t i = 0; i < OPS_PORT_CNT; i++) {
            std::cout << "OpsPortPower[" << i << "]: " << stData.stCurData.asPortPower[i] << std::endl;
        }

        std::cout << std::hex << "ucWorkMode:" << (uint32_t)stData.stCurData.ucWorkMode << std::endl;
        std::cout << std::hex << "ucSwitchPos:" << (uint32_t)stData.stCurData.ucSwitchPos << std::endl;
        std::cout << std::hex << "ucFswStatus:" << (uint32_t)stData.stCurData.ucFswStatus << std::endl;
#if 0
        for (uint32_t i = 0; i < PORT_ALARM_TYPE_CNT; i++) {
            for (uint32_t j = 0; j < OPS_PORT_CNT; j++) {
                std::cout << "aaucPortAlarm[" << i << "]" << "[" << j << "]:" << stData.stAlarm.abPortAlarm[i][j] << " ";
            }
            std::cout << std::endl;
        }

        for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
            std::cout << "WssPortPower[" << i << "]: " << stData.stWssStaData.stCurData.asPortPower[i] << std::endl;
        }

        for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
            std::cout << "WssPortPower[" << i << "]: " << stData.stWssStaData.stAlarm.abPortPower[0][i] << std::endl;
        }

        for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
            std::cout << "WssPortPower[" << i << "]: " << stData.stWssStaData.stAlarm.abPortPower[1][i] << std::endl;
        }


        for (uint32_t i = 0; i < WSS_PORT_CNT; i++) {
            std::cout << "WssPortPower[" << i << "]: " << stData.stWssStaData.stAlarm.abPortPower[2][i] << std::endl;
        }

        std::cout << std::endl;
#endif
    }

    /* printf("%02x, %02x, %02x, %02x\n", stData.stCurData.asPortPower[0], stData.stCurData.asPortPower[1],
           stData.stCurData.asPortPower[2], stData.stCurData.asPortPower[3]); */

}

void
__OpsSetRevertive(uint32_t uiDevIdx, uint32_t uiRevertive, uint32_t uiSubIdx)
{
    int32_t iSta;

    iSta = CDevApiManager::GetInstance().GetOpsApi().SetRevertive(uiDevIdx, uiRevertive, uiSubIdx);

    /* ops revertive */
    if ( iSta != OPLK_OK) {
        std::cout << "set ops revertive error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops revertive ok." << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetMode(uint32_t uiDevIdx, uint32_t uiMode, uint32_t uiSubIdx)
{
    /* ops mode */
    if (CDevApiManager::GetInstance().GetOpsApi().SetMode(uiDevIdx, uiMode, uiSubIdx) != OPLK_OK) { /* 1 */
        std::cout << "set ops mode error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops mode ok." << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetHoldOffTime(uint32_t uiDevIdx, uint32_t uiHoldOffTime, uint32_t uiSubIdx)
{
    /* ops hold off time */
    if (CDevApiManager::GetInstance().GetOpsApi().SetHoldOffTime(uiDevIdx, uiHoldOffTime, uiSubIdx) != OPLK_OK) { /* 1 */
        std::cout << "set ops hold off time error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops hold off time ok." << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetWtr(uint32_t uiDevIdx, uint32_t uiWtr, uint32_t uiSubIdx)
{
    /* ops wtr */
    if (CDevApiManager::GetInstance().GetOpsApi().SetWtr(uiDevIdx, uiWtr, uiSubIdx) != OPLK_OK) { /* 400 */
        std::cout << "set ops wtr error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops wtr ok." << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetPortPowerThr(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int32_t iThr, uint32_t uiSubIdx)
{
    /* ops thr */
    if (CDevApiManager::GetInstance().GetOpsApi().SetPortPowerThr(uiDevIdx, uiPort, uiType, iThr, uiSubIdx) != OPLK_OK) {
        std::cout << "set ops thr error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops thr ok. uiType " << uiType << "," << " iThr " << iThr << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetPortPowerHys(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int iHys, uint32_t uiSubIdx)
{
    /* ops hys */
    if (CDevApiManager::GetInstance().GetOpsApi().SetPortPowerHys(uiDevIdx, uiPort, uiType, iHys, uiSubIdx) != OPLK_OK) {
        std::cout << "set ops hys error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops hys ok. uiType " << uiType << "," << " iHys " << iHys << std::endl;
        std::cout << std::endl;
    }
}


void
__OpsSetWssPortThr(uint32_t uiDevIdx, uint32_t uiPort, uint32_t uiType, int iHys, uint32_t uiSubIdx)
{
    /* wss port thr */
    if (CDevApiManager::GetInstance().GetOpsApi().SetWssPortPowerThr(uiDevIdx, uiPort, uiType, iHys) != OPLK_OK) {
        std::cout << "set ops hys error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops hys ok. uiType " << uiType << "," << " iHys " << iHys << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetRelativeOffset(uint32_t uiDevIdx, int32_t iOffset, uint32_t uiSubIdx)
{
    int32_t iSta = 0;

    iSta = CDevApiManager::GetInstance().GetOpsApi().SetRelativeOffset(uiDevIdx, iOffset, uiSubIdx); /* 10 */

    if ( iSta != OPLK_OK) {
        std::cout << "__OpsSetRelativeOffset error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops RelativeOffset ok. iOffset " << iOffset << std::endl;
        std::cout << std::endl;
    }

}

void
__OpsGetWssPortPowerHistory(uint32_t uiDevIdx)
{
    int32_t iSta = 0;

    iSta = CDevApiManager::GetInstance().GetOpsApi().GetWssPortPowerHistory(uiDevIdx);

    if ( iSta != OPLK_OK) {
        std::cout << "__OpsGetWssPortPowerHistory error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsUpgrade(uint32_t uiDevIdx, char *pcFilePath)
{
    int32_t iSta = 0;
#if 1
    if ( NULL == pcFilePath) {
        std::cout << "Ocm upgrade fail. NULL == pcFilePath" << std::endl;
    }
    std::cout << "ops upgrade start, pcFilePath" << pcFilePath << std::endl;
    if (CDevApiManager::GetInstance().GetOpsApi().Upgrade(uiDevIdx, pcFilePath) != OPLK_OK) {  /* "/home/oplink/log/module_bundle_9.99.9999.bin" module_bundle0.01.0002.bin */
        std::cout << "Upgrade error." << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(3, S);

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOpsApi().GetUpgadeStatus(0);
        std::cout << "Ops " << "iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }
#endif

    if (CDevApiManager::GetInstance().GetOpsApi().Reset(0, 1) != OPLK_OK) {
        std::cout << "Upgrade error." << std::endl;
        std::cout << std::endl;
    }

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOpsApi().GetUpgadeStatus(0);
        std::cout << "Ops " << "iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }
}



#if 0
typedef struct
{
    uint32_t uiHour;
    uint32_t uiMinute;
    uint32_t uiSecond;
    uint32_t uiMillisecond;
    int16_t asPortPowerHistory[OPS_PORT_LINE_IN_CNT][PORT_POWER_HISTORY_MAX_NUM];

} COpsHistoryUnitData;

typedef struct
{
    uint32_t uiSwitchTimes;     /* [Switch times]: 4 bytes, unsigned int, the max is 10. */
    COpsHistoryUnitData astOpsHisUnitData[10];
} COpsHistoryData;
#endif
void
__OpsGetPortPowerHistory(uint32_t uiDevIdx)
{

    COpsHistoryData stData;

    memset(&stData, 0, sizeof(stData));

    /* ops mfg */
    if (CDevApiManager::GetInstance().GetOpsApi().GetHistoryData(uiDevIdx, stData) != OPLK_OK) {
        std::cout << "GetHisData error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "GetHisData done." << std::endl;

        std::cout << "uiSwitchTimes:" << (uint32_t)stData.uiSwitchTimes << std::endl;

        for (uint32_t i = 0; i < 10; i++) {
            std::cout << "astOpsHisUnitData[" << i << "]" << "uiHour:" << stData.astOpsHisUnitData[i].uiHour << std::endl;
            std::cout << "astOpsHisUnitData[" << i << "]" << "uiMinute:" << stData.astOpsHisUnitData[i].uiMinute << std::endl;
            std::cout << "astOpsHisUnitData[" << i << "]" << "uiSecond:" << stData.astOpsHisUnitData[i].uiSecond << std::endl;
            std::cout << "astOpsHisUnitData[" << i << "]" << "uiMillisecond:" << stData.astOpsHisUnitData[i].uiMillisecond << std::endl;
            std::cout << "astOpsHisUnitData[" << i << "]" << "asPortPowerHistory:" << std::endl;
            for (uint32_t j = 0; j < OPS_PORT_CNT; j++) {
                std::cout << "port[" << j << "]:" << std::endl;
                for (uint32_t k = 0; k < PORT_POWER_HISTORY_MAX_NUM; k++) {
                    std::cout << stData.astOpsHisUnitData[i].asPortPowerHistory[j][k] << " ";
                    /* std::cout << "astOpsHisUnitData[" << i << "]" << "asPortPowerHistory[" << j << "]"<<"[" << k << "]:" << stData.astOpsHisUnitData[i].asPortPowerHistory[j][k] << " "; */
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

}

void
__OpsSetLedTest(uint32_t uiDevIdx, uint32_t uiEnable)
{
    /* ops wtr */
    if (CDevApiManager::GetInstance().GetOpsApi().SetLedMode(uiDevIdx, uiEnable, 0) != OPLK_OK) {
        std::cout << "set ops ledtest error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops ledtest ok." << std::endl;
        std::cout << std::endl;
    }
}

void
__OpsSetAlmMask(uint32_t uiDevIdx, uint32_t uiIdx, uint32_t uiMask, uint32_t uiSubIdx)
{
    /* ops hys */
    if (CDevApiManager::GetInstance().GetOpsApi().SetAlmMask(uiDevIdx, uiIdx, uiMask, uiSubIdx) != OPLK_OK) {
        std::cout << "set ops almmask error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ops almmask ok. uiDevIdx " << uiDevIdx << "," << " uiIdx " << uiIdx << " uiMask " << uiMask << " uiSubIdx " << uiSubIdx << std::endl;
        std::cout << std::endl;
    }
}


#if 0
void
__OpsTest()
{
#define TYPE_LOS  0
#define TYPE_LOW  1
#define TYPE_HIGH 2

#if 0
    __OpsSetWssPortThr(0, TYPE_LOS, -2000);
    __OpsSetWssPortThr(0, TYPE_LOS, -600);
    __OpsSetWssPortThr(0, TYPE_LOW, -1100);
    __OpsSetWssPortThr(0, TYPE_LOW, 100);

    __OpsSetWssPortThr(0, TYPE_HIGH, 500);
    __OpsSetWssPortThr(0, TYPE_HIGH, 1500);

#endif

#if 1
    __OpsMfg();
#endif

#if 0
    __OpsStatusData();
#endif

#if 0
    __OpsUpgrade();
#endif

#if 0
    __OpsSetMode();

#endif

#if 0
    __OpsSetHoldOffTime();

#endif

#if 0
    __OpsSetWtr();

#endif

#if 0
    __OpsSetRevertive();

#endif

#if 0
    __OpsSetPortPowerThr(0, TYPE_LOS, -200);

#endif

#if 0
    __OpsSetPortPowerThr(1, TYPE_LOS, -200);

#endif

#if 0
    __OpsSetPortPowerThr(0, TYPE_LOW, 100);

#endif

#if 0
    __OpsSetPortPowerThr(2, TYPE_LOW, 100);

#endif

#if 0
    __OpsSetPortPowerThr(0, TYPE_HIGH, 1700);

#endif

#if 0
    __OpsSetPortPowerThr(2, TYPE_HIGH, 1800);

#endif


#if 0
    __OpsSetPortPowerHys(0, TYPE_HIGH, 200);

#endif

#if 0
    __OpsSetPortPowerHys(1, TYPE_LOS, 200);

#endif

#if 0
    __OpsGetWssPortPowerHistory();
#endif

#if 0
    __OpsSetRelativeOffset();
#endif


}
#else

void
__OpsAllTest()
{
#define TYPE_LOS  0
#define TYPE_LOW  1
#define TYPE_HIGH 2

    std::cout << std::endl << std::endl;

    __OpsMfg(0);
    __OpsStatusData(0, 1);

    __OpsSetWtr(0, 0, 1);
    __OpsSetHoldOffTime(0, 100, 1);
    __OpsSetWtr(0, 4, 1);

    __OpsSetPortPowerThr(0, 0, TYPE_LOS, -200, 1);
    __OpsSetPortPowerHys(0, 0, TYPE_HIGH, 200, 1);

    __OpsSetRevertive(0, 3, 1);
    __OpsSetRelativeOffset(0, 1.5, 1);

    __OpsStatusData(0, 1);

}

void
__OpsTest(char *argv[])
{
    int32_t uiPort = 0;
    uint32_t uiType = 0;
    int32_t iThr = 0;
    int32_t iHys = 0;
    uint32_t uiMode = 0;
    uint32_t uiHoldOffTime = 0;
    uint32_t uiWtr = 0;
    uint32_t uiRevertive = 0;
    int32_t iOffset = 0;
    uint32_t uiSubIdx = 1;
    uint32_t uiEnable = 1;
    uint32_t uiIdx = 0;
    uint32_t uiMask = 0;

    std::vector<std::string> staCmdVec = {"mfg", "data", "upgrade", "mode", "holdofftime",
                                          "wtr", "thr", "hys", "revertive", "relativeoffset", "history", "ledtest", "common", "all", "almmask"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);
    uint32_t uiDevIdx = atoi(argv[3]);

    switch (iCmdIdx) {
    case 0:
        __OpsMfg(uiDevIdx);
        break;

    case 1:
        uiSubIdx = atoi(argv[4]);
        __OpsStatusData(uiDevIdx, uiSubIdx);
        break;

    case 2:
        __OpsUpgrade(uiDevIdx, argv[4]);
        break;

    case 3:
        uiMode = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OpsSetMode(uiDevIdx, uiMode, uiSubIdx);
        break;

    case 4:
        uiHoldOffTime = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OpsSetHoldOffTime(uiDevIdx, uiHoldOffTime, uiSubIdx);
        break;

    case 5:
        uiWtr = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OpsSetWtr(uiDevIdx, uiWtr, uiSubIdx);
        break;

    case 6:
        uiPort = atoi(argv[4]);
        uiType = atoi(argv[5]);
        iThr = atoi(argv[6]);
        uiSubIdx = atoi(argv[7]);
        __OpsSetPortPowerThr(uiDevIdx, uiPort, uiType, iThr, uiSubIdx);
        break;

    case 7:
        uiPort = atoi(argv[4]);
        uiType = atoi(argv[5]);
        iHys = atoi(argv[6]);
        uiSubIdx = atoi(argv[7]);
        __OpsSetPortPowerHys(uiDevIdx, uiPort, uiType, iHys, uiSubIdx);
        break;

    case 8:
        uiRevertive = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OpsSetRevertive(uiDevIdx, uiRevertive, uiSubIdx);
        break;

    case 9:
        iOffset = atoi(argv[4]);
        uiSubIdx = atoi(argv[5]);
        __OpsSetRelativeOffset(uiDevIdx, iOffset, uiSubIdx);
        break;

    case 10:
        __OpsGetPortPowerHistory(uiDevIdx);
        break;

    case 11:
        uiEnable = atoi(argv[4]);
        __OpsSetLedTest(uiDevIdx, uiEnable);
        break;

    case 12:
        __OpsCommonStatusData(uiDevIdx);
        break;


    case 13:
        __OpsAllTest();
        break;

    case 14:
        uiIdx = atoi(argv[4]);
        uiMask = atoi(argv[5]);
        uiSubIdx = atoi(argv[6]);
        __OpsSetAlmMask(uiDevIdx, uiIdx, uiMask, uiSubIdx);
        break;
    default:
        std::cout << "Unknown Command." << std::endl;
    }

    return;
}
#endif

void
__OscLaserEnable(uint32_t uiDevIdx, bool bEnable)
{
    int32_t iSta;

    std::string staEnable = bEnable ? "ENABLE" : "DISABLE";

    std::cout << "set osc laser " << staEnable << "." << std::endl;

    /* while (1) { */
    __StartTime();

    if ((iSta = CDevApiManager::GetInstance().GetOscApi().SetLaserEnable(uiDevIdx, bEnable)) != OPLK_OK) {
        std::cout << "set laser error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

    __StopTime();

    return;
}
#if 0
void
__OscGetStatusData(uint32_t uiDevIdx)
{
    COscStatusData stOscStatus;
    CExtendStatusData stExtendStatus = {0};

    int32_t iSta;
#if 0
    iSta = CDevApiManager::GetInstance().GetExtendApi().GetStatusData(uiDevIdx, stExtendStatus);
    if ( OPLK_OK != iSta) {
        std::cout << "Get osc status data error." << std::endl;
        LOG_WRITE(EN_LOG_ERROR, "Get osc status data error.");
        return;
    }
#endif

    if ((iSta = CDevApiManager::GetInstance().GetOscApi().GetStatusData(uiDevIdx, stOscStatus)) != OPLK_OK) {
        std::cout << "set laser error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

    stOscStatus.sRxPower = stExtendStatus.asPD[4];
    stOscStatus.sTxPower = stExtendStatus.asPD[3];;
    stOscStatus.sAddPower = stExtendStatus.asPD[3];
    stOscStatus.sDropPower = stExtendStatus.asPD[5];


    std::cout << "Offline = " << stOscStatus.bOffLine << std::endl;
    std::cout << "RxLos = " << stOscStatus.bRxLos << std::endl;
    std::cout << "TxLos = " << stOscStatus.bTxLos << std::endl;
    std::cout << "LinkDown = " << stOscStatus.bLinkDown << std::endl;
    std::cout << "sRxPower = " << stOscStatus.sRxPower << std::endl;
    std::cout << "sTxPower = " << stOscStatus.sTxPower << std::endl;
    std::cout << "sAddPower = " << stOscStatus.sAddPower << std::endl;
    std::cout << "sDropPower = " << stOscStatus.sDropPower << std::endl;

    CDelay::Delay(1, S);
}
#endif

void
__OscGetStatusData(uint32_t uiDevIdx)
{
    COaStatusData stOaStatus;
    COscStatusData stOscStatus;

    int32_t iSta;

    memset(&stOaStatus, 0, sizeof(stOaStatus));
    memset(&stOscStatus, 0, sizeof(stOscStatus));

    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetStatusData(uiDevIdx, stOaStatus, 0)) != OPLK_OK) {
        std::cout << "set laser error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

#ifdef __NR1004__
    stOscStatus.sRxPower = stOaStatus.stOaCurData.asExtPdPower[E1_VOA3_OUT];
    stOscStatus.sTxPower = stOaStatus.stOaCurData.asExtPdPower[E2_VOA3_IN];
    stOscStatus.sAddPower = stOaStatus.stOaCurData.asExtPdPower[E2_OSCADD];
    stOscStatus.sDropPower = stOaStatus.stOaCurData.asExtPdPower[E1_OSCDROP];
#endif
    stOscStatus.bMisMatch = stOaStatus.stOscCurData.bMisMatch;
    stOscStatus.bRxLow = stOaStatus.stOscCurData.bRxLow;
    stOscStatus.bTxLow = stOaStatus.stOscCurData.bTxLow;
    stOscStatus.bAddLow = stOaStatus.stOscCurData.bAddLow;
    stOscStatus.bDropLow = stOaStatus.stOscCurData.bDropLow;
    stOscStatus.bRxHigh = stOaStatus.stOscCurData.bRxHigh;
    stOscStatus.bTxHigh = stOaStatus.stOscCurData.bTxHigh;
    stOscStatus.bAddHigh = stOaStatus.stOscCurData.bAddHigh;
    stOscStatus.bDropHigh = stOaStatus.stOscCurData.bDropHigh;

    stOscStatus.bFailure = stOaStatus.stOscCurData.bFailure;
    stOscStatus.bInnerErr = stOaStatus.stOscCurData.bInnerErr;
    stOscStatus.bOffLine = stOaStatus.stOscCurData.bOffLine;
    stOscStatus.bRxLos = stOaStatus.stOscCurData.bRxLos;
    stOscStatus.bTxLos = stOaStatus.stOscCurData.bTxLos;
    stOscStatus.bAddLos = stOaStatus.stOscCurData.bAddLos;
    stOscStatus.bDropLos = stOaStatus.stOscCurData.bDropLos;
    stOscStatus.bLinkDown = stOaStatus.stOscCurData.bLinkDown;
    stOscStatus.ucModStatus = stOaStatus.stOscCurData.ucModStatus;


    std::cout << "sRxPower = " << stOscStatus.sRxPower << std::endl;
    std::cout << "sTxPower = " << stOscStatus.sTxPower << std::endl;
    std::cout << "sAddPower = " << stOscStatus.sAddPower << std::endl;
    std::cout << "sDropPower = " << stOscStatus.sDropPower << std::endl;

    std::cout << "bFailure = " << stOscStatus.bFailure << std::endl;
    std::cout << "bInnerErr = " << stOscStatus.bInnerErr << std::endl;
    std::cout << "Offline = " << stOscStatus.bOffLine << std::endl;
    std::cout << "RxLos = " << stOscStatus.bRxLos << std::endl;
    std::cout << "TxLos = " << stOscStatus.bTxLos << std::endl;
    std::cout << "AddLos = " << stOscStatus.bAddLos << std::endl;
    std::cout << "DropLos = " << stOscStatus.bDropLos << std::endl;
    std::cout << "LinkDown = " << stOscStatus.bLinkDown << std::endl;

    std::cout << "bMisMatch = " << stOscStatus.bMisMatch << std::endl;
    std::cout << "bRxLow = " << stOscStatus.bRxLow << std::endl;
    std::cout << "bTxLow = " << stOscStatus.bTxLow << std::endl;
    std::cout << "bAddLow = " << stOscStatus.bAddLow << std::endl;
    std::cout << "bDropLow = " << stOscStatus.bDropLow << std::endl;
    std::cout << "bRxHigh = " << stOscStatus.bRxHigh << std::endl;
    std::cout << "bTxHigh = " << stOscStatus.bTxHigh << std::endl;
    std::cout << "bAddHigh = " << stOscStatus.bAddHigh << std::endl;
    std::cout << "bDropHigh = " << stOscStatus.bDropHigh << std::endl;

    std::cout << "oscVoaRx = " << stOaStatus.stOaCurData.asVoaAttnActual[VOA_OSC] << std::endl;
#ifdef __NR1004__
    std::cout << "oscVoaTx = " << stOaStatus.stOaCurData.asVoaAttnActual[VOA_OSC_TX] << std::endl;
#endif
    CDelay::Delay(1, S);
}

void
__OscSetAttn(uint32_t uiIdx, uint32_t uiAttn, uint32_t uiSubIdx)
{
    int32_t iSta;

    __StartTime();

    if ((iSta = CDevApiManager::GetInstance().GetOaApi().SetAttn(uiIdx, uiAttn, VOA_OSC_TX)) != OPLK_OK) {
        std::cout << "OSC" << uiIdx << " set attn error. uiAttn = " << uiAttn << " iSta = " << iSta << std::endl;
    } else {
        std::cout << "OSC" << uiIdx << " set attn done. uiAttn = " << uiAttn << std::endl;
    }

    __StopTime();
}

void
__OscTest(char *argv[])
{
    std::vector<std::string> staCmdVec = {"data", "voa"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);
    uint32_t uiAttn = 0;

    switch (iCmdIdx) {
    case 0:
    {
        /* ./api_main osc data */
        __OscGetStatusData(0);
        break;
    }
    case 1:
    {
        /* ./api_main osc voa 300 */
        uiAttn = atoi(argv[3]);
        __OscSetAttn(0, uiAttn, 0);
        break;
    }

    default:
        std::cout << "Unknown Command." << std::endl;
    }

    return;
}

void
__TdcmTest()
{

    CMfg stMfg;
    int32_t iSta;

    CTdcmStatusData stTdcmData;
#if 0
    memset(&stMfg, 0, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetTdcmApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "TDCM: get tdcm mfg fail. error number = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "TDCM Mfg: " << std::endl;
        std::cout << "Manufacturer: " << stMfg.acManufacturer << std::endl;
        std::cout << "SN: " << stMfg.acSN << std::endl;
        std::cout << "PN: " << stMfg.acPN << std::endl;
        std::cout << "MfgDate: " << stMfg.acMfgDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

#endif


    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetTdcmApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "TDCM: get mfg err." << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "TDCM: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "Fw Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    memset(&stTdcmData, 0, sizeof(stTdcmData));
    if ((iSta = CDevApiManager::GetInstance().GetTdcmApi().GetStatusData(0, stTdcmData)) != OPLK_OK) {
        std::cout << "TDCM: get tdcm data fail. error number = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "TDCM: " << std::endl;
        std::cout << "Frequency: " << stTdcmData.fFrequency << std::endl;
        std::cout << "Dispersion: " << stTdcmData.fDispersion << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);



}

void
__ExtendGetMfg()
{
    int32_t iSta;
    CMfg stMfg;

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetExtendApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "EXTEND: get mfg err." << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "EXTEND: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "Fw Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }

}

void
__ExtendStatusData()
{
    int32_t iSta;

    CExtendStatusData stStatus;

    memset(&stStatus, 0, sizeof(stStatus));
    if ((iSta = CDevApiManager::GetInstance().GetExtendApi().GetStatusData(0, stStatus)) != OPLK_OK) {
        std::cout << "Extend: get  data fail. error number = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "Extend: " << std::endl;
        for (int i = 0; i < 16; i++) {
            std::cout << "asPD[" << i << "] = " << stStatus.asPD[i] << std::endl;
        }
        std::cout << "voa: " << stStatus.asVoaAttnActual[1] << std::endl;
        std::cout << std::endl;
        std::cout << "switch: " << stStatus.asSwitchPosActual[0] << std::endl;
        std::cout << std::endl;
    }
}

void
__ExtendSetVoa(uint32_t uiAttn, uint32_t uiIdx)
{
    int32_t iSta;

    __StartTime();

    std::cout << "Extend: set voa uiAttn = " << uiAttn << "." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetExtendApi().SetVoaAttn(0, uiAttn, uiIdx)) != OPLK_OK) {
        std::cout << "Extend: set voa. error number = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
    }
    __StopTime();
}

void
__ExtendSetSwitchPos(uint32_t uiPos, uint32_t uiIdx)
{
    int32_t iSta;

    __StartTime();

    std::cout << "Extend: set switch pos = " << uiPos << "." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetExtendApi().SetSwitchPos(0, uiPos, uiIdx)) != OPLK_OK) {
        std::cout << "Extend: set switch. error number = " << iSta << std::endl;
        std::cout << std::endl;
    }
    __StopTime();

    return;
}

void
__ExtendReset(uint32_t uiResetType)
{
    int32_t iSta;

    std::string staResetType = (uiResetType == RST_MODE_COLD ? "Cold Reset" : "Warm Reset");

    std::cout << "Extend: " << staResetType << std::endl;

    __StartTime();
    iSta = CDevApiManager::GetInstance().GetExtendApi().Reset(0, uiResetType);

    if (iSta != OPLK_OK) {
        std::cout << "Extend: Reset error." << iSta << std::endl;
        std::cout << std::endl;
    }

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetExtendApi().GetUpgadeStatus(0);
        std::cout << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;

    }

    __StopTime();
}


void
__ExtendUpgrade()
{
    int32_t iSta;

    __StartTime();

    if ((iSta = CDevApiManager::GetInstance().GetExtendApi().Upgrade(0, "/home/oplink/log/Adapter_Main_2021-03-22_V0.03.0006.bin")) != OPLK_OK) {
        std::cout << "Extend: Upgrade error." << iSta << std::endl;
        std::cout << std::endl;
    }

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetExtendApi().GetUpgadeStatus(0);
        std::cout << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }

    __StopTime();

    __ExtendReset(RST_MODE_COLD);

    __ExtendGetMfg();

}



void
__ExtendSetSfpEnable(uint32_t uiEanble)
{
    int32_t iSta;
    std::string staEnable = 0 == uiEanble ? "Enable Sfp." : "Disable Sfp.";

    __StartTime();

    std::cout << staEnable << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetExtendApi().SetSfp(0, uiEanble, 0)) != OPLK_OK) {
        std::cout << "Extend: set sfp. error number = " << iSta << std::endl;
        std::cout << std::endl;
    }
    __StopTime();

    return;
}

void
__ExtendTest()
{
#if 1
    __ExtendSetSwitchPos(3, 1);
#endif

#if 0
    __ExtendGetMfg();
    __ExtendStatusData();

    __ExtendSetVoa(500, 2);
    __ExtendSetSwitchPos(1, 1);
    /* __ExtendSetSfpEnable(0); */
    CDelay::Delay(1, S);
    __ExtendStatusData();

    __ExtendSetVoa(1500, 2);
    /* __ExtendSetSfpEnable(1); */
    CDelay::Delay(1, S);
    __ExtendStatusData();

#endif


#if 0
    __StartTime();
    __ExtendReset(RST_MODE_WARM);
    __ExtendGetMfg();
    __StopTime();

    __StartTime();
    __ExtendReset(RST_MODE_COLD);
    __ExtendGetMfg();
    __StopTime();

#endif
}

void
__LedStatus()
{
    int32_t iSta;
    CBoardStatusData stBoardStatusData;

    __StartTime();
    std::cout << "Board: Status Data." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetStatusData(stBoardStatusData)) != OPLK_OK) {
        std::cout << "get status data error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

    for (uint32_t uiIdx = LED0; uiIdx < LED_CNT; uiIdx++) {
        std::cout << "LED" << uiIdx << ": " << stBoardStatusData.astLed[uiIdx].uiState << std::endl;
    }
}


void
__LedTest()
{

    int32_t iSta;

    __LedStatus();


    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetLedState(LED0, LED_STATUS_RED)) != OPLK_OK) {
        std::cout << "system led: red " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "system led: red done." << iSta << std::endl;
        std::cout << std::endl;
    }


    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetLedState(LED1, LED_STATUS_GREEN)) != OPLK_OK) {
        std::cout << "ba alarm led: red " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "ba alarm led: red done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetLedState(LED2, LED_STATUS_AMBER)) != OPLK_OK) {
        std::cout << "set pa system red error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led red done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetLedState(LED3, LED_STATUS_RED_BLINK)) != OPLK_OK) {
        std::cout << "set pa system red error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led red done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetLedState(LED4, LED_STATUS_GREEN_BLINK)) != OPLK_OK) {
        std::cout << "set pa system red error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led red done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetLedState(LED5, LED_STATUS_AMBER_BLINK)) != OPLK_OK) {
        std::cout << "set pa system red error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led red done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    __LedStatus();

}

void
__FanTest()
{
    CMfg stMfg;

    CBoardStatusData stBoardStatusData;

    int32_t iSta;
    std::vector<uint32_t> staFans = {FAN0, FAN1, FAN2, FAN3};

#if 0
    for (auto uiFanIdx : staFans) {
        if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetFanMfg(uiFanIdx, "/home/oplink/log/mfginfo_fan.conf")) != OPLK_OK) {
            std::cout << "set fan" << uiFanIdx << " mfg error. iSta = " << iSta << std::endl;
            std::cout << std::endl;
        } else {
            std::cout << "set fan" << uiFanIdx << "  mfg done." << std::endl;
            std::cout << std::endl;
        }
        CDelay::Delay(1, S);
    }
#else
    for (auto uiFanIdx : staFans) {
        memset(&stMfg, 0x00, sizeof(stMfg));
        if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetFanMfg(uiFanIdx, stMfg)) != OPLK_OK) {
            std::cout << "get fan mfg error. iSta = " << iSta << std::endl;
            std::cout << std::endl;
        } else {
            std::cout << "fan #" << uiFanIdx << std::endl;
            std::cout << "SN:" << stMfg.acSN << std::endl;
            std::cout << "PN:" << stMfg.acPN << std::endl;
            std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
            std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
            std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
            std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
            std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
            std::cout << std::endl;
        }
        CDelay::Delay(1, S);
    }
#endif

#if 1
    for (auto uiFanIdx : staFans) {

        if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetFanSpeed(uiFanIdx, 25)) != OPLK_OK) {
            std::cout << "set fan speed fail. error number = " << iSta << std::endl;
            return;
            std::cout << std::endl;
        } else {
            std::cout << "set fan speed done. " << std::endl;
            std::cout << std::endl;
        }
    }
    CDelay::Delay(10, S);


    memset(&stBoardStatusData, 0, sizeof(stBoardStatusData));
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetStatusData(stBoardStatusData)) != OPLK_OK) {
        std::cout << "Board: get board data fail. error number = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "Board: " << std::endl;
        std::cout << "fan 0 speed : " << stBoardStatusData.astFan[0].uiSpeed << std::endl;
        std::cout << "fan 0 online : " << stBoardStatusData.astFan[0].bIsOnline << std::endl;
        std::cout << "fan 1 speed : " << stBoardStatusData.astFan[1].uiSpeed << std::endl;
        std::cout << "fan 1 online : " << stBoardStatusData.astFan[1].bIsOnline << std::endl;
        std::cout << "fan 2 speed : " << stBoardStatusData.astFan[2].uiSpeed << std::endl;
        std::cout << "fan 2 online : " << stBoardStatusData.astFan[2].bIsOnline << std::endl;
        std::cout << "fan 3 speed : " << stBoardStatusData.astFan[3].uiSpeed << std::endl;
        std::cout << "fan 3 online : " << stBoardStatusData.astFan[3].bIsOnline << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
#endif
}

void
__PsuTest()
{
    CMfg stMfg;
    int32_t iSta;
    CBoardStatusData stBoardStatusData;

    std::vector<uint32_t> staPsus = {PSU0, PSU1};

    for (auto uiPsuIdx : staPsus) {
        memset(&stMfg, 0x00, sizeof(stMfg));

        if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetPowerMfg(uiPsuIdx, stMfg)) != OPLK_OK) {
            std::cout << "get PSU mfg error. iSta = " << iSta << std::endl;
            std::cout << std::endl;
        } else {
            std::cout << "PSU #" << uiPsuIdx << std::endl;
            std::cout << "Manufacturer:" << stMfg.acManufacturer << std::endl;
            std::cout << "ProductName:" << stMfg.acProductName << std::endl;
            std::cout << "ModuleType:" << stMfg.acModuleType << std::endl;
            std::cout << "SN:" << stMfg.acSN << std::endl;
            std::cout << "PN:" << stMfg.acPN << std::endl;
            std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
            std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
            std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
            std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
            std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
            std::cout << std::endl;
        }
        CDelay::Delay(1, S);
    }


    memset(&stBoardStatusData, 0, sizeof(stBoardStatusData));
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetStatusData(stBoardStatusData)) != OPLK_OK) {
        std::cout << "get board status. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        for (auto uiPsuIdx : staPsus) {
            std::cout << "Psu" << uiPsuIdx << "IsOnline = " << stBoardStatusData.astPower[uiPsuIdx].bIsOnline << std::endl;
            std::cout << "Psu" << uiPsuIdx << "IsFailure = " << stBoardStatusData.astPower[uiPsuIdx].bIsFailure << std::endl;
        }
    }

}

void
__GetChassisMfg()
{
    CMfg stMfg;
    int32_t iSta;

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetChassisMfg(stMfg)) != OPLK_OK) {
        std::cout << "Scc: get mfg err." << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "Chassic: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
}

void
__BoardGetMfg()
{
    CMfg stMfg;
    int32_t iSta;

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetSccMfg(stMfg)) != OPLK_OK) {
        std::cout << "Scc: get mfg err." << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "Scc: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
    return;


    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetChassisMfg(stMfg)) != OPLK_OK) {
        std::cout << "Scc: get mfg err." << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "Chassic: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
}

void
__BoardGetStatusData()
{

    int32_t iSta;
    CBoardStatusData stBoardStatusData;

    __StartTime();
    std::cout << "Board: Status Data." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetStatusData(stBoardStatusData)) != OPLK_OK) {
        std::cout << "get status data error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }


    for (uint32_t uiIdx = 0; uiIdx < 2; uiIdx++) {

        float inVol = (float)stBoardStatusData.astPower[uiIdx].usInVol / 100;
        float outVol = (float)stBoardStatusData.astPower[uiIdx].usOutVol / 100;
        float inCurrent = (float)stBoardStatusData.astPower[uiIdx].uiInCurrent / 100;
        float outCurrent = (float)stBoardStatusData.astPower[uiIdx].uiOutCurrent / 100;
        float watts = (float)stBoardStatusData.astPower[uiIdx].uiWatts / 100;

        std::cout << "Psu " << uiIdx << std::endl;
        std::cout << "Online = " << stBoardStatusData.astPower[uiIdx].bIsOnline << std::endl;
        std::cout << "Ok = " << stBoardStatusData.astPower[uiIdx].bIsFailure << std::endl;
        std::cout << "InVol = " << inVol << "V" << std::endl;
        std::cout << "OutVol = " << outVol << "V" << std::endl;
        std::cout << "InCurrent = " << inCurrent << "A" << std::endl;
        std::cout << "OutCurrent = " << outCurrent << "A" << std::endl;
        std::cout << "Watts = " << watts << "W" << std::endl;
        std::cout << std::endl;
    }

    std::string staEnable = (true == stBoardStatusData.abApsdEnable[0] ? "Enable." : "Disable.");
    std::string staStatus = (true == stBoardStatusData.abApsdStatus[0] ? "Apsd." : "Apsd Clear.");

    std::cout << "reset type = " << (int)stBoardStatusData.aucResetType[0] << std::endl;

    std::cout << "abApsdEnable = " << staEnable << std::endl;
    std::cout << "abApsdStatus = " << staStatus << std::endl;
    std::cout << "abApsdEvent = " << stBoardStatusData.abApsdEvent[0] << std::endl;

    std::cout << "temp = " << stBoardStatusData.aiTemp[0] << std::endl;

    __StopTime();

    return;
}

void
__BoardResetType(uint32_t uiResetType)
{
    int32_t iSta;

    __StartTime();
    std::cout << "set reset type " << uiResetType << "." << std::endl;

    iSta = CDevApiManager::GetInstance().GetBoardApi().SetResetType(uiResetType);
    if (OPLK_OK != iSta) {
        std::cout << "set reset type error. iSta = " << iSta << std::endl;
    }
    __StopTime();

    return;
}

void
__BoardSetApsdEnable(bool bEnable)
{
    int32_t iSta;

    __StartTime();
    std::string staEnable = (true == bEnable ? "Enable Apsd." : "Disable Apsd.");
    std::cout << staEnable << std::endl;

    iSta = CDevApiManager::GetInstance().GetBoardApi().SetApsdEnable(0, bEnable);
    if (OPLK_OK != iSta) {
        std::cout << "set apsd error. iSta = " << iSta << std::endl;
    }
    __StopTime();

    return;
}

void
__BoardCfgWatchDog(bool bEnable, uint32_t uiTimeout)
{
    int32_t iSta;

    __StartTime();
    std::string staEnable = (true == bEnable ? "Enable." : "Disable.");
    std::cout << "WatchDog " << staEnable << std::endl;
    std::cout << "Timeout = " << uiTimeout << std::endl;

    iSta = CDevApiManager::GetInstance().GetBoardApi().CfgWatchDog(bEnable, uiTimeout);
    if (OPLK_OK != iSta) {
        std::cout << "set apsd error. iSta = " << iSta << std::endl;
    }
    __StopTime();

    return;
}

void
__BoardGetChassisCfg()
{
    int32_t iSta;

    __StartTime();
    std::cout << "get Chassis cfg." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetChassisCfg(4, "/tmp/test.tgz")) != OPLK_OK) {
        std::cout << "get chassis cfg error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    }
    __StopTime();

    return;
}

void
__BoardSetChassisCfg()
{
    int32_t iSta;

    __StartTime();
    std::cout << "set Chassis cfg." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetChassisCfg(4, 0, "/tmp/test.tgz")) != OPLK_OK) {
        std::cout << "set chassis cfg error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    }
    __StopTime();

    return;
}


void
__BoardGetSccCfg()
{
    int32_t iSta;

    __StartTime();
    std::cout << "get scc cfg." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetSccCfg(8, "/tmp/test1.tgz")) != OPLK_OK) {
        std::cout << "get scc cfg error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    }
    __StopTime();

    return;
}

void
__BoardSetSccCfg()
{
    int32_t iSta;

    __StartTime();
    std::cout << "set scc cfg." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetSccCfg(8, "/tmp/test.tgz")) != OPLK_OK) {
        std::cout << "get scc cfg error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    }
    __StopTime();

    return;
}

void
__SetSccMfg()
{
    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetSccMfg("/home/oplink/log/mfginfo.conf")) != OPLK_OK) {
        std::cout << "set scc mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set scc mfg done." << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
}


void
__I2cTest(uint32_t uiPsuIdx, uint32_t uiI2cIdx)
{
    int32_t iSta = CDevApiManager::GetInstance().GetBoardApi().I2cTest(uiPsuIdx, uiI2cIdx);

    if (iSta != OPLK_OK) {
        std::cout << "i2c test error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "i2c test done." << std::endl;
        std::cout << std::endl;
    }
}

void
__SetChassisMfg(char *pcFileName)
{
    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetChassisMfg(pcFileName)) != OPLK_OK) {
        std::cout << "set chassis mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set chassis mfg done." << std::endl;
        std::cout << std::endl;
    }
}

void
__BoardEEpromRead(uint32_t uiIdx)
{
    int32_t iSta;
    uint8_t aucBuf[64] = {0xff};

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().EepromRead(uiIdx, 32, 64, aucBuf)) != OPLK_OK) {
        std::cout << "EepromRead error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        /* printf("mfg: %s\n", aucBuf); */
        std::string mfg((char *)aucBuf);
        std::cout << mfg << std::endl;

        #if 1
        for (uint32_t uiIdx = 0; uiIdx < 64; uiIdx++) {
            printf("%02x ", aucBuf[uiIdx]);

            if ((uiIdx + 1) % 32 == 0) {
                printf("\n");
            }
        }

        printf("\n");
        #endif
    }
}

void
__BoardEEpromWrite(uint32_t uiIdx, uint8_t *pucBuf)
{
    int32_t iSta;


    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().EepromWrite(uiIdx, 32, strlen((char *)pucBuf), pucBuf)) != OPLK_OK) {
        std::cout << "EepromWrite error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "EepromWrite done." << std::endl;
    }
}




void
__BoardTest(char *argv[])
{
    std::vector<std::string> staCmdVec = {"i2c-test", "mfg", "data", "set-cfg", "eeprom-read", "eeprom-write"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);

    switch (iCmdIdx) {
    case 0:
    {
        uint32_t uiPsuIdx = atoi(argv[3]);
        uint32_t uiI2cIdx = atoi(argv[4]);
        __I2cTest(uiPsuIdx, uiI2cIdx);
        break;
    }

    case 1:
        __SetChassisMfg(argv[3]);
        break;

    case 2:
        __BoardGetStatusData();
        break;

    case 3:
        __BoardGetChassisCfg();
        while (1) {
            __BoardSetChassisCfg();
        }
        break;

    case 4:
    {
        uint32_t uiEEpromIdx = atoi(argv[3]);
        __BoardEEpromRead(uiEEpromIdx);
        break;
    }

    case 5:
    {
        uint32_t uiEEpromIdx = atoi(argv[3]);
        __BoardEEpromWrite(uiEEpromIdx, (uint8_t *)argv[4]);
        break;
    }

    default:
        __PrintUnknownCmd(iCmdIdx);
        break;
    }

    return;

#if 0
    __SetSccMfg();

#endif

#if 1
    __BoardGetStatusData();
#endif

#if 0
    __BoardSetApsdEnable(false);
#endif

#if 0
    __BoardGetMfg();

    __BoardResetType(1);
    __BoardSetApsdEnable(false);
    __BoardCfgWatchDog(false, 0x0E);
    CDelay::Delay(1, S);
    __BoardGetStatusData();

    __BoardResetType(3);
    __BoardSetApsdEnable(true);
    __BoardCfgWatchDog(true, 0x0F);
    CDelay::Delay(1, S);
#endif
    return;


}

void
__SlotStatusData(uint32_t uiIdx)
{
    int32_t iSta;
    CSlotStatusData stData = {0};

    iSta = CDevApiManager::GetInstance().GetSlotApi().GetStatusData(uiIdx, stData);
    if (OPLK_OK != iSta) {
        std::cout << "set slot status data error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

    std::cout << "EEprom Alarm: " << stData.bEEpromAlarm << std::endl;

    for (int32_t uiLedIdx = 0; uiLedIdx < 2; uiLedIdx++) {
        std::cout << "Led " << uiLedIdx << ": " << stData.astLed[uiLedIdx].uiState << std::endl;
    }

}

void
__SlotSetOnline()
{
    int32_t iSta;

    iSta = CDevApiManager::GetInstance().GetSlotApi().SetOnline(SLOT1, true);
    if (OPLK_OK != iSta) {
        std::cout << "set slot online error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

    std::cout << "set slot online done." << std::endl;

}

void
__SlotLed(uint32_t uiIdx)
{
    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetLedState(uiIdx, LED0, LED_STATUS_RED_BLINK)) != OPLK_OK) {
        std::cout << "set pa system led error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led green done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetLedState(uiIdx, LED1, LED_STATUS_GREEN_BLINK)) != OPLK_OK) {
        std::cout << "set pa system led error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led green done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetLedState(uiIdx, LED0, LED_STATUS_GREEN_BLINK)) != OPLK_OK) {
        std::cout << "set pa system red error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led red done." << iSta << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetLedState(uiIdx, LED1, LED_STATUS_RED_BLINK)) != OPLK_OK) {
        std::cout << "set pa system red error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa system led red done." << iSta << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);

}

void
__SlotSetMfg()
{
    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetMfg(SLOT0, "/home/oplink/log/mfginfo_slot.conf")) != OPLK_OK) {
        std::cout << "set pa mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa mfg done." << std::endl;
        std::cout << std::endl;
    }

}

void
__SlotGetMfg()
{
    CMfg stMfg;

    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetMfg(SLOT0, stMfg)) != OPLK_OK) {
        std::cout << "get ba mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ba mfg done." << std::endl;
        std::cout << "Manufacturer: " << stMfg.acManufacturer << std::endl;
        std::cout << "SN: " << stMfg.acSN << std::endl;
        std::cout << "PN: " << stMfg.acPN << std::endl;
        std::cout << "MfgDate: " << stMfg.acMfgDate << std::endl;
        std::cout << std::endl;
    }
}

void
__SlotReset(uint32_t uiIdx, uint32_t uiResetType)
{
    int32_t iSta = CDevApiManager::GetInstance().GetSlotApi().Reset(uiIdx, uiResetType);

    if (iSta != OPLK_OK) {
        std::cout << "Reset error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "Reset done. " << std::endl;
        std::cout << std::endl;
    }
}

void
__SlotI2cTest(uint32_t uiIdx)
{
    uint8_t acTmp[128] = {0};
    int32_t iSta = CDevApiManager::GetInstance().GetSlotApi().EepromRead(uiIdx, 0, 8, acTmp);

    if (iSta != OPLK_OK) {
        std::cout << "EepromRead error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "EepromRead done. " << std::endl;
        std::cout << std::endl;
    }

    for (uint32_t uiIdx = 0; uiIdx < 8; uiIdx++) {
        printf("%02x ", acTmp[uiIdx]);

        if ((uiIdx + 1) % 32 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}


void
__SlotTest(char *argv[])
{
    std::vector<std::string> staCmdVec = {"reset", "iic-test", "led", "data"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);

    /*uint32_t uiDevIdx = atoi(argv[3]); */

    /*CMfg stMfg; */
    CSlotInfo stInfo;
    int32_t iSta;

    uint32_t uiIdx = atoi(argv[3]);

    switch (iCmdIdx) {
    case 0:
    {

        uint32_t uiResetType = atoi(argv[4]);
        __SlotReset(uiIdx, uiResetType);
        break;
    }

    case 1:
        __SlotI2cTest(uiIdx);
        break;

    case 2:
        __SlotLed(uiIdx);
        break;

    case 3:
        __SlotStatusData(uiIdx);
        break;

    default:
        std::cout << "Unknown Command. iCmdIdx = " << iCmdIdx << std::endl << std::endl;
    }

    return;

#if 0
    __SlotLed();
#endif

#if 0
    __SlotSetMfg();
#endif

#if 0
    __SlotGetMfg();
#endif

#if 1
    /*COaStatusData stEdfaStatus; */

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT0, stInfo)) != OPLK_OK) {
        std::cout << "get slot info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get slot info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        for (int i = 0; i < (int)stInfo.uiDevNum; ++i) {
            std::cout << "uiDevType: " << stInfo.astDevInfo[i].uiDevType << std::endl;
            std::cout << "uiFuncType: " << stInfo.astDevInfo[i].uiFuncType << std::endl;
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[i].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
#endif

#if 0
    CDelay::Delay(1, S);

    CDevApiManager::GetInstance().GetOaApi().GetStatusData(stInfo.astDevInfo[0].uiDevIdx, stEdfaStatus);

    std::cout << "Rx: " << stEdfaStatus.stOaCurData.asExtPdPower[7] << std::endl;
    std::cout << "Tx: " << stEdfaStatus.stOaCurData.asExtPdPower[9] << std::endl;
    std::cout << "Add: " << stEdfaStatus.stOaCurData.asExtPdPower[9] << std::endl;
    std::cout << "Drop: " << stEdfaStatus.stOaCurData.asExtPdPower[7] << std::endl;


    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT3, stInfo)) != OPLK_OK) {
        std::cout << "get ba info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ba info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);

    CDevApiManager::GetInstance().GetOaApi().GetStatusData(stInfo.astDevInfo[0].uiDevIdx, stEdfaStatus);

    std::cout << "Rx: " << stEdfaStatus.stOaCurData.asExtPdPower[7] << std::endl;
    std::cout << "Tx: " << stEdfaStatus.stOaCurData.asExtPdPower[9] << std::endl;
    std::cout << "Add: " << stEdfaStatus.stOaCurData.asExtPdPower[9] << std::endl;
    std::cout << "Drop: " << stEdfaStatus.stOaCurData.asExtPdPower[7] << std::endl;


    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT3, stInfo)) != OPLK_OK) {
        std::cout << "get ba info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ba info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetMfg(stInfo.astDevInfo[0].uiDevIdx, stMfg)) != OPLK_OK) {
        std::cout << "OA0: get mfg err. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "OA0: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT2, stInfo)) != OPLK_OK) {
        std::cout << "get pa info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get pa info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetMfg(stInfo.astDevInfo[0].uiDevIdx, stMfg)) != OPLK_OK) {
        std::cout << "OA1: get mfg err. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "OA1: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);





    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT3, stInfo)) != OPLK_OK) {
        std::cout << "get ba info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ba info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT2, stInfo)) != OPLK_OK) {
        std::cout << "get pa info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get pa info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetOaApi().GetMfg(stInfo.astDevInfo[0].uiDevIdx, stMfg)) != OPLK_OK) {
        std::cout << "OA1: get mfg err. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "OA1: " << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);





    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetSlotNum(uiSlotNum)) != OPLK_OK) {
        std::cout << "get slot num error. errorNum = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "slot num = " << uiSlotNum << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);



#if 0
    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetMfg(3, "/tmp/mfginfo_board_ocm.conf")) != OPLK_OK) {
        std::cout << "set ocm mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ocm mfg done." << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
#else
    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT0, stInfo)) != OPLK_OK) {
        std::cout << "get ocm slot info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ocm slot info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);


    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetInfo(SLOT1, stInfo)) != OPLK_OK) {
        std::cout << "get ops slot info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ops slot info done." << std::endl;
        std::cout << "cardtype: " << stInfo.uiCardType << std::endl;
        std::cout << "uiSlotState: " << stInfo.uiSlotState << std::endl;
        std::cout << "uiResetType: " << stInfo.uiResetType << std::endl;
        std::cout << "uiDevNum: " << stInfo.uiDevNum << std::endl;
        if (stInfo.uiDevNum > 0) {
            std::cout << "uiDevIdx: " << stInfo.astDevInfo[0].uiDevIdx << std::endl;
        }
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetOcmApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "get ocm module mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ocm module mfg done." << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;

        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
#endif

#if 0
    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetMfg(SLOT3, "/tmp/mfginfo_board_ba.conf")) != OPLK_OK) {
        std::cout << "set ba mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set ba mfg done." << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetMfg(SLOT2, "/tmp/mfginfo_board_pa.conf")) != OPLK_OK) {
        std::cout << "set pa mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set pa mfg done." << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);
#else


    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetMfg(SLOT3, stMfg)) != OPLK_OK) {
        std::cout << "get ba mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get ba mfg done." << std::endl;
        std::cout << "Manufacturer: " << stMfg.acManufacturer << std::endl;
        std::cout << "SN: " << stMfg.acSN << std::endl;
        std::cout << "PN: " << stMfg.acPN << std::endl;
        std::cout << "MfgDate: " << stMfg.acMfgDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().GetMfg(SLOT2, stMfg)) != OPLK_OK) {
        std::cout << "get pa mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get pa mfg done." << std::endl;
        std::cout << "Manufacturer: " << stMfg.acManufacturer << std::endl;
        std::cout << "SN: " << stMfg.acSN << std::endl;
        std::cout << "PN: " << stMfg.acPN << std::endl;
        std::cout << "MfgDate: " << stMfg.acMfgDate << std::endl;
        std::cout << std::endl;
    }
    CDelay::Delay(1, S);

#endif

#endif

}

void
__WaitLogicUpgardeDone()
{
    int32_t iSta;

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetBoardApi().GetUpgadeStatus();
        std::cout << "Board iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }

    return;
}

void
__LogicUpgrade(uint32_t uiIdx, char *pcFileName)
{
    int32_t iSta;

    std::cout << "idx = " << uiIdx << " FileName = " << pcFileName << std::endl;
    iSta = CDevApiManager::GetInstance().GetBoardApi().LogicUpgrade(uiIdx, pcFileName);

    if (iSta != OPLK_OK) {
        std::cout << "LogicUpgrade error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    }

}

void
__LogicReset(uint32_t uiIdx)
{
    __StartTime();

    if (CDevApiManager::GetInstance().GetBoardApi().LogicReset(uiIdx, 1) != OPLK_OK) {
        std::cout << "Upgrade error." << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);

    __WaitLogicUpgardeDone();

    __StopTime();
}

void
__LogicVer()
{

    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

    CDevApiManager::GetInstance().GetBoardApi().GetChassisMfg(stMfg);

    std::cout << "Cpld0 Ver: " << stMfg.acCpld1Ver << std::endl;
    std::cout << "Cpld1 Ver: " << stMfg.acCpld2Ver << std::endl;
}

void
__LogicUpgradeReset(uint32_t uiIdx, char *pcFileName)
{

    __LogicVer();

#if 1
    __StartTime();

    __LogicUpgrade(uiIdx, pcFileName);

    CDelay::Delay(1, S);

    __WaitLogicUpgardeDone();

#if 0
    __LogicReset(uiIdx);


    CDelay::Delay(1, S);

    __WaitLogicUpgardeDone();
#endif
    __LogicVer();

    __StopTime();
#endif
}

void
__UpgradeResetTest()
{
    int rc = 0;
#if 0
    uint8_t aucBuf[16] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t aucRdBuf[16];
    rc = CDevApiManager::GetInstance().GetBoardApi().SetUpgInfo(aucBuf, sizeof(aucBuf));
    if (rc != OPLK_OK) {
        std::cout << "set upg info error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    memset(aucRdBuf, 0, sizeof(aucRdBuf));
    rc = CDevApiManager::GetInstance().GetBoardApi().GetUpgInfo(aucRdBuf, sizeof(aucRdBuf));
    if (rc != OPLK_OK) {
        std::cout << "get upg info error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    for (int i = 0; i < (int)sizeof(aucRdBuf); i++) {
        printf("%d ", aucRdBuf[i]);
        /* std::cout << i << ": " << aucRdBuf[i]; */
        std::cout << std::endl;
    }
#endif

    /* extend upgrade */

    rc = CDevApiManager::GetInstance().GetExtendApi().Upgrade(0, "/tmp/extend.bin");
    if (rc != OPLK_OK) {
        std::cout << "upgrade error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    sleep(1);
    CExtendStatusData st1Data;
    rc = CDevApiManager::GetInstance().GetExtendApi().GetStatusData(0, st1Data);
    if (rc != OPLK_OK) {
        std::cout << "Get status error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }


    rc = CDevApiManager::GetInstance().GetExtendApi().Upgrade(0, "/tmp/extend.bin");
    if (rc != OPLK_OK) {
        std::cout << "upgrade error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }


    while (1) {
        rc = CDevApiManager::GetInstance().GetExtendApi().GetUpgadeStatus(0);
        std::cout << " rc = " << rc << std::endl;
        if (0 == rc) {
            break;
        }

        sleep(2);

        rc = CDevApiManager::GetInstance().GetExtendApi().GetStatusData(0, st1Data);
        if (rc != OPLK_OK) {
            std::cout << "Get status error. rc = " << rc << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << " Upgrade Done " << std::endl;

    rc = CDevApiManager::GetInstance().GetExtendApi().Reset(0, RST_MODE_WARM);
    if (rc != OPLK_OK) {
        std::cout << "reset error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    while (1) {
        rc = CDevApiManager::GetInstance().GetExtendApi().GetUpgadeStatus(0);
        std::cout << " rc = " << rc << std::endl;
        if (0 == rc) {
            break;
        }

        sleep(2);

        rc = CDevApiManager::GetInstance().GetExtendApi().GetStatusData(0, st1Data);
        if (rc != OPLK_OK) {
            std::cout << "Get status error. rc = " << rc << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << " Reset Done " << std::endl;

    return;

    /* OA upgrade */

    rc = CDevApiManager::GetInstance().GetOaApi().Upgrade(1, "/tmp/edfa_fw.bin");
    if (rc != OPLK_OK) {
        std::cout << "upgrade error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    sleep(1);
    COaStatusData stData;
    rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(0, stData, 0);
    if (rc != OPLK_OK) {
        std::cout << "Get status error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }


    rc = CDevApiManager::GetInstance().GetOaApi().Upgrade(1, "/tmp/edfa_fw.bin");
    if (rc != OPLK_OK) {
        std::cout << "upgrade error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }


    sleep(1);
    rc = CDevApiManager::GetInstance().GetOaApi().SetGain(1, 17, 0);
    if (rc != OPLK_OK) {
        std::cout << "set gain error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    rc = CDevApiManager::GetInstance().GetOaApi().SetGain(0, 17, 0);
    if (rc != OPLK_OK) {
        std::cout << "set gain error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    while (1) {
        rc = CDevApiManager::GetInstance().GetOaApi().GetUpgadeStatus(1);
        std::cout << " rc = " << rc << std::endl;
        if (0 == rc) {
            break;
        }

        sleep(2);

        rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(1, stData, 0);
        if (rc != OPLK_OK) {
            std::cout << "Get status error. rc = " << rc << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << " Upgrade Done " << std::endl;

    rc = CDevApiManager::GetInstance().GetOaApi().Reset(1, RST_MODE_WARM);
    if (rc != OPLK_OK) {
        std::cout << "reset error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    sleep(1);
    rc = CDevApiManager::GetInstance().GetOaApi().SetGain(1, 17, 0);
    if (rc != OPLK_OK) {
        std::cout << "set gain error. rc = " << rc << std::endl;
        std::cout << std::endl;
    }

    while (1) {
        rc = CDevApiManager::GetInstance().GetOaApi().GetUpgadeStatus(1);
        std::cout << " rc = " << rc << std::endl;
        if (0 == rc) {
            break;
        }

        sleep(2);

        rc = CDevApiManager::GetInstance().GetOaApi().GetStatusData(1, stData, 0);
        if (rc != OPLK_OK) {
            std::cout << "Get status error. rc = " << rc << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << " Reset Done " << std::endl;

    /* CDevApiManager::GetInstance().GetBoardApi().LogicUpgrade(0, "/tmp/logic_main.bin"); */
    /* CDevApiManager::GetInstance().GetBoardApi().LogicReset(0, 0); */
    /* CDevApiManager::GetInstance().GetOaApi().Upgrade(0, "/tmp/edfa_fw.bin"); */
    /* CDevApiManager::GetInstance().GetOcmApi().Upgrade(0, "/tmp/ocm.bin"); */
    /* CDevApiManager::GetInstance().GetOpsApi().Upgrade(0, "/tmp/ops.bin"); */

}

void
__LogicTest(uint32_t uiCmd, char *argv[])
{
    uint32_t uiIdx = atoi(argv[3]);

    switch (uiCmd) {
    case 0:
        __LogicUpgradeReset(uiIdx, argv[4]);
        break;

    case 1:
        __LogicReset(uiIdx);
        break;

    default:
        std::cout << "Unknown Command. uiCmd = " << uiCmd << std::endl;
        break;
    }
}

void
__MuxTest()
{
    int32_t iSta;
    /* CMfg stMfg = {0}; */
    CMuxStatusData stStatus = {0};
    CMuxCaliData stCali = {0};

#if 0
    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetOnline(SLOT1, true)) != OPLK_OK) {
        std::cout << "set slot online error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set slot online done." << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(10, S);

    if ((iSta = CDevApiManager::GetInstance().GetMuxApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "get status data error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get mux mfg done." << std::endl;
        std::cout << "Manufacturer: " << stMfg.acManufacturer << std::endl;
        std::cout << "SN: " << stMfg.acSN << std::endl;
        std::cout << "PN: " << stMfg.acPN << std::endl;
        std::cout << "MfgDate: " << stMfg.acMfgDate << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);
#endif
    if ((iSta = CDevApiManager::GetInstance().GetMuxApi().GetCali(0, stCali)) != OPLK_OK) {
        std::cout << "get status data error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "get mux cali done." << std::endl;
        std::cout << "dRefMuxTCali: " << stCali.dRefMuxTCali << std::endl;
        std::cout << "dRefDemuxRCali: " << stCali.dRefDemuxRCali << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetMuxApi().GetStatusData(0, stStatus)) != OPLK_OK) {
        std::cout << "get status data error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "Pd0: " << stStatus.asPdPower[0] << std::endl;
        std::cout << "Pd1: " << stStatus.asPdPower[1] << std::endl;
        std::cout << std::endl;
    }

#if 0
    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetSlotApi().SetOnline(SLOT1, false)) != OPLK_OK) {
        std::cout << "set slot online error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set slot online done." << std::endl;
        std::cout << std::endl;
    }
#endif
}

typedef struct {
    uint8_t ucState;                                                        /* Refer to EN_UPGD_STATE                                  */
    uint8_t ucPrimary;                                                      /* Refer to EN_UPGD_BANK                                   */


    uint8_t aucResv[10];
    uint32_t ulCrc32;


} ST_UPGD_CTRL;

void
__EEpromUpgInfoWrite(ST_UPGD_CTRL &stData)
{
    int32_t iSta;

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetUpgInfo((uint8_t *)&stData, sizeof(stData))) != OPLK_OK) {
        std::cout << "set upgrade info error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set upgrade info done." << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);
}

void
__EEpromUpgInfoRead()
{
    int32_t iSta;

    ST_UPGD_CTRL stData;
    memset(&stData, 0, sizeof(stData));

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetUpgInfo((uint8_t *)&stData, sizeof(stData))) != OPLK_OK) {
        std::cout << "SetUpgInfo error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "stData.ucPrimary = " << (int)stData.ucPrimary << std::endl;
        std::cout << "stData.ucState = " << (int)stData.ucState << std::endl;
        std::cout << std::endl;
    }

    /* CDelay::Delay(1, S); */
}

void
__EEpromWrite()
{
    /*int32_t iSta; */


    ST_UPGD_CTRL stData;
    memset(&stData, 0, sizeof(stData));

    stData.ucState = 1;
    stData.ucPrimary = 2;
    __EEpromUpgInfoWrite(stData);

    while (true) {


#if 0


        __EEpromUpgInfoRead();



        stData.ucState = 0;
        stData.ucPrimary = 0;
        __EEpromUpgInfoWrite(stData);

        CDelay::Delay(1, S);
#else
        memset(&stData, 0, sizeof(stData));

        __EEpromUpgInfoRead();
#endif

        CDelay::Delay(10, MS);

    }

#if 0

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetChassisCali("/tmp/LossCalibration.conf")) != OPLK_OK) {
        std::cout << "set chassis cali error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set chassis cali done. " << stData.ucPrimary << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().SetChassisMfg("/tmp/mfginfo.conf")) != OPLK_OK) {
        std::cout << "set chassis mfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "set chassis mfg done." << std::endl;
        std::cout << std::endl;
    }
#endif
    return;

}

void
__EEpromRead()
{
    int32_t iSta;

    CCali stCali;
    memset(&stCali, 0, sizeof(stCali));

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetChassisCali(stCali)) != OPLK_OK) {
        std::cout << "GetChassisCali error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "stCali.sLineInToPaIn = " << stCali.sLineInToPaIn << std::endl;
        std::cout << "stCali.sLineInToBaIn = " << stCali.sLineInToBaIn << std::endl;
        std::cout << "stCali.sPaOutToLineOut = " << stCali.sPaOutToLineOut << std::endl;
        std::cout << "stCali.sBaOutToLineOut = " << stCali.sBaOutToLineOut << std::endl;
        std::cout << std::endl;
    }

    CDelay::Delay(1, S);

    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

    if ((iSta = CDevApiManager::GetInstance().GetBoardApi().GetChassisMfg(stMfg)) != OPLK_OK) {
        std::cout << "GetChassisMfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "stMfg.acModuleType = " << stMfg.acModuleType << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << "stMfg.acEth0Mac = " << stMfg.acEth0Mac << std::endl;
        std::cout << std::endl;
    }

}



void
__LogicRead(uint32_t uiIdx, uint32_t uiAddr)
{

    /*int32_t iSta; */
    uint8_t ucValue;

    if (CDevApiManager::GetInstance().GetBoardApi().LogicRead(uiIdx, uiAddr, 1, &ucValue) != OPLK_OK) {
        std::cout << "logic read error." << std::endl;
        std::cout << std::endl;
    }

    std::cout << uiAddr << ": " << (uint32_t)ucValue << std::endl;

    return;
}

void
__LogicWrite(uint32_t uiIdx, uint32_t uiAddr, uint8_t ucValue)
{
    /*int32_t iSta; */

    if (CDevApiManager::GetInstance().GetBoardApi().LogicWrite(uiIdx, uiAddr, 1, &ucValue) != OPLK_OK) {
        std::cout << "logic read error." << std::endl;
        std::cout << std::endl;
    }

    std::cout << (uint32_t)ucValue << std::endl;

    return;
}


void
__WssGetCfg(uint32_t uiIdx)
{
    int32_t iSta;
    CWssAllCfg stCfg;

    iSta = CDevApiManager::GetInstance().GetWssApi().GetCfgData(uiIdx, stCfg);
    if (OPLK_OK != iSta) {
        std::cout << "__WssGetCfg error. iSta = " << iSta << std::endl;
        return;
    }

    std::cout << "__WssGetCfg " << uiIdx << " done." << std::endl;

#if 0
    iSta = CDevApiManager::GetInstance().GetWssApi().GetCfgData(1, stCfg);
    if (OPLK_OK != iSta) {
        std::cout << "__WssGetCfg error. iSta = " << iSta << std::endl;
    }

    std::cout << "__WssGetCfg 0 done." << std::endl;
#endif


#if 1
    for (uint32_t i = 0; i < 4; ++i) {
        if (1 /*i == 0 || i == 1*/) {
            printf("Id = %d, CenterFreq = %d, Width = %d\n",
                   stCfg.astWavePlan[i].uiChId,
                   stCfg.astWavePlan[i].uiCenterFreq,
                   stCfg.astWavePlan[i].usSliceCount);
        }
    }
    printf("\r\n");
#endif

#if 1
    printf("Cfg Add:\r\n");

    for (int i = 0; i < 4; i++) {
        if (1 /*i == 0 || i == 1*/) {
            printf("Id = %d, ComPort = %d, ExpPort = %d, Atten = %d\n",
                   stCfg.astWssChCfgAdd[i].uiId,
                   stCfg.astWssChCfgAdd[i].uiComPort,
                   stCfg.astWssChCfgAdd[i].uiExpPort,
                   stCfg.astWssChCfgAdd[i].uiAtten);
        }
    }

    printf("\r\n");
#endif

#if 1
    printf("Cfg Drop:\r\n");
    for (int i = 0; i < 4; i++) {

        if (1 /*i == 0 || i == 1*/) {
            printf("Id = %d, ComPort = %d, ExpPort = %d, Atten = %d\n",
                   stCfg.astWssChCfgDrop[i].uiId,
                   stCfg.astWssChCfgDrop[i].uiComPort,
                   stCfg.astWssChCfgDrop[i].uiExpPort,
                   stCfg.astWssChCfgDrop[i].uiAtten);
        }
    }
    printf("\r\n");
#endif
}

void
__WssSetWavePlan()
{
    int32_t iSta;
    CWavePlan astWavePlan[WSS_CH_CNT_TOTAL];
    uint32_t uiChId = 0;
    uint32_t uiBandWidth = 16;
    uint32_t uiCenterFreq = 30672; /* uiStartFreq + (uiBandWidth / 2); */
    /*uint32_t uiChCnt = 4; */
    /*bool bConfig = true; */

    std::cout << "__WssSetWavePlan +++" << std::endl;

    memset(&astWavePlan, 0, sizeof(astWavePlan));

    for (int i = 0; i < 387; ++i) {
        uiChId = i + 1;
        astWavePlan[i].uiChId = uiChId;

        astWavePlan[i].uiCenterFreq = 0; /* uiCenterFreq + i * uiBandWidth; */
        astWavePlan[i].usSliceCount = 0; /* uiBandWidth; */
    }

    for (int i = 0; i < 4; ++i) {
        uiChId = i + 1;
        astWavePlan[i].uiChId = uiChId;

        astWavePlan[i].uiCenterFreq = uiCenterFreq + i * uiBandWidth;
        astWavePlan[i].usSliceCount = uiBandWidth;

        printf("Id = %d, CenterFreq = %d\n", uiChId, astWavePlan[i].uiCenterFreq);
    }


    iSta = CDevApiManager::GetInstance().GetWssApi().SetWavePlan(0, 0, WSS_CH_CNT_TOTAL, astWavePlan);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetWavePlan error. iSta = " << iSta << std::endl;
    }

    iSta = CDevApiManager::GetInstance().GetWssApi().SetWavePlan(1, 0, WSS_CH_CNT_TOTAL, astWavePlan);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetWavePlan error. iSta = " << iSta << std::endl;
    }

    std::cout << "__WssSetWavePlan --- " << std::endl;
}

void
__WssClearWavePlan()
{

    int32_t iSta;
    /*CWavePlan astWavePlan[387]; */
    uint32_t uiChId = 0;
    uint32_t uiMaxCnt = WSS_CH_CNT_TOTAL;
    uint32_t uiChCnt = uiMaxCnt;
    CWavePlan astWavePlan[WSS_CH_CNT_TOTAL];

    std::cout << "__WssSetWavePlan +++" << std::endl;

    memset(&astWavePlan, 0, sizeof(astWavePlan));

    CWavePlan *pstWavePlan = astWavePlan;

    for (int i = 0; i < (int)uiChCnt; ++i) {
        uiChId = i + 1;
        pstWavePlan->uiChId = uiChId;

        pstWavePlan->uiCenterFreq = 0;
        pstWavePlan->usSliceCount = 0;

        pstWavePlan++;
    }

    iSta = CDevApiManager::GetInstance().GetWssApi().SetWavePlan(0, 0, uiChCnt, astWavePlan);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetWavePlan error. iSta = " << iSta << std::endl;
    }

    iSta = CDevApiManager::GetInstance().GetWssApi().SetWavePlan(1, 0, uiChCnt, astWavePlan);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetWavePlan error. iSta = " << iSta << std::endl;
    }

    std::cout << "__WssSetWavePlan --- " << std::endl;

}
void
__WssSetCfg(uint32_t uiIdx)
{
    int32_t iSta;
    uint32_t uiChCnt = WSS_CH_CNT_TOTAL;
    /* CWssCfg astWssCfg[WSS_CH_MAX_NUM]; */
    CWssAllCfg stCfg = {0};

    memset(&stCfg, 0, sizeof(stCfg));

    CDevApiManager::GetInstance().GetWssApi().GetCfgData(uiIdx, stCfg);

    CWssCfg *pstAddCfg = stCfg.astWssChCfgAdd;
    CWssCfg *pstDropCfg = stCfg.astWssChCfgDrop;

#if 0
    for (int i = 0; i < uiChCnt; ++i) {
        pstCfg->uiId = i + 1;
        pstCfg->uiComPort = 0;
        pstCfg->uiExpPort = i + 1;
        pstCfg->uiAtten = 0;

        pstCfg++;
    }
#endif

    if (0 == uiIdx) {
        pstAddCfg[0].uiComPort = 0;
        pstAddCfg[0].uiExpPort = 0;
        pstAddCfg[0].uiAtten = 0;

        pstAddCfg[1].uiComPort = 0;
        pstAddCfg[1].uiExpPort = 0;
        pstAddCfg[1].uiAtten = 0;

        pstDropCfg[0].uiComPort = 0;
        pstDropCfg[0].uiExpPort = 1;
        pstDropCfg[0].uiAtten = 0;

        pstDropCfg[1].uiComPort = 0;
        pstDropCfg[1].uiExpPort = 32;
        pstDropCfg[1].uiAtten = 0;
    } else {
        pstAddCfg[2].uiComPort = 0;
        pstAddCfg[2].uiExpPort = 0;
        pstAddCfg[2].uiAtten = 0;

#if 1
        pstAddCfg[3].uiComPort = 0;
        pstAddCfg[3].uiExpPort = 0;
        pstAddCfg[3].uiAtten = 0;
#endif
        pstDropCfg[2].uiComPort = 0;
        pstDropCfg[2].uiExpPort = 1;
        pstDropCfg[2].uiAtten = 0;

#if 1
        pstDropCfg[3].uiComPort = 0;
        pstDropCfg[3].uiExpPort = 32;
        pstDropCfg[3].uiAtten = 0;
#endif
    }



    /* add */
    iSta = CDevApiManager::GetInstance().GetWssApi().SetAllChCfg(uiIdx, EN_WSS_DIR_ADD, uiChCnt, stCfg.astWssChCfgAdd);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetCfg error. iSta = " << iSta << std::endl;
    }


    /* drop */
    iSta = CDevApiManager::GetInstance().GetWssApi().SetAllChCfg(uiIdx, EN_WSS_DIR_DROP, uiChCnt, stCfg.astWssChCfgDrop);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetCfg error. iSta = " << iSta << std::endl;
    }


    std::cout << "__WssSetCfg Done." << std::endl;
}

void
__WssLoopbackTest(uint32_t uiIdx)
{
    int32_t iSta;
    uint32_t uiChCnt = WSS_CH_CNT_TOTAL;
    /* CWssCfg astWssCfg[WSS_CH_MAX_NUM]; */
    CWssAllCfg stCfg = {0};

    memset(&stCfg, 0, sizeof(stCfg));

    CDevApiManager::GetInstance().GetWssApi().GetCfgData(uiIdx, stCfg);

    CWssCfg *pstAddCfg = stCfg.astWssChCfgAdd;
    CWssCfg *pstDropCfg = stCfg.astWssChCfgDrop;

    if (0 == uiIdx) {
        pstAddCfg[0].uiComPort = 1;
        pstAddCfg[0].uiExpPort = 1;
        pstAddCfg[0].uiAtten = 0;

        pstDropCfg[0].uiComPort = 1;
        pstDropCfg[0].uiExpPort = 1;
        pstDropCfg[0].uiAtten = 0;

        pstAddCfg[1].uiComPort = 1;
        pstAddCfg[1].uiExpPort = 32;
        pstAddCfg[1].uiAtten = 0;

        pstDropCfg[1].uiComPort = 1;
        pstDropCfg[1].uiExpPort = 32;
        pstDropCfg[1].uiAtten = 0;

    } else {

        pstAddCfg[2].uiComPort = 1;
        pstAddCfg[2].uiExpPort = 1;
        pstAddCfg[2].uiAtten = 0;

        pstDropCfg[2].uiComPort = 1;
        pstDropCfg[2].uiExpPort = 1;
        pstDropCfg[2].uiAtten = 0;

        pstAddCfg[3].uiComPort = 1;
        pstAddCfg[3].uiExpPort = 32;
        pstAddCfg[3].uiAtten = 0;

        pstDropCfg[3].uiComPort = 1;
        pstDropCfg[3].uiExpPort = 32;
        pstDropCfg[3].uiAtten = 0;

    }


    /* add */
    iSta = CDevApiManager::GetInstance().GetWssApi().SetAllChCfg(uiIdx, EN_WSS_DIR_ADD, uiChCnt, stCfg.astWssChCfgAdd);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetCfg " << uiIdx << " : " << EN_WSS_DIR_ADD << " error. iSta = " << iSta << std::endl;
    }

    /* drop */
    iSta = CDevApiManager::GetInstance().GetWssApi().SetAllChCfg(uiIdx, EN_WSS_DIR_DROP, uiChCnt, stCfg.astWssChCfgDrop);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetCfg " << uiIdx << " : " << EN_WSS_DIR_ADD << " error. iSta = " << iSta << std::endl;
    }

    std::cout << "__WssSetCfg Done." << std::endl;
}


void
__WssUpgrade()
{
    int32_t iSta;
#if 0
    const char acFileName[] = "/home/oplink/log/nistica-minku-8.0.99-201027A.bin";
#else
    const char acFileName[] = "/home/oplink/log/nistica-minku-8.0.0-201027A.bin";
#endif

    iSta = CDevApiManager::GetInstance().GetWssApi().Upgrade(0, acFileName);
    if (OPLK_OK != iSta) {
        std::cout << "__WssSetWavePlan error. iSta = " << iSta << std::endl;
    }

    CDelay::Delay(3, S);

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetWssApi().GetUpgadeStatus(0);
        std::cout << "WSS iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }

    CDevApiManager::GetInstance().GetWssApi().Reset(0, RST_MODE_WARM);


}

void
__WssMfg(uint32_t uiIdx)
{
    int32_t iSta;

    CMfg stMfg;
    memset(&stMfg, 0, sizeof(stMfg));

    if ((iSta = CDevApiManager::GetInstance().GetWssApi().GetMfg(uiIdx, stMfg)) != OPLK_OK) {
        std::cout << "GetMfg error. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
    #if 0
        std::cout << "stMfg.acModuleType = " << stMfg.acModuleType << std::endl;
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << "stMfg.acEth0Mac = " << stMfg.acEth0Mac << std::endl;
        #endif

        std::cout << "WSS" << uiIdx + 1 << " SN: " << stMfg.acSN << std::endl;
        std::cout << std::endl;
    }
}

void
__WssReset(uint32_t uiIdx, uint32_t uiResetType)
{
    __WssGetCfg(uiIdx);

    CDevApiManager::GetInstance().GetWssApi().Reset(uiIdx, uiResetType);

    int32_t iSta = -1;
    while (iSta != OPLK_OK) {
        iSta = CDevApiManager::GetInstance().GetWssApi().GetUpgadeStatus(uiIdx);
        std::cout << "WSS iSta = " << iSta << std::endl;

        CDelay::Delay(1, S);
        continue;
    }

    __WssGetCfg(uiIdx);
}

void
__WssTest()
{
#if 0
    __WssMfg(0);
#endif

#if 0
    __WssMfg(1);
#endif


#if 0
    __WssClearWavePlan();
#endif

#if 0
    __WssSetWavePlan();
#endif

#if 0
    __WssSetCfg(0);
#endif

#if 0
    __WssSetCfg(1);
#endif

#if 0
    __WssLoopbackTest(0);
#endif

#if 0
    __WssLoopbackTest(1);
#endif

#if 0
    __WssGetCfg(0);
#endif

#if 0
    __WssGetCfg(1);
#endif

#if 0
    __WssUpgrade();
#endif

#if 1
    __WssReset(1, RST_MODE_WARM);
#endif
}

#if 1

void
__ColdResetTest()
{

#if 0
    __OaReset(0, RST_MODE_COLD);
#endif


#if 0
    CDevApiManager::GetInstance().GetWssApi().Reset(0, RST_MODE_COLD);
#endif

#if 0
    CDevApiManager::GetInstance().GetWssApi().Reset(0, RST_MODE_WARM);
#endif

#if 0
    CDevApiManager::GetInstance().GetWssApi().Reset(1, RST_MODE_COLD);
#endif

#if 1
    CDevApiManager::GetInstance().GetWssApi().Reset(1, RST_MODE_WARM);
#endif

#if 0
    CDevApiManager::GetInstance().GetOcmApi().Reset(0, RST_MODE_COLD);
#endif

#if 0
    CDevApiManager::GetInstance().GetOpsApi().Reset(0, RST_MODE_COLD);
#endif



}

void
__OcsMfg()
{
    CMfg stMfg;
    int32_t iSta;

    memset(&stMfg, 0x00, sizeof(stMfg));
    if ((iSta = CDevApiManager::GetInstance().GetOcsApi().GetMfg(0, stMfg)) != OPLK_OK) {
        std::cout << "OCS: get mfg err. iSta = " << iSta << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "OCS MFG: " << std::endl;
        std::cout << "Manufacturer:" << stMfg.acManufacturer << std::endl;
        std::cout << "Mod Type:" << stMfg.acModuleType << std::endl;
        /* std::cout << "ProductName:" << stMfg.acProductName << std::endl; */
        std::cout << "SN:" << stMfg.acSN << std::endl;
        std::cout << "PN:" << stMfg.acPN << std::endl;
        std::cout << "Boot Ver:" << stMfg.acBootVer << std::endl;
        std::cout << "Main Ver:" << stMfg.acFwVer << std::endl;
        std::cout << "HW Ver:" << stMfg.acHwVer << std::endl;
        std::cout << "MFG Date:" << stMfg.acMfgDate << std::endl;
        std::cout << "Cali Date:" << stMfg.acCaliDate << std::endl;
        std::cout << std::endl;
    }

    iSta = CDevApiManager::GetInstance().GetOcsApi().GetUpgadeStatus(0);
    std::cout << "Ocs iSta = " << iSta << std::endl;
}

void
__OcsCommFail()
{
    int32_t iSta = CDevApiManager::GetInstance().GetOcsApi().DbgSetCommFail(0);

    if (iSta != OPLK_OK) {
        std::cout << "Ocs DbgSetCommFail error." << iSta << std::endl;
    } else {
        std::cout << "Ocs DbgSetCommFail done." << std::endl;
    }
}

void
__OcsClearCommFail()
{
    int32_t iSta = CDevApiManager::GetInstance().GetOcsApi().DbgClearCommFail(0);

    if (iSta != OPLK_OK) {
        std::cout << "Ocs DbgClearCommFail error." << iSta << std::endl;
    } else {
        std::cout << "Ocs DbgClearCommFail done." << std::endl;
    }
}

void
__OcsReset(uint32_t uiResetType)
{
    int32_t iSta;
    int32_t iCount = 0;
    uint32_t uiDevIdx = 0;
    std::string staResetType = (uiResetType == RST_MODE_COLD ? "Cold" : "Warm");

    std::cout << staResetType << " Reset." << std::endl;
    if ((iSta = CDevApiManager::GetInstance().GetOcsApi().Reset(uiDevIdx, uiResetType)) != OPLK_OK) {
        std::cout << "Ocs" << " reset error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "Ocs" << " reset done." << std::endl;
    }

    iSta = -1;
    while (iSta != OPLK_OK) {

        iSta = CDevApiManager::GetInstance().GetOcsApi().GetUpgadeStatus(uiDevIdx);

        iCount++;
        CDelay::Delay(1, S);
        continue;

    }

    std::cout << "Ocs reset waiting for " << iCount << "s" << std::endl;

}

std::string
__OcsLedWorkStateStr(uint8_t ucLedSta)
{
    std::map<int, std::string> ledMap = {{0, "Normal work state"},
                                         {1, "Lamp test state"}};

    for (auto iter : ledMap) {
        if (iter.first == ucLedSta) {
            return iter.second;
        }
    }

    return "Unknown";
}

std::string
__OcsLedStateStr(uint8_t ucLedSta)
{
    const std::map<int, std::string> ledMap = {{0, "OFF"},
                                               {1, "Red"},
                                               {2, "Green"},
                                               {3, "Yellow"},
                                               {4, "Red blink, 1s ON, 1s OFF"},
                                               {5, "Green blink, 1s ON, 1s OFF"},
                                               {6, "Yellow blink, 1s ON, 1s OFF"},
                                               {7, "Red / Green blink, 1s red, 1s green"},
                                               {8, "Red / Yellow blink, 1s red, 1s yellow"},
                                               {9, "Green / Yellow blink, 1s green, 1s yellow"}};

    for (auto iter : ledMap) {
        if (iter.first == ucLedSta) {
            return iter.second;
        }
    }

    return "Unknown";
}


void
__OcsStatusData()
{
    uint32_t uiDevIdx = 0;
    COcsStatusData stData;

    memset(&stData, 0, sizeof(stData));

    if (CDevApiManager::GetInstance().GetOcsApi().GetStatusData(uiDevIdx, stData) != OPLK_OK) {
        std::cout << "GetStatusData error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "GetStatusData done." << uiDevIdx << std::endl;

        std::cout << "------switch channels total: " << stData.stConnsSta.uiCount << std::endl;
        std::cout << "    index     #IN   #OUT " << std::endl;
        for (uint32_t i = 0; i < stData.stConnsSta.uiCount; i++) {
            std::cout << "   " << i << "        " << (int)stData.stConnsSta.astConn[i].ucSource << "      " << (int)stData.stConnsSta.astConn[i].ucDestination << std::endl;
        }
        std::cout << "------alarms: " << std::endl;
        std::cout << "     Power supply voltage high alarm : " << stData.stAlm.abAlarms[0] << std::endl;
        std::cout << "      Power supply voltage low alarm : " << stData.stAlm.abAlarms[1] << std::endl;
        std::cout << "    Switch driver voltage high alarm : " << stData.stAlm.abAlarms[2] << std::endl;
        std::cout << "     Switch driver voltage low alarm : " << stData.stAlm.abAlarms[3] << std::endl;
        std::cout << "              Calibration data alarm : " << stData.stAlm.abAlarms[4] << std::endl;

        std::cout << "------led state: " << std::endl;
        std::cout << "     ucLedWorkSta : " << __OcsLedWorkStateStr(stData.stLedSta.ucLedWorkSta) << std::endl;
        std::cout << "     ACT LED : " << __OcsLedStateStr(stData.stLedSta.aucLedSta[0]) << std::endl;
        std::cout << "     ALM LED : " << __OcsLedStateStr(stData.stLedSta.aucLedSta[1]) << std::endl;

        std::cout << "------temperature state: " << std::endl;
        std::cout << "     sCaseTemp : " << (int)stData.stTempSta.sCaseTemp << std::endl;
    }

}

void
__OcsGetSwitchChannels()
{
    uint32_t uiDevIdx = 0;
    COcsConnections stConns;

    memset(&stConns, 0, sizeof(stConns));

    if (CDevApiManager::GetInstance().GetOcsApi().GetSwitchChannels(uiDevIdx, stConns) != OPLK_OK) {
        std::cout << "GetSwitchChannels error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "GetSwitchChannels done." << uiDevIdx << std::endl;

        std::cout << "------switch channels total: " << stConns.uiCount << std::endl;
        std::cout << "    index     #IN   #OUT " << std::endl;
        for (uint32_t i = 0; i < stConns.uiCount; i++) {
            std::cout << "   " << i << "        " << (int)stConns.astConn[i].ucSource << "      " << (int)stConns.astConn[i].ucDestination << std::endl;
        }
    }
}

void
__OcsSetSwitchChannels()
{
    int32_t iSta;
    uint32_t uiDevIdx = 0;
    COcsConnections stConns;

    memset(&stConns, 0, sizeof(stConns));
    stConns.uiCount = 16;
    for (uint32_t i = 0; i < stConns.uiCount; i++) {
        stConns.astConn[i].ucId = i;
        stConns.astConn[i].ucSource = i + 1;
        stConns.astConn[i].ucDestination = stConns.uiCount - i;
    }

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOcsApi().SetSwitchChannels(uiDevIdx, stConns)) != OPLK_OK) {
        std::cout << "OCS" << uiDevIdx << " set switch channels error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OCS" << uiDevIdx << " set switch channels done." << std::endl;
    }
    __StopTime();
}

void
__OcsSetSwitchChannel(int iInputChannel, int iOutputChannel)
{
    int32_t iSta;
    uint32_t uiDevIdx = 0;
    COcsConnection stConn;

    memset(&stConn, 0, sizeof(stConn));
    stConn.ucSource = iInputChannel;
    stConn.ucDestination = iOutputChannel;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOcsApi().SetSwitchChannel(uiDevIdx, stConn)) != OPLK_OK) {
        std::cout << "OCS" << uiDevIdx << " set switch channels error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OCS" << uiDevIdx << " set switch channels done." << std::endl;
    }
    __StopTime();
}

void
__OcsGetLedState()
{
    uint32_t uiDevIdx = 0;
    COcsLedSta stLedSta;

    memset(&stLedSta, 0, sizeof(stLedSta));

    if (CDevApiManager::GetInstance().GetOcsApi().GetLedState(uiDevIdx, stLedSta) != OPLK_OK) {
        std::cout << "GetLedState error." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "GetLedState done." << uiDevIdx << std::endl;

        std::cout << "------led state: " << std::endl;
        std::cout << "     ucLedWorkSta : " << __OcsLedWorkStateStr(stLedSta.ucLedWorkSta) << std::endl;
        std::cout << "     ACT LED : " << __OcsLedStateStr(stLedSta.aucLedSta[0]) << std::endl;
        std::cout << "     ALM LED : " << __OcsLedStateStr(stLedSta.aucLedSta[1]) << std::endl;
    }
}

void
__OcsSetLampTestState(int iLampTestState)
{
    int32_t iSta;
    uint32_t uiDevIdx = 0;

    __StartTime();
    if ((iSta = CDevApiManager::GetInstance().GetOcsApi().SetLampTestState(uiDevIdx, (0 == iLampTestState) ? false : true)) != OPLK_OK) {
        std::cout << "OCS" << uiDevIdx << " set lamp test state error. iSta = " << iSta << std::endl;
    } else {
        std::cout << "OCS" << uiDevIdx << " set lamp test state done." << std::endl;
    }
    __StopTime();
}

void
__OcsCommit(uint32_t uiIdx)
{
    int32_t iSta;

    while (1) {
        if ((iSta = CDevApiManager::GetInstance().GetOcsApi().Commit(uiIdx)) != OPLK_OK) {
            std::cout << "OCS" << uiIdx << " commit error." << iSta << std::endl;
            CDelay::Delay(1, S);
            continue;

        } else {
            std::cout << "OCS" << uiIdx << " commit done." << std::endl;
            break;
        }
    }
}

void
__OcsUpgrade(char *pcFilePath)
{
    int32_t iSta;
    int32_t i = 0;

#if 1
    if (NULL == pcFilePath) {
        std::cout << "Ocs upgrade fail. NULL == pcFilePath" << std::endl;
    }

    std::cout << "Ocs upgrade start, pcFilePath" << pcFilePath << std::endl;
    CDevApiManager::GetInstance().GetOcsApi().Upgrade(0, pcFilePath);
#if 0
    for (int i = 0; i < 30; i++) {
        CDelay::Delay(1, S);
        std::cout << "Ocs sleep " << i << std::endl;
    }
#endif

#endif

    CDelay::Delay(3, S);

    iSta = -1;
    iSta = CDevApiManager::GetInstance().GetOcsApi().GetUpgadeStatus(0);
    while (iSta != OPLK_OK) {
        std::cout << "Ocs iSta = " << iSta << " sleep " << ++i << " s" << std::endl;
        if ((iSta != ERR_UPGRADING) && (iSta != ERR_LOADING)) {
            std::cout << "Ocs upgrade abnormal!!! " << std::endl;
            break;
        }
        iSta = CDevApiManager::GetInstance().GetOcsApi().GetUpgadeStatus(0);
        CDelay::Delay(1, S);
        continue;
    }

    /* __OcsReset(RST_MODE_WARM); */

    __OcsMfg();
}

void
__OcsTest(char *argv[])
{
    std::vector<std::string> staCmdVec = {"mfg", "comm-fail", "clear-comm-fail", "reset", "status", "get-channels", "set-channels", "set-channel", "get-leds", "lamptest", "upgrade"};
    int32_t iCmdIdx = __CmdIdx(argv[2], staCmdVec);


    switch (iCmdIdx) {
    case 0:
        __OcsMfg();
        break;

    case 1:
        __OcsCommFail();
        break;

    case 2:
        __OcsClearCommFail();
        break;

    case 3:
        /* RST_MODE_WARM : 1, RST_MODE_COLD : 2 */
        __OcsReset(atoi(argv[3]));
        break;

    case 4:
        __OcsStatusData();
        break;

    case 5:
        __OcsGetSwitchChannels();
        break;
    case 6:
        __OcsSetSwitchChannels();
        break;

    case 7:
        __OcsSetSwitchChannel(atoi(argv[3]), atoi(argv[4]));
        break;

    case 8:
        __OcsGetLedState();
        break;
    case 9:
        __OcsSetLampTestState(atoi(argv[3]));
        break;

    case 10:
        __OcsUpgrade(argv[3]);
        break;

    default:
        std::cout << "Unknown Command." << std::endl;
    }

    return;

}

int32_t
main(int32_t argc, char *argv[])
{
    std::vector<std::string> staCmdVec = {"oa", "otdr", "board", "osc", "slot", "fan", "ocm", "logic", "ops", "psu", "ocs"};
    uint32_t uiCmdIdx = __CmdIdx(argv[1], staCmdVec);


    switch (uiCmdIdx) {
    case 0:
        __OaTest(argv);
        break;

    case 1:
        __OtdrTest(argv);
        break;

    case 2:
        __BoardTest(argv);
        break;

    case 3:
        __OscTest(argv);
        break;

    case 4:
        __SlotTest(argv);
        break;

    case 5:
        __FanTest();
        break;

    case 6:
        __OcmTest(argv);
        break;

    case 7:
        __LogicTest(atoi(argv[2]), argv);
        break;

    case 8:
        __OpsTest(argv);
        break;

    case 9:
        __PsuTest();
        break;

    case 10:
        __OcsTest(argv);
        break;

    default:
        std::cout << "Unknown Command! uiCmdIdx = " << uiCmdIdx << std::endl;
        return -1;
    }

    return 0;

#if 0
    __OaAliTest();
#endif


#if 0
    __OaSetTest();
#endif


#if 0
    __OaUpgrade(PA_IDX, "/tmp/edfa_pa_ba_sw_0.01.0002.bin");
#endif


#if 0
    __GetChassisMfg();
#endif

#if 0
    __FanTest();
#endif

#if 0
    __PsuTest();
#endif

#if 0
    __LedTest();
#endif

#if 0
    __BoardTest();
#endif


#if 0
    __OcmTest();
#endif


#if 0
    __ExtendTest();
#endif


#if 0
    __ExtendSetVoa(200, 0);
#endif


#if 0
    __ExtendStatusData();
#endif


#if 0
    __ExtendUpgrade();
#endif


    /* __UpgradeResetTest(); */

    /* } */
#if 0
    __MuxTest();
#endif

#if 0
    __EEpromWrite();
#endif

#if 0
    __WssTest();
#endif
    /* __EEpromRead(); */

#if 0
    __SlotTest();
#endif

#if 0
    __OpsTest();
#endif

#if 0
    __LogicReset(1);
#endif


#if 0
    __LogicUpgradeReset(1);
#endif

#if 0
    CDevApiManager::GetInstance().GetBoardApi().LogicReset(0, 1);
#endif

#if 0
    __ColdResetTest();
#endif

#if 0

    __SlotSetOnline();


    __LogicWrite(2, 20, 25);

    __LogicWrite(2, 21, 50);

    __LogicWrite(2, 22, 75);

    __LogicWrite(2, 23, 99);

#endif


    return 0;

}


#else

int32_t
main(int32_t argc, char *argv[])
{
#if 1
    __OaAliTest();
#endif
}


#endif
