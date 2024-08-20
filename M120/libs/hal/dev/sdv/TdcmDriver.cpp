/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      TdcmDriver.cpp
   Author:        Zhen Zhang
   Date:          2018-11-08
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-11-08] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "TdcmDriver.h"

#define IIC_CMD_GET_STATUS                      0x00
#define IIC_CMD_RESET                           0x28
#define IIC_CMD_SET_FREQUENCY                   0x2E
#define IIC_CMD_GET_FREQUENCY                   0x2F
#define IIC_CMD_SET_DISPERSION                  0x30
#define IIC_CMD_GET_DISPERSION                  0x31
#define IIC_CMD_GET_RTD_TEMPERATURE             0x17
#define IIC_CMD_FREEZE_TDC                      0x40
#define IIC_CMD_GET_MEAS_DISPERSION             0x41
#define IIC_CMD_EN_DEVICE                       0x1E
#define IIC_CMD_DIS_DEVICE                      0x1F

#define IIC_CMD_SET_STARTUP_BYTE                0x34
#define IIC_CMD_GET_STARTUP_BYTE                0x35
#define IIC_CMD_SAVE_NOMINAL_SETTINGS           0x36
#define IIC_CMD_GET_NOMINAL_SETTINGS            0x37

#define IIC_CMD_GET_CH_PLAN                     0x3B
#define IIC_CMD_GET_TUNING_RANGE                0x3C

#define IIC_CMD_GET_VERSION                     0x0F
#define IIC_CMD_READ_MANUFACTURER_NAME          0x0E
#define IIC_CMD_READ_MODEL_NUMBER               0x27
#define IIC_CMD_READ_SERIAL_NUMBER              0x29
#define IIC_CMD_READ_MANUFACTURING_DATE         0x2B


typedef struct tdcmx_status_t
{

    union
    {
        struct
        {
            uint32_t bTec1InRange              : 1;         /* Bit0:  */
            uint32_t bTec2InRange              : 1;         /* Bit1:  */
            uint32_t bTec3InRange              : 1;         /* Bit2:  */
            uint32_t bTec4InRange              : 1;         /* Bit3:  */

            uint32_t bTec1TempReach            : 1;         /* Bit4: temperature limit reached */
            uint32_t bTec2TempReach            : 1;         /* Bit5: */
            uint32_t bTec3TempReach            : 1;         /* Bit6:  */
            uint32_t bTec4TempReach            : 1;         /* Bit7:  */

            uint32_t bDispersionStability      : 1;         /* Bit8:  */
            uint32_t bRsv9_15                  : 7;         /* Bit9 ~ 15:  */

            uint32_t bRsv16_17                 : 2;         /* Bit16 ~ 17:  */
            uint32_t bInvalidE2Rom             : 1;         /* Bit18:  */
            uint32_t bTdcmxReady               : 1;         /* Bit19:  */

            uint32_t bTdcmxActive              : 1;         /* Bit20:  */
            uint32_t bCmdErr                   : 1;         /* Bit21: */
            uint32_t bOverRunErr               : 1;         /* Bit22: */
            uint32_t bBusyErr                  : 1;         /* Bit23:  */

            uint32_t bRsv24_31                 : 8;         /* Bit24 ~ 31:  */

        };
        uint32_t uiStatus;
    };

} TDCMX_STATUS_T;

typedef struct tdcmx_version_t
{
    TDCMX_STATUS_T stStatus;
    UCHAR ucVersionMajor;
    UCHAR ucVersionMinor;

}TDCMX_VERSION_T;


typedef struct get_frequency_t
{
    TDCMX_STATUS_T stStatus;
    float fFrequency;

}GET_FREQUENCY_T;



typedef struct get_dispersion_t
{
    TDCMX_STATUS_T stStatus;
    float fDispersion;

}GET_DISPERSION_T;



/*==============================function=========================*/

bool
CI2cTdcm::IsOnline(void)
{

    return true;
}


bool
CI2cTdcm::GetMfgItem(uint8_t ucReg, char *pcItem)
{
    uint8_t aucBuf[128] = {0};

    if ( NULL == pcItem ) {
        return false;
    }

    if (nullptr == m_stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpI2cPtr is nullptr");
        return false;
    }

    /*if(!m_stpI2cPtr->Write(m_uiHwIdx, &ucReg, 1))
       {
            LOG_WRITE(EN_LOG_ERROR, "I2C #%d write error. ucReg = %x", m_uiHwIdx, ucReg);
            return false;
       }*/

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &ucReg, 1, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "I2C #%d read error.", m_uiDevAddr);
        return false;
    }

    memcpy(pcItem, (aucBuf + 4), MFG_INFO_LEN_MAX);

    return true;

}


bool
CI2cTdcm::GetMfg(CMfg &rstMfg)
{
    UCHAR aucBuf[64];
    TDCMX_VERSION_T *pstVer = NULL;

    if (nullptr == m_stpI2cPtr) {
        return false;
    }
    memset(&rstMfg, 0, sizeof(rstMfg));

    LOG_WRITE(EN_LOG_NOTICE, "CI2cTdcm::GetMfg +++");

    memset(aucBuf, 0, sizeof(aucBuf));
    if (!GetMfgItem(IIC_CMD_GET_VERSION, (char *)aucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "get FwVer error.");
        return false;
    }

    pstVer = (TDCMX_VERSION_T *)aucBuf;
    snprintf(rstMfg.acFwVer, (MFG_INFO_LEN_MAX - 1), "%hhd.%hhd", pstVer->ucVersionMajor, pstVer->ucVersionMinor);

    if (!GetMfgItem(IIC_CMD_READ_MANUFACTURER_NAME, rstMfg.acManufacturer)) {
        LOG_WRITE(EN_LOG_ERROR, "get Manufacturer error.");
        return false;
    }

    if (!GetMfgItem(IIC_CMD_READ_MODEL_NUMBER, rstMfg.acPN)) {
        LOG_WRITE(EN_LOG_ERROR, "get PN error.");
        return false;
    }

    if (!GetMfgItem(IIC_CMD_READ_SERIAL_NUMBER, rstMfg.acSN)) {
        LOG_WRITE(EN_LOG_ERROR, "get SN error.");
        return false;
    }

    if (!GetMfgItem(IIC_CMD_READ_MANUFACTURING_DATE, rstMfg.acMfgDate)) {
        LOG_WRITE(EN_LOG_ERROR, "get mfg date error.");
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);

    LOG_WRITE(EN_LOG_NOTICE, "CI2cTdcm::GetMfg ---");

    return true;
}

bool
CI2cTdcm::GetStatusData(CTdcmStatusData &rstData)
{
    if (!GetFrequency(rstData.fFrequency)) {
        LOG_WRITE(EN_LOG_ERROR, "get tdcm frequency error.");
        return false;
    }

    if (!GetDispersion(rstData.fDispersion)) {
        LOG_WRITE(EN_LOG_ERROR, "get tdcm dispersion error.");
        return false;
    }

    return true;
}

bool
CI2cTdcm::GetCfgData(CTdcmCfgData &rstData)
{
    return true;
}

bool
CI2cTdcm::SetFrequency(float fFrequency)
{
    uint32_t uiDataW;
    uint8_t aucBufW[32] = {0};

    memcpy(&uiDataW, &fFrequency, 4);
    uiDataW = htonl(uiDataW);

    aucBufW[0] = IIC_CMD_SET_FREQUENCY;
    memcpy(&aucBufW[1], &uiDataW, sizeof(uiDataW));

    if (!m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, aucBufW, sizeof(uiDataW) + 1)) {
        LOG_WRITE(EN_LOG_ERROR, "frequency i2c#%d write error. cmd = %x, data = %d ",
                  m_uiDevAddr, aucBufW[0], uiDataW);
        return false;
    }

    return true;
}


static void
_U32ArrayHtonl(uint8_t *paucArray, uint32_t ulSize)
{
    uint32_t i;
    uint32_t ulTmp;
    uint32_t *pulArray = (uint32_t *)paucArray;

    for (i = 0; i < ulSize; i++, pulArray++) {
        ulTmp = *pulArray;
        *pulArray = htonl(ulTmp);

    }
}


bool
CI2cTdcm::GetFrequency(float &fFrequency)
{
    uint8_t aucBuf[32];
    uint8_t ucReg;

    memset(aucBuf, 0, sizeof(aucBuf));

    ucReg = IIC_CMD_GET_FREQUENCY;

    /*if(!m_stpI2cPtr->Write(m_uiHwIdx, &ucReg, 1))
       {
            LOG_WRITE(EN_LOG_ERROR, "frequency i2c#%d write error. ucReg = %x", m_uiHwIdx, ucReg);
            return false;
       }*/

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &ucReg, 1, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "frequency i2c#%d read error.", m_uiDevAddr);
        return false;
    }

    _U32ArrayHtonl(aucBuf, 2);

    GET_FREQUENCY_T *pstFrequency = (GET_FREQUENCY_T *)aucBuf;

    fFrequency = pstFrequency->fFrequency;

    return true;
}


bool
CI2cTdcm::SetDispersion(float fDispersion)
{
    uint32_t uiDataW;
    uint8_t aucBufW[32] = {0};

    memcpy((uint8_t *)&uiDataW, (uint8_t *)&fDispersion, 4);
    uiDataW = htonl(uiDataW);

    aucBufW[0] = IIC_CMD_SET_DISPERSION;
    memcpy(&aucBufW[1], (uint8_t *)&uiDataW, sizeof(uiDataW));

    if (!m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, aucBufW, sizeof(uiDataW) + 1)) {
        LOG_WRITE(EN_LOG_ERROR, "dispersion i2c#%d write error. cmd = %x, data = %d",
                  m_uiDevAddr, aucBufW[0], uiDataW);
        return false;
    }

    return true;
}


bool
CI2cTdcm::GetDispersion(float &fDispersion)
{

    uint8_t aucBuf[32];
    uint8_t ucReg;

    memset(aucBuf, 0, sizeof(aucBuf));

    ucReg = IIC_CMD_GET_DISPERSION;

    /*if(!m_stpI2cPtr->Write(m_uiHwIdx, &ucReg, 1))
       {
            LOG_WRITE(EN_LOG_ERROR, "dispersion i2c#%d write error. ucReg = %x", m_uiHwIdx, ucReg);
            return false;
       }*/

    if (!m_stpI2cPtr->Read(m_uiHwSubIdx, m_uiDevAddr, &ucReg, 1, aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "dispersion i2c#%d read error.", m_uiDevAddr);
        return false;
    }

    _U32ArrayHtonl(aucBuf, 2);

    GET_DISPERSION_T *pstDispersion = (GET_DISPERSION_T *)aucBuf;

    fDispersion = pstDispersion->fDispersion;

    return true;
}


bool
CI2cTdcm::Reset(uint8_t ucResetType, uint32_t uiSubIdx)
{
    LOG_WRITE(EN_LOG_TRACE, "CI2cTdcm::Reset ++ %d %d", uiSubIdx, ucResetType);
    uint8_t ucReg;

    if ((nullptr == m_stpI2cPtr) || (nullptr == m_stpProtoPtr)) {
        return false;
    }

    if (RST_MODE_COLD == ucResetType) {
        if (nullptr != m_stpResetBoardPtr) {
            m_stpResetBoardPtr->Reset(DEV_TYPE_TDCM);
        }
    } else {
        ucReg = IIC_CMD_RESET;
        if (!m_stpI2cPtr->Write(m_uiHwSubIdx, m_uiDevAddr, &ucReg, 1)) {
            LOG_WRITE(EN_LOG_ERROR, "SoftReset i2c#%d write %d error.", m_uiDevAddr, ucReg);
            return false;
        }
    }

    LOG_WRITE(EN_LOG_TRACE, "CI2cTdcm::Reset --");

    return true;
}


