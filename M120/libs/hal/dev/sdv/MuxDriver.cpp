/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      MuxDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-07-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-07-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include <math.h>
#include "MuxDriver.h"
#include "PdDriver.h"
#include "RspHandler.h"

#define MUX_MFG_INFO_LEN_MAX 32

typedef struct
{
    char acManufacturer[MUX_MFG_INFO_LEN_MAX];
    char acProductName[MUX_MFG_INFO_LEN_MAX];
    char acModuleType[MUX_MFG_INFO_LEN_MAX];
    char acSN[MUX_MFG_INFO_LEN_MAX];
    char acPN[MUX_MFG_INFO_LEN_MAX];
    char acMfgDate[MUX_MFG_INFO_LEN_MAX];
    char acHwVer[MUX_MFG_INFO_LEN_MAX];

    char acReserve[MUX_MFG_INFO_LEN_MAX];
} CMuxMfg;

#define PD_NUM (12)
#define MUX_CALI_DATA_MAX_LEN  (256)

typedef struct
{
    float afPd0[PD_NUM];
    float afPd1[PD_NUM];
    float fRefMuxTCali;
    float fRefDemuxRCali;

    char acReserve[MUX_CALI_DATA_MAX_LEN - 4 * (PD_NUM * 2 + 2)];

} CMuxPdCali;

static CMuxPdCali s_stPdCali = {0};

#define REFMUXCALI_LOW              (0.0)
#define REFMUXCALI_HIGH             (1.5)
#define REFMUXCALI_DEFAULT_VALUE    (0.8)
#define AWG_75G_PN                  "1834770039"
#define AWG_150G_PN1                "1834770043"
#define AWG_150G_PN2                "1831800033"
#define PN_Lable                    "18"
/* from keyan */
/*if PN == 1834770043 | PN == 1831800033
   {
   type = 150G
   }else if SN == 1834770043 | SN == 1831800033
   {
   type = 150G
   }
   else
   {
   type = 75G

   }
 */

/*
   bool __Is150GAwg(char *pcPN, char *pcSN){

    if(NULL == pcPN || NULL == pcSN){
        return false;
    }

    if (0 == strncmp(pcPN, AWG_150G_PN1, strlen(AWG_150G_PN1)) || 0 == strncmp(pcPN, AWG_150G_PN2, strlen(AWG_150G_PN2))){
        return true;
    }
    else if(0 == strncmp(pcSN, AWG_150G_PN1, strlen(AWG_150G_PN1)) || 0 == strncmp(pcSN, AWG_150G_PN2, strlen(AWG_150G_PN2))){
        return true;
    } else{
        return false;
    }
   }
 */

/*==============================function=========================*/


bool
CMuxSpiDriver::IsOnline(void)
{

    return true;
}


bool
CMuxSpiDriver::GetMfg(CMfg &rstMfg)
{
    CMuxMfg stMuxMfg;
    uint32_t uiStartAddr = 0x00;
    char *pcTmp = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "CMuxSpiDriver::GetMfg +++");

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpEEpromPtr is nullptr.");
        return false;
    }

#if 0
    CMuxMfg stMuxMfg1;

    memset(&stMuxMfg1, 0, sizeof(stMuxMfg1));
    strcpy(stMuxMfg1.acManufacturer, "Molex");
    strcpy(stMuxMfg1.acProductName, "400GZR");
    strcpy(stMuxMfg1.acModuleType, "MUX");
    strcpy(stMuxMfg1.acSN, "WH20341103");
    strcpy(stMuxMfg1.acPN, "1835260025");
    strcpy(stMuxMfg1.acMfgDate, "2020-09-07");
    strcpy(stMuxMfg1.acHwVer, "1.01");

    if (false == m_stpEEpromPtr->Write(uiStartAddr, (uint8_t *)&stMuxMfg1, sizeof(stMuxMfg1))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Mux Mfg error.");
        return false;
    }

    CDelay::Delay(10, MS);

#endif

    memset(&stMuxMfg, 0, sizeof(stMuxMfg));
    if (false == m_stpEEpromPtr->Read(uiStartAddr, (uint8_t *)&stMuxMfg, sizeof(stMuxMfg))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Mux Mfg error.");
        return false;
    }

    stMuxMfg.acManufacturer[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acProductName[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acModuleType[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acSN[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acPN[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acMfgDate[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acHwVer[MUX_MFG_INFO_LEN_MAX - 1] = '\0';
    stMuxMfg.acReserve[MUX_MFG_INFO_LEN_MAX - 1] = '\0';

    /* patch for RNR51 */
    LOG_WRITE(EN_LOG_NOTICE, "Read Mux mfg, PN(%s),SN(%s)", stMuxMfg.acPN, stMuxMfg.acSN);
    if (0 == strncmp(stMuxMfg.acPN, AWG_150G_PN1, strlen(AWG_150G_PN1))) {           /* 150G PN:T  SN:T */
        LOG_WRITE(EN_LOG_NOTICE, "150G MUX");
    } else if (0 == strncmp(stMuxMfg.acPN, AWG_150G_PN2, strlen(AWG_150G_PN2))) {    /* 150G PN:F  SN:T */
        LOG_WRITE(EN_LOG_NOTICE, "150G MUX, update PN");
        memset(stMuxMfg.acPN, 0, sizeof(stMuxMfg.acPN));
        memcpy(stMuxMfg.acPN, AWG_150G_PN1, strlen(AWG_150G_PN1));
    } else if (0 == strncmp(stMuxMfg.acSN, AWG_150G_PN1, strlen(AWG_150G_PN1)) || 0 == strncmp(stMuxMfg.acSN, AWG_150G_PN2, strlen(AWG_150G_PN2))) { /*150G PN:F SN:F, is reversed */
        LOG_WRITE(EN_LOG_NOTICE, "150G MUX, swap PNSN and update PN");
        memset(stMuxMfg.acSN, 0, sizeof(stMuxMfg.acSN));
        memcpy(stMuxMfg.acSN, stMuxMfg.acPN, sizeof(stMuxMfg.acSN) - 1);
        memset(stMuxMfg.acPN, 0, sizeof(stMuxMfg.acPN));
        memcpy(stMuxMfg.acPN, AWG_150G_PN1, strlen(AWG_150G_PN1));
    } else if (0 == strncmp(stMuxMfg.acPN, PN_Lable, strlen(PN_Lable))) {            /* 75G, write right PN */
        LOG_WRITE(EN_LOG_NOTICE, "75G MUX, update PN");
        memset(stMuxMfg.acPN, 0, sizeof(stMuxMfg.acPN));
        memcpy(stMuxMfg.acPN, AWG_75G_PN, strlen(AWG_75G_PN));
    } else if (0 == strncmp(stMuxMfg.acSN, PN_Lable, strlen(PN_Lable))) {            /* 75G, PNSN reversed and write right PN */
        LOG_WRITE(EN_LOG_NOTICE, "75G MUX, swap PNSN and update PN");
        memset(stMuxMfg.acSN, 0, sizeof(stMuxMfg.acSN));
        memcpy(stMuxMfg.acSN, stMuxMfg.acPN, sizeof(stMuxMfg.acSN) - 1);
        memset(stMuxMfg.acPN, 0, sizeof(stMuxMfg.acPN));
        memcpy(stMuxMfg.acPN, AWG_75G_PN, strlen(AWG_75G_PN));
    } else {
        LOG_WRITE(EN_LOG_NOTICE, "Other MUX");
    }

    strncpy(rstMfg.acManufacturer, stMuxMfg.acManufacturer, MFG_INFO_LEN_MAX - 1);
    strncpy(rstMfg.acProductName, stMuxMfg.acProductName, MFG_INFO_LEN_MAX - 1);
    strncpy(rstMfg.acModuleType, stMuxMfg.acModuleType, MFG_INFO_LEN_MAX - 1);

    strncpy(rstMfg.acPN, stMuxMfg.acPN, MFG_INFO_LEN_MAX - 1);
    strncpy(rstMfg.acSN, stMuxMfg.acSN, MFG_INFO_LEN_MAX - 1);

    /* strncpy(rstMfg.acMfgDate, stMuxMfg.acMfgDate, MFG_INFO_LEN_MAX - 1); */
    pcTmp = stMuxMfg.acMfgDate;
    memcpy(pcTmp + strlen("2000-01-01"), "T00:00:00Z", strlen("T00:00:00Z"));
    strncpy(rstMfg.acMfgDate, pcTmp, MFG_INFO_LEN_MAX - 1);

    strncpy(rstMfg.acHwVer, stMuxMfg.acHwVer, MFG_INFO_LEN_MAX - 1);

    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acManufacturer = %s", stMuxMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acProductName = %s", stMuxMfg.acProductName);
    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acModuleType = %s", stMuxMfg.acModuleType);
    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acSN = %s", stMuxMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acPN = %s", stMuxMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acMfgDate = %s", stMuxMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "stMuxMfg.acHwVer = %s", stMuxMfg.acHwVer);


    LOG_WRITE(EN_LOG_NOTICE, "CMuxSpiDriver::GetMfg ---");

    return true;
}

bool
CMuxSpiDriver::GetCali(CMuxCaliData &rstData)
{
    uint32_t uiStartAddr = 0x100;

    CMuxPdCali stPdCali;
    float fRefMuxTCali = 0.0;
    float fRefDemuxRCali = 0.0;

    memset(&stPdCali, 0, sizeof(stPdCali));
    memset(&s_stPdCali, 0, sizeof(s_stPdCali));


    LOG_WRITE(EN_LOG_NOTICE, "CMuxSpiDriver::GetCali +++");

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpEEpromPtr is nullptr.");
        return false;
    }

    if (false == m_stpEEpromPtr->Read(uiStartAddr, (uint8_t *)&stPdCali, sizeof(stPdCali))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Mux Mfg error.");
        return false;
    }

#if 0
    for (int i = 0; i < 256; i++) {
        printf(" %02X", *((uint8_t *)&stPdCali + i));


        if (0 == (i + 1) % 16) {
            printf("\n");
        }
    }
    printf("\r\n");

    printf("pd0 K %f \r\n", stPdCali.afPd0[2]);
    printf("pd0 C %f \r\n", stPdCali.afPd0[5]);

    printf("pd1 K %f \r\n", stPdCali.afPd1[2]);
    printf("pd1 C %f \r\n", stPdCali.afPd1[5]);
#endif

    for (int i = 0; i < PD_NUM; i++) {
        s_stPdCali.afPd0[i] = UTIL_Ntohf(stPdCali.afPd0[i]);
        LOG_WRITE(EN_LOG_NOTICE, "stPdCali.afPd0[%d] = %f", i, s_stPdCali.afPd0[i]);
    }

    for (int i = 0; i < PD_NUM; i++) {
        s_stPdCali.afPd1[i] = UTIL_Ntohf(stPdCali.afPd1[i]);
        LOG_WRITE(EN_LOG_NOTICE, "stPdCali.afPd1[%d] = %f", i, s_stPdCali.afPd1[i]);
    }

    /* s_stPdCali.fRefMuxTCali = UTIL_Ntohf(stPdCali.fRefMuxTCali); */
    fRefMuxTCali = UTIL_Ntohf(stPdCali.fRefMuxTCali);
    if ( isnan(fRefMuxTCali) || fRefMuxTCali < REFMUXCALI_LOW || fRefMuxTCali > REFMUXCALI_HIGH ) {
        LOG_WRITE(EN_LOG_NOTICE, "fRefMuxTCali = %f", fRefMuxTCali);
        s_stPdCali.fRefMuxTCali = REFMUXCALI_DEFAULT_VALUE;
    } else {
        s_stPdCali.fRefMuxTCali = fRefMuxTCali;
    }
    rstData.dRefMuxTCali = s_stPdCali.fRefMuxTCali;

    /* s_stPdCali.fRefDemuxRCali = UTIL_Ntohf(stPdCali.fRefDemuxRCali); */
    fRefDemuxRCali = UTIL_Ntohf(stPdCali.fRefDemuxRCali);
    if ( isnan(fRefDemuxRCali) || fRefDemuxRCali < REFMUXCALI_LOW || fRefDemuxRCali > REFMUXCALI_HIGH) {
        LOG_WRITE(EN_LOG_NOTICE, "fRefDemuxRCali = %f", fRefDemuxRCali);
        s_stPdCali.fRefDemuxRCali = REFMUXCALI_DEFAULT_VALUE;
    } else {
        s_stPdCali.fRefDemuxRCali = fRefDemuxRCali;
    }
    rstData.dRefDemuxRCali = s_stPdCali.fRefDemuxRCali;

    LOG_WRITE(EN_LOG_NOTICE, "dRefMuxTCali = %f", s_stPdCali.fRefMuxTCali);
    LOG_WRITE(EN_LOG_NOTICE, "dRefDemuxRCali = %f", s_stPdCali.fRefDemuxRCali);

    LOG_WRITE(EN_LOG_NOTICE, "CMuxSpiDriver::GetCali ---");

    return true;
}

bool
CMuxSpiDriver::GetStatusData(CMuxStatusData &rstData)
{
    CPdKcLevel stPdKc = {0};

    int16_t sAdc0 = 0;
    int16_t sAdc1 = 0;

    float fK = 0.0, fC = 0.0;

    LOG_WRITE(EN_LOG_DEBUG, "CMuxSpiDriver::GetStatusData +++");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpSpiPtr is nullptr.");
        return false;
    }

    if (false == m_stpDrvPtr->Read(0, (uint8_t *)&sAdc0, std::make_shared<CRspHandler>(2))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Adc0 error.");
        return false;
    }

    if (false == m_stpDrvPtr->Read(1, (uint8_t *)&sAdc1, std::make_shared<CRspHandler>(2))) {
        LOG_WRITE(EN_LOG_ERROR, "Read Adc1 error.");
        return false;
    }

    sAdc0 = ntohs(sAdc0) >> 2;
    sAdc1 = ntohs(sAdc1) >> 2;

    /* PD 0 */
    memset(&stPdKc, 0, sizeof(stPdKc));
    memcpy(&stPdKc, s_stPdCali.afPd0, sizeof(stPdKc));

    CPdDriver::CalcKC(stPdKc, 25, fK, fC);
    rstData.asPdPower[0] = (fK * sAdc0 + fC) * 100;

    LOG_WRITE(EN_LOG_DEBUG, "pd0 = %d, fK = %f, fC = %f", rstData.asPdPower[0], fK, fC);

    /* PD 1 */
    fK = 0.0;
    fC = 0.0;
    memset(&stPdKc, 0, sizeof(stPdKc));
    memcpy(&stPdKc, s_stPdCali.afPd1, sizeof(stPdKc));

    CPdDriver::CalcKC(stPdKc, 25, fK, fC);
    rstData.asPdPower[1] = (fK * sAdc1 + fC) * 100;

    LOG_WRITE(EN_LOG_DEBUG, "pd1 = %d, fK = %f, fC = %f", rstData.asPdPower[1], fK, fC);

    CDelay::Delay(1, S);

    LOG_WRITE(EN_LOG_DEBUG, "CMuxSpiDriver::GetStatusData ---");

    return true;
}




