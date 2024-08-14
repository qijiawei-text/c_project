
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/io.h>

#include "sdv_jtag.h"
#include "sdv_cpld.h"
#include "./jtag/ivm_eprom.h"
#include "./jtag/vmopcode.h"


static UCHAR *m_pucCpldImg = NULL;
static ULONG m_ulCpldImgLen = 0;
static ULONG m_ulCpldImgPos = 0;

static UINT m_uiCpldIndex = CPLD0_IDX;

INT
SDV_JtagWritePin(UCHAR ucPin, UCHAR ucValue)
{

    UCHAR ucBuf = 0;

    /* Check parameters                                                         */
    if ((ucPin != JTAG_PIN_TDI) &&
        (ucPin != JTAG_PIN_TCK) &&
        (ucPin != JTAG_PIN_TMS)) {
        OPLK_LOG_PRINT_ERR("gpio pin error");
        goto ERR_LABEL;
    }

    if (CPLD0_IDX == m_uiCpldIndex) {
        if (0 != SDV_GetCpldValue(CPLD0_JTAG_UPG, 1, &ucBuf)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
    } else if (CPLD1_IDX == m_uiCpldIndex) {
        if (0 != SDV_GetCpldValue(CPLD1_JTAG_UPG, 1, &ucBuf)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
    }

    switch (ucPin) {
    case JTAG_PIN_TCK:
        if (ucValue) {
            ucBuf |= 0x01;
        } else {
            ucBuf &= (~0x01);
        }
        break;

    case JTAG_PIN_TMS:
        if (ucValue) {
            ucBuf |= 0x02;
        } else {
            ucBuf &= (~0x02);
        }
        break;

    case JTAG_PIN_TDI:
        if (ucValue) {
            ucBuf |= 0x04;
        } else {
            ucBuf &= (~0x04);
        }
        break;

    default:
        goto ERR_LABEL;
    }


    if (CPLD0_IDX == m_uiCpldIndex) {
        if (0 != SDV_SetCpldValue(CPLD0_JTAG_UPG, 1, &ucBuf)) {
            OPLK_LOG_PRINT_ERR("set cpld value err.");
            goto ERR_LABEL;
        }

    } else if (CPLD1_IDX == m_uiCpldIndex) {
        if (0 != SDV_SetCpldValue(CPLD1_JTAG_UPG, 1, &ucBuf)) {
            OPLK_LOG_PRINT_ERR("set cpld value err.");
            goto ERR_LABEL;
        }
    }

    return 0;

ERR_LABEL:
    return -1;
}


INT
SDV_JtagReadTDO(UCHAR *pucValue)
{
    UCHAR ucValue = 0;

    if (CPLD0_IDX == m_uiCpldIndex) {
        if (0 != SDV_GetCpldValue(CPLD0_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
    } else {
        if (0 != SDV_GetCpldValue(CPLD1_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
    }

    if (ucValue & (1 << 3)) {
        ucValue = 1;
    } else {
        ucValue = 0;
    }

    *pucValue = ucValue;

    return 0;

ERR_LABEL:
    return -1;
}

INT
SDV_JtagGetImg(UCHAR *pucData)
{
    if (NULL == pucData) {
        OPLK_LOG_PRINT_ERR("input null");
        goto ERR_LABEL;
    }

    if (m_ulCpldImgPos >= m_ulCpldImgLen) {
        OPLK_LOG_PRINT_ERR("data length error");
        goto ERR_LABEL;
    }

    *pucData = *(m_pucCpldImg + m_ulCpldImgPos);
    m_ulCpldImgPos++;

    return 0;

ERR_LABEL:
    return -1;
}

INT
SDV_JtagCpld(UCHAR ucCpldIdx, UCHAR *pucImg, ULONG ulLen)
{
    INT iRet = 0;
    UCHAR ucValue = 0;

    if (NULL == pucImg) {
        OPLK_LOG_PRINT_ERR("input null");
        goto ERR_LABEL;
    }

    /* Save the image information firstly                                       */
    m_pucCpldImg = pucImg;
    m_ulCpldImgLen = ulLen;
    m_ulCpldImgPos = 0;

    if (CPLD0_IDX == ucCpldIdx) {
        m_uiCpldIndex = CPLD1_IDX;
        if (0 != SDV_GetCpldValue(CPLD1_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
        ucValue |= 0x10;
        if (0 != SDV_SetCpldValue(CPLD1_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("set cpld value err.");
            goto ERR_LABEL;
        }
    } else if (CPLD1_IDX == ucCpldIdx) {
        m_uiCpldIndex = CPLD0_IDX;
        if (0 != SDV_GetCpldValue(CPLD0_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
        ucValue |= 0x10;
        if (0 != SDV_SetCpldValue(CPLD0_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("set cpld value err.");
            goto ERR_LABEL;
        }
    }

    iRet = ispVM();
    if (iRet != 0) {
        OPLK_LOG_PRINT_ERR("JTAG CPLD Error, cRet = %d\n\r", iRet);
        goto ERR_LABEL;
    }

    if (CPLD0_IDX == ucCpldIdx) {
        if (0 != SDV_GetCpldValue(CPLD1_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
        ucValue &= ~0x10;
        if (0 != SDV_SetCpldValue(CPLD1_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("set cpld value err.");
            goto ERR_LABEL;
        }
    } else if (CPLD1_IDX == ucCpldIdx) {
        if (0 != SDV_GetCpldValue(CPLD0_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("get cpld value err.");
            goto ERR_LABEL;
        }
        ucValue &= ~0x10;
        if (0 != SDV_SetCpldValue(CPLD0_JTAG_UPG, 1, &ucValue)) {
            OPLK_LOG_PRINT_ERR("set cpld value err.");
            goto ERR_LABEL;
        }
    }

    return 0;

ERR_LABEL:

    return -1;
}

INT
SDV_JtagCpldReset(UCHAR ucCpldIdx)
{
    UCHAR ucReloadCpld[] = {
        0x65, 0xde, 0x35, 0x5f, 0x5f, 0x5f, 0x5f, 0x31, 0x32, 0x2e, 0x31, 0xf1, 0x19, 0xc8, 0xf, 0x8,
        0x0, 0x6, 0x0, 0x9, 0x0, 0x7, 0x0, 0x2, 0x3, 0x3, 0x2, 0xb, 0xe8, 0x7, 0x10, 0x1,
        0x11, 0x8, 0x13, 0x0, 0x7, 0x70, 0x12, 0x20, 0x13, 0x1, 0x0, 0x4, 0x14, 0x0, 0xc2, 0xb,
        0xd4, 0x80, 0x15, 0x2, 0xff, 0x4, 0x70, 0x11, 0x8, 0x13, 0x0, 0x9e, 0x70, 0x10, 0x1, 0x1b,
        0x2, 0x1a, 0xe8, 0x87, 0x2, 0x11, 0x8, 0x13, 0x0, 0xff, 0x70, 0x10, 0x1, 0x1b, 0x64, 0x1a,
        0x88, 0xa7, 0x2, 0x7f
    };

    if (SDV_JtagCpld(ucCpldIdx, ucReloadCpld, sizeof(ucReloadCpld))) {
        OPLK_LOG_PRINT_ERR("JTAG CPLD Reset Fail!\n");
        return -1;
    }

    return 0;
}

