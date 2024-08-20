
/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      OaMasterProto.cpp
   Author:
   Date:
   Version:
   Description:
   Function List:

   History:


*****************************************************************************************************/

/*=========================include head files====================*/

#include "OaMasterSpiProto.h"
#include "StringTool.h"
#include "OaComm.h"
#include "OaDriver.h"
/*==============================function=========================*/

bool
COaSwMasterSpiProto::ProcRsp(std::string & rstaMsg)
{

    bool bRet = false;
    bool bInProgress = false;

    bRet = CFrameProto::UnPack(rstaMsg, bInProgress);
    LOG_WRITE(EN_LOG_DEBUG, "bRet = %d, bInProgress = %d", bRet, bInProgress);

    return bInProgress;
}

bool
COaSwMasterSpiProto::ProcRsp(std::string & rstaMsg, bool &rbInProgress)
{

    bool bRet = false;
    bool bInProgress = false;

    bRet = CFrameProto::UnPack(rstaMsg, bInProgress, MODULE_TPYE_OA);
    rbInProgress = bInProgress;
    LOG_WRITE(EN_LOG_DEBUG, "bRet = %d, bInProgress = %d", bRet, bInProgress);

    return bRet;
}

void
COaSwMasterSpiProto::GetMfgCmd(std::string &rstaMsg)
{

    /*rstaMsg = "privinfo\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_GET_MODULE_INFO;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);
}

bool
COaSwMasterSpiProto::GetMfgRsp(const std::string &c_rstaMsg, CMfg &rstMfg)
{

    ST_MODULE_INFO *pstModuleInfo = nullptr;;
    uint8_t *pucMsg = nullptr;

    /* LOG_WRITE(EN_LOG_NOTICE, "UnPack +++"); */

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    pstModuleInfo = (ST_MODULE_INFO *)pucMsg;

    sprintf(rstMfg.acManufacturer, "%s", "Molex");
    memcpy(rstMfg.acModuleType, pstModuleInfo->acModuleName, sizeof(rstMfg.acModuleType) - 1);
    memcpy(rstMfg.acSwVer, pstModuleInfo->aucFwMainVer, sizeof(rstMfg.acSwVer) - 1);
    memcpy(rstMfg.acFwVer, pstModuleInfo->aucBundleVer, sizeof(rstMfg.acFwVer) - 1);
    memcpy(rstMfg.acBootVer, pstModuleInfo->aucFwBootVer, sizeof(rstMfg.acBootVer) - 1);
    memcpy(rstMfg.acHwVer, pstModuleInfo->aucHwVer, sizeof(rstMfg.acHwVer) - 1);
    memcpy(rstMfg.acSN, pstModuleInfo->aucSerialNum, sizeof(rstMfg.acSN) - 1);
    sprintf(rstMfg.acProductName, "%s", "EDFA");
    memcpy(rstMfg.acFpgaRunVer, pstModuleInfo->aucFpgaVer, sizeof(rstMfg.acFpgaRunVer) - 1);
    memcpy(rstMfg.acPN, pstModuleInfo->aucPartNum, sizeof(rstMfg.acPN) - 1);
    memcpy(rstMfg.acCpld1Ver, pstModuleInfo->aucRsv, sizeof(rstMfg.acCpld1Ver) - 1);
    memcpy(rstMfg.acMfgDate, pstModuleInfo->aucMfgDate, sizeof(rstMfg.acMfgDate) - 1);
    memcpy(rstMfg.acCaliDate, pstModuleInfo->aucCaliDate, sizeof(rstMfg.acCaliDate) - 1);
#ifdef __NR1004__
    strncpy(rstMfg.acUnitName, pstModuleInfo->stExtModInf.aucUnitName, sizeof(rstMfg.acUnitName) - 1);
    strncpy(rstMfg.acFCPN, pstModuleInfo->stExtModInf.aucFCPN, sizeof(rstMfg.acFCPN) - 1);
    strncpy(rstMfg.acFCIss, pstModuleInfo->stExtModInf.aucFCIss, sizeof(rstMfg.acFCIss) - 1);
    strncpy(rstMfg.acFujitsuPN, pstModuleInfo->stExtModInf.aucFujitsuPN, sizeof(rstMfg.acFujitsuPN) - 1);
    strncpy(rstMfg.acCleiCode, pstModuleInfo->stExtModInf.aucCleiCode, sizeof(rstMfg.acCleiCode) - 1);
    strncpy(rstMfg.acEth0Mac, pstModuleInfo->stExtModInf.aucMac, sizeof(rstMfg.acEth0Mac) - 1);
    strncpy(rstMfg.acFjtSn, pstModuleInfo->stExtModInf.aucFSN, sizeof(rstMfg.acFjtSn) - 1);
    LOG_WRITE(EN_LOG_NOTICE, "acUnitName %s", pstModuleInfo->stExtModInf.aucUnitName);
    LOG_WRITE(EN_LOG_NOTICE, "acFCPN %s", pstModuleInfo->stExtModInf.aucFCPN);
    LOG_WRITE(EN_LOG_NOTICE, "acFCIss %s", pstModuleInfo->stExtModInf.aucFCIss);
    LOG_WRITE(EN_LOG_NOTICE, "acFujitsuPN %s", pstModuleInfo->stExtModInf.aucFujitsuPN);
    LOG_WRITE(EN_LOG_NOTICE, "acCleiCode %s", pstModuleInfo->stExtModInf.aucCleiCode);
    LOG_WRITE(EN_LOG_NOTICE, "acMac %s", pstModuleInfo->stExtModInf.aucMac);
    LOG_WRITE(EN_LOG_NOTICE, "acFjtSn %s", pstModuleInfo->stExtModInf.aucFSN);
#endif

    return true;

}


void
COaSwMasterSpiProto::GetStatusData(std::string &rstaMsg)
{
    /*rstaMsg = "allinfo\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_GET_EDFA_WBA_DATA;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);

}
#ifdef __OCS__
bool
COaSwMasterSpiProto::GetStatusDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    return true;
}
#else
#ifdef __HCF__
bool
COaSwMasterSpiProto::GetStatusDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    uint8_t *pucMsg = nullptr;
    ST_EDFA_OPER *pstEdfaOperData = nullptr;
    COaStatusData stEdfaStatus;
    uint8_t acIndex[2][EN_EDFA_SUB_PUMP_MAX] = {{0, 1, 2, 4, 5, 6, 7, 3}, {0, 1, 2, 3, 4, 5, 6, 7}};
    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    pstEdfaOperData = (ST_EDFA_OPER *)pucMsg;

    for (int32_t i = 0; i < 3; i++) {
        /* uint32_t uiSubIdx = i; */
        uint32_t k = 0;
        if (i == 0) {
            k = 0;
        } else if (i == 1) {
            k = 1;
        }
        /*****************************************************/
        /***  get status ***/
        /*****************************************************/
        memset(&stEdfaStatus, 0, sizeof(stEdfaStatus));
        /* AGC:1, APC:2 */
        stEdfaStatus.stOaCurData.ucModeActual = pstEdfaOperData->astEdfaCfg[k].uiCtrlMode;

        /* power */
        if (0 == i) {               /* pa */
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[BA_IN_PWR];           /* In PD */
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[BA_OUT_SIG];      /* Out Sig Power */
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[BA_OUT_PWR];     /* Out Total Power TBD */
            stEdfaStatus.stOaCurData.sPortOutSigCPower = pstEdfaOperData->aiPDPwr[BA_LINE_OUT_SIG];  /* Port Sig Power TBD */
            stEdfaStatus.stOaCurData.sPortOutTotalPower = pstEdfaOperData->aiPDPwr[BA_LINE_OUT_PWR]; /* Port Total Power TBD */
            stEdfaStatus.stOaCurData.sBackRefPower = pstEdfaOperData->aiPDPwr[BA_LINE_RFL_PWR];      /* Back Ref Power */
            stEdfaStatus.stOaCurData.sBackRefRatio = pstEdfaOperData->aiPDPwr[BA_LINE_RFL_RATIO];      /* TBD */
            stEdfaStatus.stOaCurData.sLaserSafetyRxPower = pstEdfaOperData->aiPDPwr[SAFTY_RX_PWR];      /* TBD */
            stEdfaStatus.stOaCurData.sLaserSafetyTxPower = pstEdfaOperData->aiPDPwr[SAFTY_TX_PWR];      /* TBD */
        } else if (1 == i) {       /* ba */
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[PA_IN_PWR];            /* In PD */
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[PA_OUT_SIG];       /* Out Sig Power TBD */
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[PA_OUT_PWR];      /* Out Total Power TBD */
            stEdfaStatus.stOaCurData.sPortOutSigCPower = pstEdfaOperData->aiPDPwr[PA_LINE_OUT_SIG];   /* Port Sig Power TBD */
            stEdfaStatus.stOaCurData.sPortOutTotalPower = pstEdfaOperData->aiPDPwr[PA_LINE_OUT_PWR];  /* Port Total Power TBD */
            stEdfaStatus.stOaCurData.sBackRefPower = pstEdfaOperData->aiPDPwr[PA_LINE_RFL_PWR];       /* Back Ref Power */
            stEdfaStatus.stOaCurData.sBackRefRatio = pstEdfaOperData->aiPDPwr[PA_LINE_RFL_RATIO];       /* TBD */
            stEdfaStatus.stOaCurData.sLaserSafetyRxPower = pstEdfaOperData->aiPDPwr[SAFTY_RX_PWR];      /* TBD */
            stEdfaStatus.stOaCurData.sLaserSafetyTxPower = pstEdfaOperData->aiPDPwr[SAFTY_TX_PWR];      /* TBD */
        } else if (2 == i) {
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[OSC_TX_PWR];
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[OSC_ADD_PWR];
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[OSC_ADD_PWR];
        }
        /* Lband Power */
        stEdfaStatus.stOaCurData.sOutSigLPower = NO_POWER_6000;
        stEdfaStatus.stOaCurData.sPortOutSigLPower = NO_POWER_6000;

        /* Msa Power */
        stEdfaStatus.stOaCurData.sMsaInPower = NO_POWER_6000;
        stEdfaStatus.stOaCurData.sMsaOutPower = NO_POWER_6000;

        /* Gain & Tilt */
        stEdfaStatus.stOaCurData.sGainActual = pstEdfaOperData->astEdfaStatus[k].iActualGain;

        stEdfaStatus.stOaCurData.sTiltActual = pstEdfaOperData->astEdfaCfg[k].iGainTilt;

        /* Moduel & Heater Temperature */
        stEdfaStatus.stOaCurData.asCaseTemp[0] = pstEdfaOperData->astEdfaStatus[k].iCaseTemp;
        stEdfaStatus.stOaCurData.asHeaterTemp[0] = pstEdfaOperData->astEdfaStatus[k].iFiberTemp;

        /* Pump Temperature */
        for (uint32_t m = 0; m < MIN(sizeof(stEdfaStatus.stOaCurData.asPumpTemp) / sizeof(stEdfaStatus.stOaCurData.asPumpTemp[0]), EN_EDFA_SUB_PUMP_MAX); m++) {
            stEdfaStatus.stOaCurData.asPumpTemp[m] = pstEdfaOperData->astEdfaStatus[k].aiLaserTemp[acIndex[i][m]];
        }

        /* Pump Current */
        for (uint32_t m = 0; m < MIN(sizeof(stEdfaStatus.stOaCurData.asPumpCurrent) / sizeof(stEdfaStatus.stOaCurData.asPumpCurrent[0]), EN_EDFA_SUB_PUMP_MAX); m++) {
            stEdfaStatus.stOaCurData.asPumpCurrent[m] = pstEdfaOperData->astEdfaStatus[k].auiLaserCurr[acIndex[i][m]];
        }

        /* Voa Attn */
        for (uint32_t m = 0; m < sizeof(stEdfaStatus.stOaCurData.asVoaAttnActual) / sizeof(int16_t); m++) {
            stEdfaStatus.stOaCurData.asVoaAttnActual[m] = pstEdfaOperData->astVoaStatus[m].iAtten;
        }

        /* ExtPd */
        for (uint32_t m = 0; m < sizeof(stEdfaStatus.stOaCurData.asExtPdPower) / sizeof(int16_t); m++) {
            stEdfaStatus.stOaCurData.asExtPdPower[m] = (pstEdfaOperData->aiPDPwr[m]);
        }

        /* Osc Dfb */
        stEdfaStatus.stOscCurData.sAddPower = pstEdfaOperData->uiOscAddPower;
        stEdfaStatus.stOscCurData.sDropPower = pstEdfaOperData->uiOscDropPower;
        stEdfaStatus.stOscCurData.ucModStatus = pstEdfaOperData->uiOscOnOff;
        stEdfaStatus.stOaCurData.abDfbEnable = pstEdfaOperData->uiDFBOnOff;
        stEdfaStatus.stOaCurData.abApsdEnable = pstEdfaOperData->uiApsdOnOff;
        /* stEdfaStatus.stOaCurData.abApsdStatus = pstEdfaOperData->uiApsdStatus; */
        stEdfaStatus.stOaCurData.uiOtdrSwitchInEx = pstEdfaOperData->uiOtdrSwitchInEx;
        stEdfaStatus.stOaCurData.uiOtdrSwitch = pstEdfaOperData->uiOtdrSwitch + 1;

        /* Annex */
        stEdfaStatus.stOaCurData.sDfbInPower2 = pstEdfaOperData->aiPDPwr[PA_DFB_RFL_PWR];
        stEdfaStatus.stOaCurData.sDfbOutPower2 = pstEdfaOperData->aiPDPwr[PA_DFB_OUT_PWR];
        stEdfaStatus.stOaCurData.sDfbInPower1 = pstEdfaOperData->aiPDPwr[BA_DFB_RFL_PWR];
        stEdfaStatus.stOaCurData.sDfbOutPower1 = pstEdfaOperData->aiPDPwr[BA_DFB_OUT_PWR];

        for (uint32_t m = 0; m < sizeof(stEdfaStatus.stOaAnnexData.aucLedSta); m++) {
            stEdfaStatus.stOaAnnexData.aucLedSta[m] = (pstEdfaOperData->aucLedSta[m]);
        }

        /* stEdfaStatus.stOaAnnexData.uiOtdrSwitchInEx = pstEdfaOperData->uiOtdrSwitchInEx; */
        /* stEdfaStatus.stOaAnnexData.uiOtdrSwitch = pstEdfaOperData->uiOtdrSwitch; */
        stEdfaStatus.stOaAnnexData.uiSccUpgSta = pstEdfaOperData->uiSccUpgSta;
        stEdfaStatus.stOaAnnexData.uiTestLedSta = pstEdfaOperData->uiTestLedSta;
        stEdfaStatus.stOaAnnexData.uiLedAlmSta = pstEdfaOperData->uiLedAlmSta;

        LOG_WRITE(EN_LOG_DEBUG, "uiOtdrSwitch= %d", pstEdfaOperData->uiOtdrSwitch);
        LOG_WRITE(EN_LOG_DEBUG, "uiSccUpgSta= %d", pstEdfaOperData->uiSccUpgSta);
        LOG_WRITE(EN_LOG_DEBUG, "uiTestLedSta= %d", pstEdfaOperData->uiTestLedSta);
        LOG_WRITE(EN_LOG_DEBUG, "uiLedAlmSta= %d", pstEdfaOperData->uiLedAlmSta);

        /* Threshold TBD*/
        /* memcpy((char *)&stEdfaStatus.stOaCurData.astThrHys, (char *)pstEdfaOperData->astThrHys, sizeof(CMN_THR_HYS_INFO_ST) * OA_THR_PORT_MAX * OA_THR_TYPE_MAX); */

        /* AlmMask TBD */
        memcpy((char *)&stEdfaStatus.stOaAnnexData.aucAlmMask, (char *)pstEdfaOperData->aucAlmMask, sizeof(stEdfaStatus.stOaAnnexData.aucAlmMask));
        LOG_WRITE(EN_LOG_DEBUG, "AlmMask[0]= %d", pstEdfaOperData->aucAlmMask[0]);
        LOG_WRITE(EN_LOG_DEBUG, "AlmMask[1]= %d", pstEdfaOperData->aucAlmMask[1]);

        /* laser safefy */
        memcpy((char *)&stEdfaStatus.stOaCurData.stLaserSafety, (char *)&pstEdfaOperData->stLaserSafety, sizeof(stEdfaStatus.stOaCurData.stLaserSafety));

        stEdfaStatus.stOaAnnexData.ucShutterSta = pstEdfaOperData->stWbaStatus.uiStatus;
        stEdfaStatus.stOaCurData.uiDFBPwr = pstEdfaOperData->uiDFBPwr;

        /* add for Beta autogain */
        stEdfaStatus.stOaCurData.uiAutogainSt = pstEdfaOperData->auiAutogainSt[k];
        stEdfaStatus.stOaCurData.uiAutogainMode = pstEdfaOperData->auiAutogainMode[k];

        stEdfaStatus.stOaCurData.usRemoteLockEn = pstEdfaOperData->usRemoteLockEn;
        stEdfaStatus.stOaCurData.usAdminKey = pstEdfaOperData->usAdminKey;
        stEdfaStatus.stOaCurData.uiPanelSafe = pstEdfaOperData->uiPanelSafe;
        stEdfaStatus.stOaCurData.uiApcTargetPwr = pstEdfaOperData->auiApcTargetPwr[k];
        stEdfaStatus.stOaCurData.uiReadyPwr = pstEdfaOperData->auiReadyPwr[k];

        rstaData.push_back(stEdfaStatus);
    }


    return true;

}
#else
bool
COaSwMasterSpiProto::GetStatusDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    uint8_t *pucMsg = nullptr;
    /*uint32_t uiLen = 0; */
    /*int16_t sCheckSum = 0; */
    /*int16_t *psStatusData = NULL; */
    /*EDFA_DATA_ST stEdfaData; */
    ST_EDFA_OPER *pstEdfaOperData = nullptr;
    COaStatusData stEdfaStatus;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    pstEdfaOperData = (ST_EDFA_OPER *)pucMsg;

    for (int32_t i = 0; i < 2; i++) {
        /* uint32_t uiSubIdx = i; */
        uint32_t k = 0;
        if (i == 0) {
            k = 0;
        } else if (i == 1) {
            k = 1;
        }
        /*****************************************************/
        /***  get status ***/
        /*****************************************************/
        memset(&stEdfaStatus, 0, sizeof(stEdfaStatus));
        /**********************************
           BIT0- 1��OK	  0����״̬
           BIT1- 1: DIS  0����DIS
           BIT2- 1: LIM  0����LIM
           BIT3- 1: ES	 0����ES
           BIT4- 1: EOP  0����EOP
           BIT5- 1: NOT READY	0: OK
        **********************************/
        /*stEdfaStatus.stOaCurData.bIsInService = (stEdfaData.stEDFA.asStatus[uiSubIdx] & 0x02) ? false : true; */
        /*stEdfaStatus.stOaCurData.bIsInService = pstEdfaOperData-> */

        /* AGC:1, APC:2 */
        /*stEdfaStatus.stOaCurData.ucModeActual = stEdfaData.stEDFA.asModeSet[uiSubIdx]; */
        stEdfaStatus.stOaCurData.ucModeActual = pstEdfaOperData->astEdfaCfg[k].uiCtrlMode;
        /* In PD */
        /*stEdfaStatus.stOaCurData.sInPower = stEdfaData.stEDFA.asPD[uiSubIdx * 8 + PD_IN]; */
        if (0 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[E1_PDIN];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[PD5_EDFA1_IN_PWR];                                                       /* to be verify*/
#endif
        } else if (1 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[E2_PDIN];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sInPower = pstEdfaOperData->aiPDPwr[PD1_EDFA2_IN_PWR];
#endif
        }


        /* Out Sig Power */
        /*stEdfaStatus.stOaCurData.sOutSigCPower = stEdfaData.stEDFA.asOsp[uiSubIdx]; */
        if (0 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[E1_PM5];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[PD6_EDFA1_OUT_SIG];                                                       /* to be verify*/
#endif
        } else if (1 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[E2_PM5];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sOutSigCPower = pstEdfaOperData->aiPDPwr[PD2_EDFA2_OUT_SIG];
#endif
        }
        stEdfaStatus.stOaCurData.sOutSigLPower = NO_POWER_6000;

        /* Out PD */
        /*stEdfaStatus.stOaCurData.sOutTotalPower = stEdfaData.stEDFA.asPD[uiSubIdx * 8 + PD_OUT]; */
        if (0 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[E1_PDOUT];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[PD6_EDFA1_OUT_PWR];
#endif
        } else {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[E2_PDOUT];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sOutTotalPower = pstEdfaOperData->aiPDPwr[PD2_EDFA2_OUT_PWR];
#endif
        }

        /* Port Power */
        /* int16_t sPower = stEdfaData.stEDFA.asOsp[uiSubIdx] - stEdfaData.stEDFA.asVOAAct[1]; */
        /* int16_t sPower = pstEdfaOperData->astEdfaCfg[k].iOutPwr; */
        /* stEdfaStatus.stOaCurData.sPortOutSigCPower = ((sPower <= NO_POWER_6000) ? NO_POWER_6000 : sPower); */
        if (0 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sPortOutSigCPower = pstEdfaOperData->aiPDPwr[E1_PM5];
            stEdfaStatus.stOaCurData.sPortOutTotalPower = pstEdfaOperData->aiPDPwr[E1_PDOUT];
#else

#endif
        } else {
#ifdef __NR1004__
            if ( pstEdfaOperData->aiPDPwr[E2_VOA2_OUT] <= NO_POWER_6000 ) {
                stEdfaStatus.stOaCurData.sPortOutSigCPower = NO_POWER_6000;
            } else {
                int16_t sPower = pstEdfaOperData->aiPDPwr[E2_PM5] - (pstEdfaOperData->aiPDPwr[E2_VOA2_IN] - pstEdfaOperData->aiPDPwr[E2_VOA2_OUT]);
                stEdfaStatus.stOaCurData.sPortOutSigCPower = ((sPower <= NO_POWER_6000) ? NO_POWER_6000 : sPower);
            }
            stEdfaStatus.stOaCurData.sPortOutTotalPower = pstEdfaOperData->aiPDPwr[E2_VOA2_OUT];
#else

#endif
        }

#if 0
        if ((stEdfaData.stEDFA.asOsp[0] - stEdfaData.stEDFA.asVOAAct[1]) <= NO_POWER_6000) {
            stEdfaStatus.stOaCurData.sPortOutSigCPower = NO_POWER_6000;
        } else {
            stEdfaStatus.stOaCurData.sPortOutSigCPower = stEdfaData.stEDFA.asOsp[0] - stEdfaData.stEDFA.asVOAAct[1];
        }
#endif

        stEdfaStatus.stOaCurData.sPortOutSigLPower = NO_POWER_6000;
        /*stEdfaStatus.stOaCurData.sPortOutTotalPower = stEdfaData.stEDFA.asPD[uiSubIdx * 8 + PD_VOA2_OUT];  //?? */

        /* Msa Power */
        stEdfaStatus.stOaCurData.sMsaInPower = NO_POWER_6000;
        stEdfaStatus.stOaCurData.sMsaOutPower = NO_POWER_6000;

        /* Back Ref Power */
        /*stEdfaStatus.stOaCurData.sBackRefPower = stEdfaData.stEDFA.asPD[PD_REF]; */
        if (0 == i) {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sBackRefPower = pstEdfaOperData->aiPDPwr[E1_RFL];
            stEdfaStatus.stOaCurData.sBackRefRatio = pstEdfaOperData->aiPDPwr[E1_PM4];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sBackRefPower = pstEdfaOperData->aiPDPwr[PD8_EDFA1_LINE_TX_RFL_PWR];
#endif
        } else {
#ifdef __NR1004__
            stEdfaStatus.stOaCurData.sBackRefPower = pstEdfaOperData->aiPDPwr[E2_RFL];
            stEdfaStatus.stOaCurData.sBackRefRatio = pstEdfaOperData->aiPDPwr[E2_PM4];
#endif
#ifdef __OLSP__
            stEdfaStatus.stOaCurData.sBackRefPower = pstEdfaOperData->aiPDPwr[PD4_EDFA2_LINE_TX_RFL_PWR];
#endif
        }
        /* Gain & Tilt */
        /*stEdfaStatus.stOaCurData.sGainActual = stEdfaData.stEDFA.asGain[uiSubIdx]; */
        stEdfaStatus.stOaCurData.sGainActual = pstEdfaOperData->astEdfaStatus[k].iActualGain;
        /* if (NO_POWER_6000 == stEdfaStatus.stOaCurData.sInPower) {
            stEdfaStatus.stOaCurData.sGainActual = NO_POWER_6000;
           } */

        /*stEdfaStatus.stOaCurData.sTiltActual = stEdfaData.stEDFA.asTiltAct[uiSubIdx]; */
        stEdfaStatus.stOaCurData.sTiltActual = pstEdfaOperData->astEdfaCfg[k].iGainTilt;

        /* Moduel & Heater Temperature */
        /*stEdfaStatus.stOaCurData.asCaseTemp[0] = stEdfaData.stEDFA.sCtValue; */
        /*stEdfaStatus.stOaCurData.asHeaterTemp[0] = stEdfaData.stEDFA.sHtValue; */
        stEdfaStatus.stOaCurData.asCaseTemp[0] = pstEdfaOperData->astEdfaStatus[k].iCaseTemp;
        stEdfaStatus.stOaCurData.asHeaterTemp[0] = pstEdfaOperData->astEdfaStatus[k].iFiberTemp;

        /* Pump Temperature */
        /*stEdfaStatus.stOaCurData.asPumpTemp[0] = stEdfaData.stEDFA.asPtValue[uiSubIdx]; */
        stEdfaStatus.stOaCurData.asPumpTemp[0] = pstEdfaOperData->astEdfaStatus[k].aiLaserTemp[0];
        stEdfaStatus.stOaCurData.asPumpTemp[1] = pstEdfaOperData->astEdfaStatus[k].aiLaserTemp[1];
        stEdfaStatus.stOaCurData.asPumpTemp[2] = pstEdfaOperData->astEdfaStatus[k].aiLaserTemp[2];
        /* stEdfaStatus.stOaCurData.asPumpTemp[1] = stEdfaData.stEDFA.asPtValue[1]; */

        /* Pump Current */
        /*stEdfaStatus.stOaCurData.asPumpCurrent[0] = stEdfaData.stEDFA.asPcValue[uiSubIdx]; */
        stEdfaStatus.stOaCurData.asPumpCurrent[0] = pstEdfaOperData->astEdfaStatus[k].auiLaserCurr[0];
        stEdfaStatus.stOaCurData.asPumpCurrent[1] = pstEdfaOperData->astEdfaStatus[k].auiLaserCurr[1];
        stEdfaStatus.stOaCurData.asPumpCurrent[2] = pstEdfaOperData->astEdfaStatus[k].auiLaserCurr[2];
        /* stEdfaStatus.stOaCurData.asPumpCurrent[1] = stEdfaData.stEDFA.asPcValue[1]; */

#ifdef __400GZR__
        /*stEdfaStatus.stOaCurData.asVoaAttnActual[0] = stEdfaData.stEDFA.asVOAAct[0]; */
        /*stEdfaStatus.stOaCurData.asVoaAttnActual[1] = stEdfaData.stEDFA.asVOAAct[1]; */
        stEdfaStatus.stOaCurData.asVoaAttnActual[0] = pstEdfaOperData->astVoaStatus[0].iAtten;                   /*? */
        stEdfaStatus.stOaCurData.asVoaAttnActual[1] = pstEdfaOperData->astVoaStatus[1].iAtten;                   /*? */
#else
        /* Voa Attn */
        /*stEdfaStatus.stOaCurData.asVoaAttnActual[0] = stEdfaData.stEDFA.asVOAAct[uiSubIdx]; */
        stEdfaStatus.stOaCurData.asVoaAttnActual[0] = pstEdfaOperData->astVoaStatus[0].iAtten;                   /*? */
        stEdfaStatus.stOaCurData.asVoaAttnActual[1] = pstEdfaOperData->astVoaStatus[1].iAtten;                   /*? */
        stEdfaStatus.stOaCurData.asVoaAttnActual[2] = pstEdfaOperData->astVoaStatus[2].iAtten;
        stEdfaStatus.stOaCurData.asVoaAttnActual[3] = pstEdfaOperData->astVoaStatus[3].iAtten;
        stEdfaStatus.stOaCurData.asVoaAttnActual[4] = pstEdfaOperData->astVoaStatus[4].iAtten;
        stEdfaStatus.stOaCurData.asVoaAttnActual[5] = pstEdfaOperData->astVoaStatus[5].iAtten;
        stEdfaStatus.stOaCurData.asVoaAttnActual[6] = pstEdfaOperData->astVoaStatus[6].iAtten;
        stEdfaStatus.stOaCurData.asVoaAttnActual[7] = pstEdfaOperData->astVoaStatus[7].iAtten;

#endif

        for (uint32_t m = 0; m < sizeof(stEdfaStatus.stOaCurData.asExtPdPower) / sizeof(int16_t); m++) {
            stEdfaStatus.stOaCurData.asExtPdPower[m] = (pstEdfaOperData->aiPDPwr[m]);
        }
        stEdfaStatus.stOscCurData.sAddPower = pstEdfaOperData->uiOscAddPower;
        stEdfaStatus.stOscCurData.sDropPower = pstEdfaOperData->uiOscDropPower;
        stEdfaStatus.stOscCurData.ucModStatus = pstEdfaOperData->uiOscOnOff;
        stEdfaStatus.stOaCurData.abDfbEnable = pstEdfaOperData->uiDFBOnOff;
        stEdfaStatus.stOaCurData.abApsdEnable = pstEdfaOperData->uiApsdOnOff;
        /* stEdfaStatus.stOaCurData.abApsdStatus = pstEdfaOperData->uiApsdStatus; */
        stEdfaStatus.stOaCurData.uiOtdrSwitch = pstEdfaOperData->uiOtdrSwitch + 1;
#ifdef __OLSP__
        stEdfaStatus.stOaCurData.sDfbInPower2 = pstEdfaOperData->aiPDPwr[PD9_EDFA2_DFB_IN_PWR];
        stEdfaStatus.stOaCurData.sDfbOutPower2 = pstEdfaOperData->aiPDPwr[PD10_EDFA2_DFB_OUT_PWR];
        stEdfaStatus.stOaCurData.sDfbInPower1 = pstEdfaOperData->aiPDPwr[PD11_EDFA1_DFB_IN_PWR];
        stEdfaStatus.stOaCurData.sDfbOutPower1 = pstEdfaOperData->aiPDPwr[PD12_EDFA1_DFB_OUT_PWR];
#endif
        for (uint32_t m = 0; m < sizeof(stEdfaStatus.stOaAnnexData.aucLedSta); m++) {
            stEdfaStatus.stOaAnnexData.aucLedSta[m] = (pstEdfaOperData->aucLedSta[m]);
        }
        /* stEdfaStatus.stOaAnnexData.uiOtdrSwitch = pstEdfaOperData->uiOtdrSwitch; */
        stEdfaStatus.stOaAnnexData.uiSccUpgSta = pstEdfaOperData->uiSccUpgSta;
        stEdfaStatus.stOaAnnexData.uiTestLedSta = pstEdfaOperData->uiTestLedSta;
        stEdfaStatus.stOaAnnexData.uiLedAlmSta = pstEdfaOperData->uiLedAlmSta;

        LOG_WRITE(EN_LOG_DEBUG, "uiOtdrSwitch= %d", pstEdfaOperData->uiOtdrSwitch);
        LOG_WRITE(EN_LOG_DEBUG, "uiSccUpgSta= %d", pstEdfaOperData->uiSccUpgSta);
        LOG_WRITE(EN_LOG_DEBUG, "uiTestLedSta= %d", pstEdfaOperData->uiTestLedSta);
        LOG_WRITE(EN_LOG_DEBUG, "uiLedAlmSta= %d", pstEdfaOperData->uiLedAlmSta);

        memcpy((char *)&stEdfaStatus.stOaCurData.astThrHys, (char *)pstEdfaOperData->astThrHys, sizeof(CMN_THR_HYS_INFO_ST) * OA_THR_PORT_MAX * OA_THR_TYPE_MAX);
        memcpy((char *)&stEdfaStatus.stOaAnnexData.aucAlmMask, (char *)pstEdfaOperData->aucAlmMask, sizeof(stEdfaStatus.stOaAnnexData.aucAlmMask));
        LOG_WRITE(EN_LOG_DEBUG, "AlmMask[0]= %d", pstEdfaOperData->aucAlmMask[0]);
        LOG_WRITE(EN_LOG_DEBUG, "AlmMask[1]= %d", pstEdfaOperData->aucAlmMask[1]);
#if 0
        for (uint32_t p = 0; p < OA_THR_PORT_MAX; p++) {
            for (uint32_t q = 0; q < OA_THR_TYPE_MAX; q++) {
                /* stEdfaStatus.stOaCurData.astThrHys[p][q].iThr = pstEdfaOperData->astThrHys[p][q].iThr; */
                /* stEdfaStatus.stOaCurData.astThrHys[p][q].iHys = pstEdfaOperData->astThrHys[p][q].iHys; */
                LOG_WRITE(EN_LOG_NOTICE, "ThrHys[%d][%d].iThr= %d, iHys= %d", p, q, stEdfaStatus.stOaCurData.astThrHys[p][q].iThr, stEdfaStatus.stOaCurData.astThrHys[p][q].iHys);
            }
        }
#endif
#if 0
        /*****************************************************/
        /***  debug alarms ***/
        /*****************************************************/
        if (m_bModuleAlarmDebug) {
            /* OOG debug --- alarm mdoule unsupport */
            SET_BIT_2_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);

            /* EOL debug --- alarm module unsupport */
            SET_BIT_4_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);

            /* Case Temp debug --- alarm module unsupport */
            SET_BIT_8_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);

            /* Disable debug */
            stEdfaStatus.stOaCurData.bIsInService = false;

            /* Pump Temp debug */
            SET_BIT_0_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        }
#endif

#if 0
        /* compare alarm */
        if ((0 != memcmp(m_stOaData.stEDFA.asAlarm1, stEdfaData.stEDFA.asAlarm1, sizeof(m_stOaData.stEDFA.asAlarm1))) ||
            (0 != memcmp(m_stOaData.stEDFA.asAlarm2, stEdfaData.stEDFA.asAlarm2, sizeof(m_stOaData.stEDFA.asAlarm2))) ||
            (0 != memcmp(m_stOaData.stEDFA.asEXTAlarm, stEdfaData.stEDFA.asEXTAlarm, sizeof(m_stOaData.stEDFA.asEXTAlarm))) ||
            (0 != memcmp(m_stOaData.stEDFA.asStatus, stEdfaData.stEDFA.asStatus, sizeof(m_stOaData.stEDFA.asStatus)))) {
            CEvent::SendEvent((char *)DEF_CSMD_EVENT_SOCKET_PATH, DEF_EVENT_ID_PA, 0);
            LOG_WRITE(EN_LOG_WARNING, "OA alarm hal SendEvent");
        }
#endif
/*
        stEdfaStatus.stOaAlm.bFailure = false;
        //stEdfaStatus.stOaAlm.bDisable = !stEdfaStatus.stOaCurData.bIsInService;   // ?
        stEdfaStatus.stOaAlm.bInnerErr = false;

        stEdfaStatus.stOaAlm.bInLos = BIT_0_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
        stEdfaStatus.stOaAlm.bOutLos = false;
        stEdfaStatus.stOaAlm.bMsaInLos = BIT_6_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
        stEdfaStatus.stOaAlm.bMsaOutLos = BIT_5_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);

        stEdfaStatus.stOaAlm.bRFL = BIT_4_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);
        stEdfaStatus.stOaAlm.bOOG = BIT_2_VALUE(stEdfaData.stEDFA.asAlarm1[uiSubIdx]);


        stEdfaStatus.stOaAlm.bOOP = false;
        stEdfaStatus.stOaAlm.bPumpEOL = BIT_4_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]) | BIT_5_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        stEdfaStatus.stOaAlm.bPumpTemp = BIT_0_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]) | BIT_1_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
        stEdfaStatus.stOaAlm.bCaseTemp = BIT_8_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]) | BIT_9_VALUE(stEdfaData.stEDFA.asAlarm2[uiSubIdx]);
 */

        /*memcpy((uint8_t *)&m_stOaData, (uint8_t *)&(stEdfaData), sizeof(m_stOaData)); */

        rstaData.push_back(stEdfaStatus);
    }


    return true;

}
#endif
#endif

void
COaSwMasterSpiProto::GetDebugInfoCmd(std::string &rstaMsg)
{
    /*rstaMsg = "DBGINFO\r\n"; */
    /*LOG_WRITE(EN_LOG_NOTICE, "rstaMsg = %s", rstaMsg.c_str()); */
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_DBG_EDFA_FPGA;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::GetMcuDebugInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx)
{
    std::string staBody;

    uint16_t ulWLen = 4;
    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = DEF_OA_SPI_DID_GET_MCU_DBG_INFO;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&ulDbgIdx, sizeof(ulDbgIdx));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::GetFpgaInfoCmd(std::string &rstaMsg, uint32_t ulDbgIdx)
{
    std::string staBody;

    uint16_t ulWLen = 4;
    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = DEF_OA_SPI_DID_GET_FPGA_INFO;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&ulDbgIdx, sizeof(ulDbgIdx));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetGainRange(uint32_t uiId, std::string &rstaMsg, uint32_t uiGainRange)
{

    /*rstaMsg = "sgtype " + std::to_string(uiId + 1) + " " + std::to_string(uiGainRange) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_GAIN_RANGE_MODE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiGainRange = htonl(uiGainRange); */
    staBody.append((char *)&uiGainRange, sizeof(uiGainRange));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetGain(uint32_t uiId, std::string &rstaMsg, uint32_t uiGain)
{

    /*float fData = (float)uiGain / 100; */

    /*rstaMsg = "mode g " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_GAIN;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiGain = htonl(uiGain); */
    staBody.append((char *)&uiGain, sizeof(uiGain));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetTilt(uint32_t uiId, std::string &rstaMsg, int16_t sTilt)
{
    /*float fData = (float)sTilt / 100; */

    /*rstaMsg = "tilt " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    int32_t iTilt = sTilt;
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_TILT;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* iTilt = htonl(iTilt); */
    staBody.append((char *)&iTilt, sizeof(iTilt));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetMode(uint32_t uiId, std::string &rstaMsg, uint8_t ucMode, int16_t sVal)
{

    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiMode = ucMode;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_CTRL_MODE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiMode = htonl(uiMode); */
    staBody.append((char *)&uiMode, sizeof(uiMode));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetRxLosThr(uint32_t uiId, std::string &rstaMsg, int16_t sThr)
{
#if 0
    int32_t lData = sThr / 100;

    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_RX_LOS_THR;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    lData = htonl(lData);
    staBody.append((char *)&lData, sizeof(lData));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
#endif
}

void
COaSwMasterSpiProto::SetRxLosHys(uint32_t uiId, std::string &rstaMsg, int16_t sHys)
{

    int32_t lData = sHys / 100;

    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_RX_LOS_HYS;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* lData = htonl(lData); */
    staBody.append((char *)&lData, sizeof(lData));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetRxLosThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    CMN_THR_HYS_ST stCmnThrHys;
    CMN_THR_HYS_ST stCmnThrHysBE;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_RX_LOS_THR;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    memset(&stCmnThrHys, 0, sizeof(stCmnThrHys));
    memset(&stCmnThrHysBE, 0, sizeof(stCmnThrHysBE));
    stCmnThrHys.stCmnThr.iFailLow = sThr;
    stCmnThrHys.stCmnHys.iFailLow = sHys;
    /* UTIL_htonl((uint32_t *)&stCmnThrHysBE, (uint32_t *)&stCmnThrHys, sizeof(stCmnThrHysBE) / sizeof(int32_t)); */
    staBody.append((char *)&stCmnThrHys, sizeof(stCmnThrHys));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetPumpEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_OSRI_CTRL_MODE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiEnable = htonl(uiEnable); */
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetAprEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = (bEnable ? "e" : "n");

    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_APR_ENABLE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetAutolosEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staEnable = bEnable ? "a" : "n";

    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_AUTOLOS_ENABLE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "rstaMsg = %s", rstaMsg.c_str());
}



void
COaSwMasterSpiProto::SetAttn(uint32_t uiId, std::string &rstaMsg, uint32_t uiAttn)
{
    /* float fData = (float)uiAttn / 100; */

    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    if ( VOA_IDX_6 == uiId) {                   /* fnc */
        stSpiCmdBody.usMid = htons(1);
        stSpiCmdBody.ucDid = OA_SPI_DID_OSC_TX_VOA;
    } else {
        stSpiCmdBody.usMid = htons(usId);
        stSpiCmdBody.ucDid = OA_SPI_DID_PADDING_VOA;
    }
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiAttn, sizeof(uiAttn));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "Send SetAttn[%d]: %d", uiId, uiAttn);
}

void
COaSwMasterSpiProto::SetTarget(uint32_t uiId, std::string &rstaMsg, uint32_t uiTarget)
{
    /* float fData = (float)uiTarget / 100; */

    std::string staBody;
    uint16_t ulWLen = 4;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);

    stSpiCmdBody.usMid = htons(1);
    stSpiCmdBody.ucDid = OA_SPI_DID_OSC_TX_VOA_POWER; /* fnc */

    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiTarget, sizeof(uiTarget));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_DEBUG, "Send SetTarget: %d", uiTarget);
}

void
COaSwMasterSpiProto::SoftReset(uint32_t uiId, std::string &rstaMsg)
{

    std::string staBody;
    uint16_t ulWLen = 0;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_FW_RESET;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::Upgrade(uint32_t uiId, std::string &rstaMsg)
{
    std::string staBody;
    uint16_t ulWLen = 0;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_FW_UPGD_START;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}


void
COaSwMasterSpiProto::Swap(uint32_t uiId, std::string &rstaMsg)
{

    std::string staBody;
    uint16_t ulWLen = 0;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_FW_UPGD_SWITCH;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::Write(uint32_t uiId, uint8_t *pucBuf, uint32_t ulLen, std::string &rstaMsg)
{
    std::string staBody;
    uint16_t ulWLen = ulLen;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_FW_UPGD_WRITE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)pucBuf, ulLen);
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::Verify(uint32_t uiId, std::string &rstaMsg)
{
    std::string staBody;
    uint16_t ulWLen = 0;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_FW_UPGD_VERIFY;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}


void
COaSwMasterSpiProto::Commit(uint32_t uiId, std::string &rstaMsg)
{
    std::string staBody;
    uint16_t ulWLen = 0;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_FW_UPGD_COMMIT;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::GetGain(uint32_t uiId, std::string &rstaMsg)
{
}

bool
COaSwMasterSpiProto::GetGainRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsGain)
{
    return true;
}

void
COaSwMasterSpiProto::GetTilt(uint32_t uiId, std::string &rstaMsg)
{
}

bool
COaSwMasterSpiProto::GetTiltRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsTilt)
{

    return true;
}

void
COaSwMasterSpiProto::GetRxLosThr(uint32_t uiId, std::string &rstaMsg)
{
}

bool
COaSwMasterSpiProto::GetRxLosThrRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsThr, int16_t &rsHys)
{

    return true;

}

void
COaSwMasterSpiProto::GetPumpEnable(uint32_t uiId, std::string &rstaMsg)
{
}

bool
COaSwMasterSpiProto::GetPumpEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable)
{

    return true;

}

void
COaSwMasterSpiProto::GetAutolosEnable(uint32_t uiId, std::string &rstaMsg)
{
}


bool
COaSwMasterSpiProto::GetAutolosEnableRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &rbEnable)
{
    return true;
}

void
COaSwMasterSpiProto::GetAttn(uint32_t uiId, std::string &rstaMsg)
{
}

bool
COaSwMasterSpiProto::GetAttnRsp(uint32_t uiId, const std::string &c_rstaMsg, int16_t &rsAttn)
{

    return true;
}

void
COaSwMasterSpiProto::GetAlarmData(std::string &rstaMsg)
{
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_GET_ALARM_STATUS;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);
}

bool
COaSwMasterSpiProto::GetAlarmDataRsp(const std::string &c_rstaMsg, std::vector<COaStatusData> &rstaData)
{
    uint8_t *pucMsg = nullptr;
    ST_EDFA_ALARM1 *pstEdfaAlm = nullptr;
    COaAlarm stOaAlm;
    int j = 0;

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    pstEdfaAlm = (ST_EDFA_ALARM1 *)pucMsg;

    for (uint32_t i = 0; i < rstaData.size(); i++) {


        memset(&stOaAlm, 0, sizeof(stOaAlm));

        rstaData.at(i).stOaAlm.bFailure = pstEdfaAlm->stModuleAlm.Fail[i];
        rstaData.at(i).stOaAlm.bDisable = pstEdfaAlm->stModuleAlm.Disable[i];
        rstaData.at(i).stOaAlm.bInnerErr = false;
        if (0 == i) { /* PA */
#ifdef __NR1004__
            rstaData.at(i).stOaAlm.bInLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[LINE_R_LOS];
            rstaData.at(i).stOaAlm.bInLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[LINE_R_POWER_IN_LOW];
            rstaData.at(i).stOaAlm.bInHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[LINE_R_POWER_IN_HI];
            rstaData.at(i).stOaAlm.bOutLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[MUX_T_LOS];
            rstaData.at(i).stOaAlm.bOutLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[MUX_T_POWER_OUT_LOW];
            rstaData.at(i).stOaAlm.bOutHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[MUX_T_POWER_OUT_HI];
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInLos: %d", i, rstaData.at(i).stOaAlm.bInLos);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInLow: %d", i, rstaData.at(i).stOaAlm.bInLow);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInHigh: %d", i, rstaData.at(i).stOaAlm.bInHigh);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bOutLos: %d", i, rstaData.at(i).stOaAlm.bOutLos);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bOutLow: %d", i, rstaData.at(i).stOaAlm.bOutLow);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bOutHigh: %d", i, rstaData.at(i).stOaAlm.bOutHigh);
#endif
#if defined __HCF__ || defined __OCS__
            rstaData.at(i).stOaAlm.bInLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[BA_IN_PWR];
            rstaData.at(i).stOscCurData.bLaserSafetyRxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[SAFTY_RX_PWR];
            rstaData.at(i).stOscCurData.bLaserSafetyTxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[SAFTY_TX_PWR];
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInLos: %d", i, rstaData.at(i).stOaAlm.bInLos);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOscCurData.bLaserSafetyRxLos: %d", i, rstaData.at(i).stOscCurData.bLaserSafetyRxLos);
#endif
#ifdef __OLSP__
            rstaData.at(i).stOaAlm.bInLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[PD5_EDFA1_IN_PWR];
#endif
        } else if (1 == i) { /* BA */
#ifdef __NR1004__
            rstaData.at(i).stOaAlm.bInLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[MUX_R_LOS];
            rstaData.at(i).stOaAlm.bInLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[MUX_R_POWER_IN_LOW];
            rstaData.at(i).stOaAlm.bInHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[MUX_R_POWER_IN_HI];

            rstaData.at(i).stOaAlm.bOutLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[LINE_T_LOS];
            rstaData.at(i).stOaAlm.bOutLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[LINE_T_POWER_OUT_LOW];
            rstaData.at(i).stOaAlm.bOutHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[LINE_T_POWER_OUT_HI];
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInLos: %d", i, rstaData.at(i).stOaAlm.bInLos);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInLow: %d", i, rstaData.at(i).stOaAlm.bInLow);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInHigh: %d", i, rstaData.at(i).stOaAlm.bInHigh);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bOutLos: %d", i, rstaData.at(i).stOaAlm.bOutLos);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bOutLow: %d", i, rstaData.at(i).stOaAlm.bOutLow);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bOutHigh: %d", i, rstaData.at(i).stOaAlm.bOutHigh);
#endif
#if defined __HCF__ || defined __OCS__
            rstaData.at(i).stOaAlm.bInLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[PA_IN_PWR];
            rstaData.at(i).stOscCurData.bLaserSafetyRxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[SAFTY_RX_PWR];
            rstaData.at(i).stOscCurData.bLaserSafetyTxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[SAFTY_TX_PWR];
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOaAlm.bInLos: %d", i, rstaData.at(i).stOaAlm.bInLos);
            LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).stOscCurData.bLaserSafetyRxLos: %d", i, rstaData.at(i).stOscCurData.bLaserSafetyRxLos);
#endif
#ifdef __OLSP__
            rstaData.at(i).stOaAlm.bInLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[PD1_EDFA2_IN_PWR];
#endif
        }

        /* rstaData.at(i).stOaAlm.bOutLos = false; */
        rstaData.at(i).stOaAlm.bMsaInLos = false;
        rstaData.at(i).stOaAlm.bMsaOutLos = false;

        rstaData.at(i).stOaAlm.bRFL = pstEdfaAlm->stModuleAlm.Rfl[i];
        rstaData.at(i).stOaAlm.bOOG = pstEdfaAlm->stModuleAlm.OOG[i];
        rstaData.at(i).stOaAlm.bApr = pstEdfaAlm->stModuleAlm.Apr[i];
        rstaData.at(i).stOaAlm.bOOP = false;
        LOG_WRITE(EN_LOG_DEBUG, "rstaData.at(%d).EOP: %d", i, pstEdfaAlm->stModuleAlm.EOP[i]);

        for (j = 0; j < 8; j++) {
            rstaData.at(i).stOaAlm.abPumpEOL[j] = pstEdfaAlm->stPumpIldAlm.aucPumpIld[i][j];;
        }

        for (j = 0; j < 8; j++) {
            rstaData.at(i).stOaAlm.abPumpTemp[j] = pstEdfaAlm->stPumpTempAlm.aucPumpTemp[i][j];
        }

        rstaData.at(i).stOaAlm.bCaseTemp = pstEdfaAlm->stModuleAlm.ucCaseTmp;
        rstaData.at(i).stOaAlm.bFiberTemp = pstEdfaAlm->stModuleAlm.FiberTmp;
        rstaData.at(i).stOaAlm.bHeaterTemp = pstEdfaAlm->stModuleAlm.HeaterTmp;

        /* osc */
        /* if (1 == i) { */
        rstaData.at(i).stOscCurData.bFailure = pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_Failure];
        rstaData.at(i).stOscCurData.bInnerErr = pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_InnerErr];
        rstaData.at(i).stOscCurData.bOffLine = pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_OffLine];
        rstaData.at(i).stOscCurData.bLinkDown = pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_PhyLinkDown];
#ifdef __NR1004__
        rstaData.at(i).stOscCurData.bRxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_R_RX_LOS];
        rstaData.at(i).stOscCurData.bRxLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_R_RX_POWER_IN_LOW];
        rstaData.at(i).stOscCurData.bRxHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_R_RX_POWER_IN_HI];
        rstaData.at(i).stOscCurData.bTxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_T_TX_LOS];
        rstaData.at(i).stOscCurData.bTxLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_T_TX_POWER_OUT_LOW];
        rstaData.at(i).stOscCurData.bTxHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_T_TX_POWER_OUT_HI];
        rstaData.at(i).stOscCurData.bDropLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_R_DROP_LOS];
        rstaData.at(i).stOscCurData.bDropLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_R_DROP_POWER_IN_LOW];
        rstaData.at(i).stOscCurData.bDropHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_R_DROP_POWER_IN_HI];
        rstaData.at(i).stOscCurData.bAddLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_T_ADD_LOS];
        rstaData.at(i).stOscCurData.bAddLow = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_T_ADD_POWER_OUT_LOW];
        rstaData.at(i).stOscCurData.bAddHigh = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_T_ADD_POWER_OUT_HI];
        rstaData.at(i).stOscCurData.bMisMatch = pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_MisMatch];
#else
        rstaData.at(i).stOscCurData.bRxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_RX_PWR]; /* pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_RxLos]; */
        rstaData.at(i).stOscCurData.bTxLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_TX_PWR]; /* pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_TxLos]; */
        rstaData.at(i).stOscCurData.bDropLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_DROP_PWR]; /* pstEdfaAlm->stOscAlm.aucOsc[OSC_IDX_DropLos]; */
        rstaData.at(i).stOscCurData.bAddLos = pstEdfaAlm->stPdLosAlm.aucPdLosAlarm[OSC_ADD_PWR];
#endif
        LOG_WRITE(EN_LOG_DEBUG, "m_uiDevIdx = %d", i);
        LOG_WRITE(EN_LOG_DEBUG, "bOscOffLine = %d", rstaData.at(i).stOscCurData.bOffLine);
        LOG_WRITE(EN_LOG_DEBUG, "bOscRxLos = %d", rstaData.at(i).stOscCurData.bRxLos);
        LOG_WRITE(EN_LOG_DEBUG, "bOscRxLow = %d", rstaData.at(i).stOscCurData.bRxLow);
        LOG_WRITE(EN_LOG_DEBUG, "bOscRxHigh = %d", rstaData.at(i).stOscCurData.bRxHigh);
        LOG_WRITE(EN_LOG_DEBUG, "bOscLinkDown = %d", rstaData.at(i).stOscCurData.bLinkDown);
        LOG_WRITE(EN_LOG_DEBUG, "bOscTxLos = %d", rstaData.at(i).stOscCurData.bTxLos);
        LOG_WRITE(EN_LOG_DEBUG, "bOscTxLow = %d", rstaData.at(i).stOscCurData.bTxLow);
        LOG_WRITE(EN_LOG_DEBUG, "bOscTxHigh = %d", rstaData.at(i).stOscCurData.bTxHigh);
        LOG_WRITE(EN_LOG_DEBUG, "bRFL = %d", rstaData.at(i).stOaAlm.bRFL);
        LOG_WRITE(EN_LOG_DEBUG, "bApr = %d", rstaData.at(i).stOaAlm.bApr);
        LOG_WRITE(EN_LOG_DEBUG, "bOOG = %d", rstaData.at(i).stOaAlm.bOOG);
        LOG_WRITE(EN_LOG_DEBUG, "bPumpEOL = %d", rstaData.at(i).stOaAlm.abPumpEOL[0]);
        LOG_WRITE(EN_LOG_DEBUG, "bPumpTemp = %d", rstaData.at(i).stOaAlm.abPumpTemp[0]);
        LOG_WRITE(EN_LOG_DEBUG, "bCaseTemp = %d", rstaData.at(i).stOaAlm.bCaseTemp);
        LOG_WRITE(EN_LOG_DEBUG, "bFiberTemp = %d", rstaData.at(i).stOaAlm.bFiberTemp);
        LOG_WRITE(EN_LOG_DEBUG, "bHeaterTemp = %d", rstaData.at(i).stOaAlm.bHeaterTemp);
        /* } */

        rstaData.at(i).stOaCurData.abApsdStatus = pstEdfaAlm->stModuleAlm.Apsd[i]; /* pstEdfaAlm->stApsdAlm.ucApsd; */
        rstaData.at(i).stOaAlm.bApsd = pstEdfaAlm->stModuleAlm.Apsd[i]; /* pstEdfaAlm->stApsdAlm.ucApsd; */
        LOG_WRITE(EN_LOG_DEBUG, "abApsdStatus = %d", rstaData.at(i).stOaCurData.abApsdStatus);
        LOG_WRITE(EN_LOG_DEBUG, "bApsd = %d", rstaData.at(i).stOaAlm.bApsd);

    }

    return true;
}

void
COaSwMasterSpiProto::SetDfbEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable, int32_t iPower)
{
    /*float fData = (float)sTilt / 100; */

    /*rstaMsg = "tilt " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 8;
    uint32_t uiEnable = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_DFB_ONFF;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiEnable = htonl(uiEnable); */
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    iPower = iPower * 100;
    staBody.append((char *)&iPower, sizeof(iPower));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetOscEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /*float fData = (float)sTilt / 100; */

    /*rstaMsg = "tilt " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_OSC_ONOFF;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiEnable = htonl(uiEnable); */
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetOtdrSwitchPos(uint32_t uiId, std::string &rstaMsg, uint32_t ulPos)
{
    /*float fData = (float)sTilt / 100; */

    /*rstaMsg = "tilt " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_OTDR_SWITCH;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
#if 0
    if (ulPos > 0 && ulPos < 4) {
        ulPos = ulPos - 1;
    }
#endif
    /* ulPos = htonl(ulPos); */
    LOG_WRITE(EN_LOG_NOTICE, "SetOtdrSwitchPos, port %u, %u, %u", (ulPos & 0xff00) >> 16, ulPos & 0xff, ulPos);
    staBody.append((char *)&ulPos, sizeof(ulPos));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetOscAddThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    CMN_THR_HYS_ST stCmnThrHys;
    CMN_THR_HYS_ST stCmnThrHysBE;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_OSC_ADD_THR_HYS;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    memset(&stCmnThrHys, 0, sizeof(stCmnThrHys));
    memset(&stCmnThrHysBE, 0, sizeof(stCmnThrHysBE));
    stCmnThrHys.stCmnThr.iFailLow = sThr;
    stCmnThrHys.stCmnHys.iFailLow = sHys;
    /* UTIL_htonl((uint32_t *)&stCmnThrHysBE, (uint32_t *)&stCmnThrHys, sizeof(stCmnThrHysBE) / sizeof(int32_t)); */
    staBody.append((char *)&stCmnThrHys, sizeof(stCmnThrHys));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetOscDropThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    CMN_THR_HYS_ST stCmnThrHys;
    CMN_THR_HYS_ST stCmnThrHysBE;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_OSC_DROP_THR_HYS;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    memset(&stCmnThrHys, 0, sizeof(stCmnThrHys));
    memset(&stCmnThrHysBE, 0, sizeof(stCmnThrHysBE));
    stCmnThrHys.stCmnThr.iFailLow = sThr;
    stCmnThrHys.stCmnHys.iFailLow = sHys;
    /* UTIL_htonl((uint32_t *)&stCmnThrHysBE, (uint32_t *)&stCmnThrHys, sizeof(stCmnThrHysBE) / sizeof(int32_t)); */
    staBody.append((char *)&stCmnThrHys, sizeof(stCmnThrHys));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetOscRxThrHys(uint32_t uiId, std::string &rstaMsg, int16_t sThr, int16_t sHys)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    CMN_THR_HYS_ST stCmnThrHys;
    CMN_THR_HYS_ST stCmnThrHysBE;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_OSC_RX_THR_HYS;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    memset(&stCmnThrHys, 0, sizeof(stCmnThrHys));
    memset(&stCmnThrHysBE, 0, sizeof(stCmnThrHysBE));
    stCmnThrHys.stCmnThr.iFailLow = sThr;
    stCmnThrHys.stCmnHys.iFailLow = sHys;
    /* UTIL_htonl((uint32_t *)&stCmnThrHysBE, (uint32_t *)&stCmnThrHys, sizeof(stCmnThrHysBE) / sizeof(int32_t)); */
    staBody.append((char *)&stCmnThrHys, sizeof(stCmnThrHys));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetApsdEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    /*float fData = (float)sTilt / 100; */

    /*rstaMsg = "tilt " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_APSD_ONOFF;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiEnable = htonl(uiEnable); */
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetUpgState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{

    /*float fData = (float)uiGain / 100; */

    /*rstaMsg = "mode g " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_SET_SLC_UPG_STATE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiState = htonl(uiState); */
    staBody.append((char *)&uiState, sizeof(uiState));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetLedState(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{

    /*float fData = (float)uiGain / 100; */

    /*rstaMsg = "mode g " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_SET_SLC_LED_STATE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiState = htonl(uiState); */
    staBody.append((char *)&uiState, sizeof(uiState));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetLedAlm(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{

    /*float fData = (float)uiGain / 100; */

    /*rstaMsg = "mode g " + std::to_string(uiId + 1) + " " + std::to_string(fData) + "\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_SET_SLC_LED_ALM;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiState = htonl(uiState); */
    staBody.append((char *)&uiState, sizeof(uiState));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::GetOtdrCaliCmd(std::string &rstaMsg)
{

    /*rstaMsg = "privinfo\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_SLC_OTDR_CALI;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);
}

bool
COaSwMasterSpiProto::GetOtdrCaliRsp(const std::string &c_rstaMsg, COtdrStartOffsetAll &rstCali)
{

    COtdrStartOffsetAll *pstCaliInfo = nullptr;;
    uint8_t *pucMsg = nullptr;

    LOG_WRITE(EN_LOG_DEBUG, "GetOtdrCaliRsp +++");

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
#if 0
    for (long unsigned int i = 0; i < sizeof(COtdrStartOffsetAll); i++) {
        if (0 == (i % 16)) {
            printf("\n");
        }
        printf("%02x ", pucMsg[i]);
    }
    printf("\n");
    printf("\n");
#endif

    pstCaliInfo = (COtdrStartOffsetAll *)pucMsg;
#if 0
    LOG_WRITE(EN_LOG_NOTICE, "uiPositionOffset = %d", pstCaliInfo->stOtdrStartOffsetPort[0].uiPositionOffset);
    LOG_WRITE(EN_LOG_NOTICE, "fRev00 = %f", pstCaliInfo->stOtdrStartOffsetPort[0].fRev0[0]);
    LOG_WRITE(EN_LOG_NOTICE, "fRev01 = %f", pstCaliInfo->stOtdrStartOffsetPort[0].fRev0[1]);
    LOG_WRITE(EN_LOG_NOTICE, "fLossOffset = %f", pstCaliInfo->stOtdrStartOffsetPort[0].fLossOffset);

    LOG_WRITE(EN_LOG_NOTICE, "uiPositionOffset = %d", pstCaliInfo->stOtdrStartOffsetPort[1].uiPositionOffset);
    LOG_WRITE(EN_LOG_NOTICE, "fRev00 = %f", pstCaliInfo->stOtdrStartOffsetPort[1].fRev0[0]);
    LOG_WRITE(EN_LOG_NOTICE, "fRev01 = %f", pstCaliInfo->stOtdrStartOffsetPort[1].fRev0[1]);
    LOG_WRITE(EN_LOG_NOTICE, "fLossOffset = %f", pstCaliInfo->stOtdrStartOffsetPort[1].fLossOffset);

    LOG_WRITE(EN_LOG_NOTICE, "uiPositionOffset = %d", pstCaliInfo->stOtdrStartOffsetPort[2].uiPositionOffset);
    LOG_WRITE(EN_LOG_NOTICE, "fRev00 = %f", pstCaliInfo->stOtdrStartOffsetPort[2].fRev0[0]);
    LOG_WRITE(EN_LOG_NOTICE, "fRev01 = %f", pstCaliInfo->stOtdrStartOffsetPort[2].fRev0[1]);
    LOG_WRITE(EN_LOG_NOTICE, "fLossOffset = %f", pstCaliInfo->stOtdrStartOffsetPort[2].fLossOffset);
#endif
    memcpy(&rstCali, (char *)pstCaliInfo, sizeof(COtdrStartOffsetAll));

    return true;

}

void
COaSwMasterSpiProto::GetOcmCaliCmd(std::string &rstaMsg)
{

    /*rstaMsg = "privinfo\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_SLC_OCM_CALI;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);
}

bool
COaSwMasterSpiProto::GetOcmCaliRsp(const std::string &c_rstaMsg, COcmCali &rstCali)
{
    COcmCali *pstCaliInfo = nullptr;;
    uint8_t *pucMsg = nullptr;

    LOG_WRITE(EN_LOG_DEBUG, "GetOcmCaliRsp +++");

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    pstCaliInfo = (COcmCali *)pucMsg;
    #if 0
    for (int i = 0; i < EN_OCM_PORT_CNT; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K3 = %f", i, pstCaliInfo->stOcmPortCali[i].dK3L);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K2 = %f", i, pstCaliInfo->stOcmPortCali[i].dK2L);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K1 = %f", i, pstCaliInfo->stOcmPortCali[i].dK1L);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]C = %f", i, pstCaliInfo->stOcmPortCali[i].dCL);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K3 = %f", i, pstCaliInfo->stOcmPortCali[i].dK3N);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K2 = %f", i, pstCaliInfo->stOcmPortCali[i].dK2N);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K1 = %f", i, pstCaliInfo->stOcmPortCali[i].dK1N);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]C = %f", i, pstCaliInfo->stOcmPortCali[i].dCN);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K3 = %f", i, pstCaliInfo->stOcmPortCali[i].dK3H);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K2 = %f", i, pstCaliInfo->stOcmPortCali[i].dK2H);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]K1 = %f", i, pstCaliInfo->stOcmPortCali[i].dK1H);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]C = %f", i, pstCaliInfo->stOcmPortCali[i].dCH);
    }
    #endif
    memcpy(&rstCali, (char *)pstCaliInfo, sizeof(COcmCali));

    return true;

}

void
COaSwMasterSpiProto::SetOtdrCali(uint32_t uiId, std::string &rstaMsg, COtdrStartOffsetAll &rstCali)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(COtdrStartOffsetAll);
    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_SLC_OTDR_CALI;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&rstCali, sizeof(COtdrStartOffsetAll));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetOcmCali(uint32_t uiId, std::string &rstaMsg, COcmCali &rstCali)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(COcmCali);
    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_SLC_OCM_CALI;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&rstCali, sizeof(COcmCali));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetManualVoaEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(uiId);
    stSpiCmdBody.ucDid = OA_SPI_DID_MANUAL_VOA_CONTROL;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiEnable = htonl(uiEnable); */
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetPaddingVoaConfigPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_PADDING_VOA_POWER;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiPower, sizeof(uiPower));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);


    LOG_WRITE(EN_LOG_NOTICE, "Send PaddingVoaConfigPower: %s", rstaMsg.c_str());
}

void
COaSwMasterSpiProto::SetManualGainEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    uint32_t uiEnable = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(uiId);
    stSpiCmdBody.ucDid = OA_SPI_DID_MANUAL_GAIN_CONTROL;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiEnable = htonl(uiEnable); */
    staBody.append((char *)&uiEnable, sizeof(uiEnable));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto:: SetThrHys(uint32_t uiId, std::string &rstaMsg, uint32_t uiPortIdx, uint32_t uiType, int16_t sThr, int16_t sHys)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(CMN_GENERIC_THR_HYS_ST);
    CMN_GENERIC_THR_HYS_ST stCmnThrHys;
    uint16_t usId = uiId;
    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_THR_HYS;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);

    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    memset(&stCmnThrHys, 0, sizeof(stCmnThrHys));

    stCmnThrHys.uiPortIdx = uiPortIdx;
    stCmnThrHys.uiType = uiType;
    stCmnThrHys.iThr = sThr;
    stCmnThrHys.iHys = sHys;

    staBody.append((char *)&stCmnThrHys, sizeof(stCmnThrHys));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "SetThrHys, edfa[%d] pd%[d], type[%d], thr[%d, hys[%d]", uiPortIdx, uiType, sThr, sHys);
}

void
COaSwMasterSpiProto::SetPumpCurrent(uint32_t uiId, std::string &rstaMsg, uint32_t uiPumpIdx, int16_t sCurrent)
{
    int32_t iCurrent = sCurrent;
    std::string staBody;
    uint16_t ulWLen = 8;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_PUMP_CURRENT;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiPumpIdx, sizeof(uiPumpIdx));
    staBody.append((char *)&iCurrent, sizeof(iCurrent));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
}

void
COaSwMasterSpiProto::SetAlmMask(uint32_t uiId, std::string &rstaMsg, int32_t iMaskIdx, int32_t iMaskVal)
{

    std::string staBody;
    uint16_t ulWLen = 8;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_PORT_ALM_MASK;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    staBody.append((char *)&iMaskIdx, sizeof(iMaskIdx));
    staBody.append((char *)&iMaskVal, sizeof(iMaskVal));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::GetEventLogCmd(std::string &rstaMsg)
{

    /*rstaMsg = "privinfo\r\n"; */
    std::string staBody;
    uint16_t ulWLen = 0;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(0);
    stSpiCmdBody.ucDid = OA_SPI_DID_EVENT_LOG;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);
}

bool
COaSwMasterSpiProto::GetEventLogRsp(const std::string &c_rstaMsg, COaEventLogAllData &rstEventLogAllData)
{
    COaEventLogAllData *pstEventLogAllData = nullptr;;
    uint8_t *pucMsg = nullptr;

    LOG_WRITE(EN_LOG_DEBUG, "GetEventLogRsp +++");

    pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    pstEventLogAllData = (COaEventLogAllData *)pucMsg;

#if 1
    LOG_WRITE(EN_LOG_NOTICE, "[%d]uiBALostTimeCount   = 0x%0x", 0, pstEventLogAllData->uiBaLostTimeCount);
    for (int i = 0; i < 4; i++) {
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostTimeCount     = 0x%0x", i, pstEventLogAllData->stOaEventLogData[i].uiLostTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLostPD            = 0x%0x", i, pstEventLogAllData->stOaEventLogData[i].uiLostPD);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiLinkDownTimeCount = 0x%0x", i, pstEventLogAllData->stOaEventLogData[i].uiLinkDownTimeCount);
        LOG_WRITE(EN_LOG_NOTICE, "[%d]uiApsdTimeCount     = 0x%0x", i, pstEventLogAllData->stOaEventLogData[i].uiApsdTimeCount);
    }
#endif
    memcpy(&rstEventLogAllData, (char *)pstEventLogAllData, sizeof(COaEventLogAllData));

    return true;

}

bool
COaSwMasterSpiProto::SetShutterState(uint32_t uiId, std::string &rstaMsg, bool block)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(uint32_t);
    uint16_t usId = uiId;
    uint32_t uiData = block;  /* block ? 0 : 1; */

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_SHUTTER_STATE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons(ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiData, sizeof(uiData));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "SetShutterState %s: %u", __func__, uiData);
    return true;
}

bool
COaSwMasterSpiProto::GetShutterState(uint32_t uiId, std::string &rstaMsg)
{
    std::string staBody;
    uint16_t ulWLen = 0;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_SHUTTER_STATE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons(ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));

    CFrameProto::Pack(1, 0, OA_SPI_CMD_READ, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_DEBUG, "Send %s", __func__);

    return true;
}

bool
COaSwMasterSpiProto::GetShutterStateRsp(uint32_t uiId, const std::string &c_rstaMsg, bool &block)
{
    uint8_t *pucMsg = (uint8_t *)c_rstaMsg.data() + sizeof(ST_OA_SPI_MSG_BODY);
    block = *((uint32_t *)pucMsg) == 0;
    LOG_WRITE(EN_LOG_DEBUG, "%s, %u, %d", __func__, *((uint32_t *)pucMsg), block);
    return true;
}

void
COaSwMasterSpiProto::SetReadyTrigger(uint32_t uiId, std::string &rstaMsg, uint32_t uiState)
{

    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_Set_DID_EDFA_READY_TRIGGER;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiState = htonl(uiState); */
    staBody.append((char *)&uiState, sizeof(uiState));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
    LOG_WRITE(EN_LOG_NOTICE, "%s, uiState: %d", __func__, uiState);
}

void
COaSwMasterSpiProto::SetAutoGainMode(uint32_t uiId, std::string &rstaMsg, uint32_t uiMode)
{

    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_Set_DID_EDFA_AUTOGAIN_MODE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    /* uiState = htonl(uiState); */
    staBody.append((char *)&uiMode, sizeof(uiMode));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);
    LOG_WRITE(EN_LOG_NOTICE, "%s, uiMode: %d", __func__, uiMode);
}

bool
COaSwMasterSpiProto::SetRemoteInterlockEnable(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(uint32_t);
    uint16_t usId = uiId;
    uint32_t uiData = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_REMOTE_INTERLOCK_ENABLE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons(ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiData, sizeof(uiData));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "SetRemoteInterlockEnable %s: %u", __func__, uiData);
    return true;
}

bool
COaSwMasterSpiProto::SetDummyAdminKey(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(uint32_t);
    uint16_t usId = uiId;
    uint32_t uiData = bEnable;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_DUMMY_ADMIN_KEY_ENABLE;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons(ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiData, sizeof(uiData));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "SetDummyAdminKey %s: %u", __func__, uiData);
    return true;
}

bool
COaSwMasterSpiProto::SetPanelSafeCmpt(uint32_t uiId, std::string &rstaMsg, bool bEnable)
{
    std::string staBody;
    uint16_t ulWLen = sizeof(uint32_t);
    uint16_t usId = uiId;
    uint32_t uiData = bEnable;

    /* https://jira.atlassian.molexcloud.com/browse/M120-1101
       Panel safety component enable
       0xF000F: Panel Disable
       0x00000: Panel Enable
     */
    if ( true == bEnable) {
        uiData = 0;
    } else {
        uiData = 0xF000F;
    }

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_PIF_SAFE_EN;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons(ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiData, sizeof(uiData));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

    LOG_WRITE(EN_LOG_NOTICE, "SetPanelSafeCmpt %s: %u", __func__, uiData);
    return true;
}

void
COaSwMasterSpiProto::SetApcTargetPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_EDFA_APC_TAR;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiPower, sizeof(uiPower));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

void
COaSwMasterSpiProto::SetReadyPower(uint32_t uiId, std::string &rstaMsg, uint32_t uiPower)
{
    std::string staBody;
    uint16_t ulWLen = 4;
    uint16_t usId = uiId;

    ST_OA_SPI_MSG_BODY stSpiCmdBody = {0};

    stSpiCmdBody.usOid = htons(OA_SPI_OID_FIX);
    stSpiCmdBody.usMid = htons(usId);
    stSpiCmdBody.ucDid = OA_SPI_DID_READY_POWER;
    stSpiCmdBody.ucInst = OA_SPI_INST_FIX;
    stSpiCmdBody.usAddr = htons(OA_SPI_ADDR_FIX);
    stSpiCmdBody.usDataLen = htons((uint16_t)ulWLen);
    staBody.append((char *)&stSpiCmdBody, sizeof(stSpiCmdBody));
    staBody.append((char *)&uiPower, sizeof(uiPower));
    CFrameProto::Pack(1, 0, OA_SPI_CMD_WRITE, staBody, rstaMsg, MODULE_TPYE_OA);

}

