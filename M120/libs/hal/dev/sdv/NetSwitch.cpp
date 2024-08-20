#include "NetSwitch.h"

#define MV6390X_PHY_ADDR    0x1E

#define MV6390X_PORT0_ADDR  0x00
#define MV6390X_PORT1_ADDR  0x01
#define MV6390X_PORT2_ADDR  0x02
#define MV6390X_PORT3_ADDR  0x03
#define MV6390X_PORT4_ADDR  0x04
#define MV6390X_PORT5_ADDR  0x05
#define MV6390X_PORT6_ADDR  0x06
#define MV6390X_PORT7_ADDR  0x07
#define MV6390X_PORT8_ADDR  0x08
#define MV6390X_PORT9_ADDR  0x09
#define MV6390X_PORTA_ADDR  0x0A

#define MV6390X_GlOB1_ADDR  0x1B
#define MV6390X_GlOB2_ADDR  0x1C

#define MV6390X_INCPU_ADDR  0x1E
#define MV6390X_TCAM_ADDR   0x1F

#define MV6390X_SMI_CMD_ADDR    0x18
#define MV6390X_SMI_DATA_ADDR   0x19

#define MSD_REG_SMI_COMMAND     0x0
#define MSD_REG_SMI_DATA        0x1

/* Bit definition for MSD_REG_SMI_COMMAND */
#define MSD_SMI_BUSY                0x8000
#define MSD_SMI_MODE                0x1000
#define MSD_SMI_MODE_BIT            12
#define MSD_SMI_FUNC_BIT            13
#define MSD_SMI_FUNC_SIZE            2
#define MSD_SMI_OP_BIT              10
#define MSD_SMI_OP_SIZE              2
#define MSD_SMI_DEV_ADDR_BIT         5
#define MSD_SMI_DEV_ADDR_SIZE        5
#define MSD_SMI_REG_ADDR_BIT         0
#define MSD_SMI_REG_ADDR_SIZE        5

#define MSD_SMI_CLAUSE45            0
#define MSD_SMI_CLAUSE22            1

#define MSD_SMI_WRITE_ADDR          0x00
#define MSD_SMI_WRITE               0x01
#define MSD_SMI_READ_22             0x02
#define MSD_SMI_READ_45             0x03
#define MSD_SMI_READ_INC            0x02

#define MSD_SMI_ACCESS_LOOP        1000
#define MSD_SMI_TIMEOUT            2

#define MSD_PHY_MMD_ADDR          0x0000
#define MSD_PHY_MMD_DATA_NO_INC   0x4000
#define MSD_PHY_MMD_DATA_RW_INC   0x8000
#define MSD_PHY_MMD_DATA_WR_INC   0xc000

#define MSD_INVALID_PHY             0xFF
#define MSD_INVALID_PORT            0xFF
#define MSD_INVALID_PORT_VEC        0xFFFFFFFF

#define VALIDPORTVEC    0x1FE
#define BASEPHYREGADDR  0

#define NVM_MAX_RETRY_SPI           10 /* Max wait of 5ms, for RDY signal */
#define NVM_READ_OPCODE_SPI         0x03 /* NVM read opcode */
#define NVM_WRITE_OPCODE_SPI        0x02 /* NVM write opcode */
#define NVM_A8_OPCODE_SPI           0x08 /* opcode bit-3 = address bit-8 */
#define NVM_WREN_OPCODE_SPI         0x06 /* NVM set Write Enable latch */
#define NVM_RDSR_OPCODE_SPI         0x05 /* NVM read Status register */


bool
CNetSwitch::ReadReg(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusValue)
{
    uint16_t usMiReg = 0;
    volatile uint32_t vulTimeOut; /* in 100MS units */
    int iErrno = 0;

    if (nullptr == m_stpDrvPtr || nullptr == pusValue) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr.");
        return false;
    }

    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!m_stpDrvPtr->Read(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, &usMiReg)) {
            iErrno = 1;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulPhyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_22 << MSD_SMI_OP_BIT) |
              ((uint8_t)ulRegAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);
    if (!m_stpDrvPtr->Write(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, usMiReg)) {
        iErrno = 3;
        goto ERR_LABEL;
    }

    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!m_stpDrvPtr->Read(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, &usMiReg)) {
            iErrno = 4;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 5;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);

    if (!m_stpDrvPtr->Read(MV6390X_PHY_ADDR, MSD_REG_SMI_DATA, &usMiReg)) {
        iErrno = 6;
        goto ERR_LABEL;
    }

    *pusValue = usMiReg;

    /* LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::ReadReg phy %x, reg %x, val %hx", ulPhyAddr, ulRegAddr, usMiReg); */
    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::ReadReg ** %d", iErrno);
    return false;
}

bool
CNetSwitch::WriteReg(uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usValue)
{
    uint16_t usMiReg = 0;
    volatile uint32_t vulTimeOut; /* in 100MS units */
    int iErrno = 0;

    if (nullptr == m_stpDrvPtr) {
        return false;
    }

    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!m_stpDrvPtr->Read(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, &usMiReg)) {
            iErrno = 1;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);

    if (!m_stpDrvPtr->Write(MV6390X_PHY_ADDR, MSD_REG_SMI_DATA, usValue)) {
        iErrno = 3;
        goto ERR_LABEL;
    }

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulPhyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) |
              ((uint8_t)ulRegAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);
    if (!m_stpDrvPtr->Write(MV6390X_PHY_ADDR, MSD_REG_SMI_COMMAND, usMiReg)) {
        iErrno = 4;
        goto ERR_LABEL;
    }

    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_DEBUG, "CNetSwitch::WriteReg ** %d", iErrno);
    return false;
}
bool
CNetSwitch::ReadSMIC45PhyReg(uint32_t ulDevAddr, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t *pusValue)
{
    volatile uint32_t vulTimeOut;
    uint16_t usMiReg = 0;
    uint16_t usTmp = 0;
    int iErrno = 0;

    /* first check that it is not busy */
    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 1;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);


    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, ulRegAddr)) {
        iErrno = 3;
        goto ERR_LABEL;
    }

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulPhyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE_ADDR << MSD_SMI_OP_BIT) |
              ((uint8_t)ulDevAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);
    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, usMiReg)) {
        iErrno = 4;
        goto ERR_LABEL;
    }

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulPhyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_45 << MSD_SMI_OP_BIT) |
              ((uint8_t)ulDevAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);
    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, usMiReg)) {
        iErrno = 5;
        goto ERR_LABEL;
    }

    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 6;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 7;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);


    if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, &usTmp)) {
        iErrno = 8;
        goto ERR_LABEL;
    }

    *pusValue = usTmp;

    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::ReadSMIC45PhyReg ** %d", iErrno);
    return false;
}
bool
CNetSwitch::WriteSMIC45PhyReg(uint32_t ulDevAddr, uint32_t ulPhyAddr, uint32_t ulRegAddr, uint16_t usValue)
{
    volatile uint32_t vulTimeOut;
    uint16_t usMiReg = 0;
    int iErrno = 0;

    /* first check that it is not busy */
    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 1;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);

    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, (uint16_t)ulRegAddr)) {
        iErrno = 3;
        goto ERR_LABEL;
    }

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulPhyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE_ADDR << MSD_SMI_OP_BIT) |
              ((uint8_t)ulDevAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);
    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, usMiReg)) {
        iErrno = 4;
        goto ERR_LABEL;
    }

    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 5;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 6;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);


    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, usValue)) {
        iErrno = 7;
        goto ERR_LABEL;
    }
    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulPhyAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) |
              ((uint8_t)ulDevAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE45 << MSD_SMI_MODE_BIT);

    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, usMiReg)) {
        iErrno = 8;
        goto ERR_LABEL;
    }

    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::WriteSMIC45PhyReg ** %d", iErrno);
    return false;
}

bool
CNetSwitch::ReadSMIC22PhyReg(uint32_t ulDevAddr, uint32_t ulRegAddr, uint16_t *pusValue)
{
    volatile uint32_t vulTimeOut;
    uint16_t usMiReg = 0;
    int iErrno = 0;

    /* first check that it is not busy */
    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 1;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulDevAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_READ_22 << MSD_SMI_OP_BIT) |
              ((uint8_t)ulRegAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);

    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, usMiReg)) {
        iErrno = 3;
        goto ERR_LABEL;
    }

    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 4;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 5;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);

    if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, &usMiReg)) {
        iErrno = 6;
        goto ERR_LABEL;
    }
    *pusValue = usMiReg;

    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::ReadSMIC22PhyReg ** %d", iErrno);
    return false;
}
bool
CNetSwitch::WriteSMIC22PhyReg(uint32_t ulDevAddr, uint32_t ulRegAddr, uint16_t usValue)
{
    volatile uint32_t vulTimeOut;
    uint16_t usMiReg = 0;
    int iErrno = 0;

    /* first check that it is not busy */
    vulTimeOut = MSD_SMI_ACCESS_LOOP; /* initialize the loop count */

    do {
        if (!ReadReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, &usMiReg)) {
            iErrno = 1;
            goto ERR_LABEL;
        }
        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usMiReg & MSD_SMI_BUSY);


    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_DATA_ADDR, usValue)) {
        iErrno = 3;
        goto ERR_LABEL;
    }

    usMiReg = MSD_SMI_BUSY | ((uint8_t)ulDevAddr << MSD_SMI_DEV_ADDR_BIT) | (MSD_SMI_WRITE << MSD_SMI_OP_BIT) |
              ((uint8_t)ulRegAddr << MSD_SMI_REG_ADDR_BIT) | (MSD_SMI_CLAUSE22 << MSD_SMI_MODE_BIT);
    if (!WriteReg(MV6390X_GlOB2_ADDR, MV6390X_SMI_CMD_ADDR, usMiReg)) {
        iErrno = 4;
        goto ERR_LABEL;
    }

    return true;
ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::WriteSMIC22PhyReg ** %d", iErrno);
    return false;
}

bool
CNetSwitch::WriteSMIC22PhyPage(uint32_t ulPortNum, uint32_t ulPageNum, uint32_t ulRegAddr, uint16_t usValue)
{
    if (!WriteSMIC22PhyReg(ulPortNum, 22, ulPageNum)) {
        return false;
    }
    return WriteSMIC22PhyReg(ulPortNum, ulRegAddr, usValue);
}

bool
CNetSwitch::ReadSMIC22PhyPage(uint32_t ulPortNum, uint32_t ulPageNum, uint32_t ulRegAddr, uint16_t *pusValue)
{
    if (!WriteSMIC22PhyReg(ulPortNum, 22, ulPageNum)) {
        return false;
    }
    return ReadSMIC22PhyReg(ulPortNum, ulRegAddr, pusValue);
}
#if 0
bool
CNetSwitch::CfgNoCpuMode()
{
    /* set port9 lan0/1/2(port9/2/3) sedres power normal */
    WriteSMIC45PhyReg(0x04, 0x12, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x13, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x09, 0x2000, 0x9140);

    /* set port10 lan1/2 (sfp port5/6) sedres power normal */
    WriteSMIC45PhyReg(0x04, 0x0a, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x15, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x16, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x17, 0x2000, 0x9140);

    /* set port1/4/8 phy power normal */
    WriteSMIC22PhyPage(0x01, 0x00, 0x00, 0x9140);
    WriteSMIC22PhyPage(0x04, 0x00, 0x00, 0x9140);
    WriteSMIC22PhyPage(0x08, 0x00, 0x00, 0x9140);

    /* set port0-10 forwarding */
    WriteReg(0, 4, 0x007f);
    WriteReg(1, 4, 0x007f);
    WriteReg(2, 4, 0x007f);
    WriteReg(3, 4, 0x007f);
    WriteReg(4, 4, 0x007f);
    WriteReg(5, 4, 0x007f);
    WriteReg(6, 4, 0x007f);
    WriteReg(7, 4, 0x007f);
    WriteReg(8, 4, 0x007f);
    WriteReg(9, 4, 0x007f);
    WriteReg(10, 4, 0x007f);

    return true;
}

bool
CNetSwitch::Init()
{
    uint16_t usDataBuf = 0;
    int iErrno = 0;
    volatile uint32_t vulTimeOut = 0; /* in 100MS units */

    LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::Init ++");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr");
        return false;
    }

    /* TBD: reset ? */
    if (!Reset()) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error.");
        return false;
    }

    /* Port 1 : DCN */
    /* Port 4 : DS1 */
    /* Port 8 : DS2 */
    /* Port 9 : CPU */
    /* Port 10 : SFP lane1=port5; lane2=port6 */

    /* config no cpu mode */
    CfgNoCpuMode();

    /*********************/
    WriteSMIC45PhyReg(0x04, 0x17, 0x2000, 0x9140);
    ReadReg(MV6390X_PORT7_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORT7_ADDR, 0, usDataBuf);

    WriteSMIC45PhyReg(0x04, 0x17, 0xa000, 0x2001);

    /*********************/

    /* set port9/10 C_MODE 1000base-x */
    ReadReg(MV6390X_PORT9_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORT9_ADDR, 0, usDataBuf);

    ReadReg(MV6390X_PORTA_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORTA_ADDR, 0, usDataBuf);

    /* Port10 lane1(5)/2(6) 1000base-x nosie */
    WriteSMIC45PhyReg(0x04, 0x15, 0xa000, 0x2001);
    WriteSMIC45PhyReg(0x04, 0x16, 0xa000, 0x2001);

    /* port1/4/8 led init for hardware issue */
    WriteReg(1, 0x16, 0x8058);
    WriteReg(4, 0x16, 0x8058);
    WriteReg(8, 0x16, 0x8058);

    /* Port10 lane1(5)/2(6) led Link */
    WriteReg(5, 0x16, 0x8058);
    WriteReg(6, 0x16, 0x8058);

    /* set port isolation, each front port can olny forward to CPU */
    WriteReg(1, 6, 1);     /* DCN P1*/
    WriteReg(4, 6, 1);     /* DS1 P4*/
    WriteReg(8, 6, 1);     /* DS2 P8*/

    WriteReg(5, 6, 1);     /* OSC P10-1(5)*/
    WriteReg(6, 6, 1);     /* OSC P10-2(6)*/
    WriteReg(7, 6, 1);

    /* disable lldp for port 1/4/8/Port10-2(6) */
    WriteReg(1, 0xf, 0x88cc);
    WriteReg(4, 0xf, 0x88cc);
    WriteReg(8, 0xf, 0x88cc);
    WriteReg(6, 0xf, 0x88cc);

    WriteReg(1, 0xe, 0x300);
    WriteReg(4, 0xe, 0x300);
    WriteReg(8, 0xe, 0x300);
    WriteReg(6, 0xe, 0x300);

    /* set port 0 and Port10-7 to support lldp */
    usDataBuf = 0;
    vulTimeOut = MSD_SMI_ACCESS_LOOP;             /* initialize the loop count */

    do {
        if (!ReadReg(0x1B, 0xB, &usDataBuf)) {
            iErrno = 1;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x1, 0);
    /* WriteReg(0x1B, 0xC, 0x0417); */ /* for port 5 */
    WriteReg(0x1B, 0xC, 0x0817);
    WriteReg(0x1B, 0xD, 0x0180);
    WriteReg(0x1B, 0xE, 0xC200);
    WriteReg(0x1B, 0xF, 0x000E);
    WriteReg(0x1B, 0xB, 0xB000);

    usDataBuf = 0;
    vulTimeOut = MSD_SMI_ACCESS_LOOP;             /* initialize the loop count */

    do {
        if (!ReadReg(0x1B, 0xB, &usDataBuf)) {
            iErrno = 3;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 4;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x1, 0);
    /* WriteReg(0x1B, 0xC, 0x0417); */ /* for port 5 */
    WriteReg(0x1B, 0xC, 0x0817);
    WriteReg(0x1B, 0xD, 0x0180);
    WriteReg(0x1B, 0xE, 0xC200);
    WriteReg(0x1B, 0xF, 0x0000);
    WriteReg(0x1B, 0xB, 0xB000);

    usDataBuf = 0;
    vulTimeOut = MSD_SMI_ACCESS_LOOP;             /* initialize the loop count */

    do {
        if (!ReadReg(0x1B, 0xB, &usDataBuf)) {
            iErrno = 5;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 6;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x1, 0);
    /* WriteReg(0x1B, 0xC, 0x0417); */ /* for port 5 */
    WriteReg(0x1B, 0xC, 0x0817);
    WriteReg(0x1B, 0xD, 0x0180);
    WriteReg(0x1B, 0xE, 0xC200);
    WriteReg(0x1B, 0xF, 0x0003);
    WriteReg(0x1B, 0xB, 0xB000);

    /* add eth0.1 VLAN ID 0x801 for port 7 */
    do {
        if (!ReadReg(0x1B, 0x5, &usDataBuf)) {
            iErrno = 7;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 8;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x2, 0);
    WriteReg(0x1B, 0x3, 0);
    WriteReg(0x1B, 0x6, 0x1801);
    WriteReg(0x1B, 0x7, 0x3FFC);
    WriteReg(0x1B, 0x8, 0x03FF);
    WriteReg(0x1B, 0x5, 0xB000);

    WriteReg(5, 0x8, 0x2480);

    /* 6390 port10 lane3 SGMII */
    WriteSMIC45PhyReg(0x04, 0x17, 0xF07C, 0x1FAC);


    LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::Init --");

    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::Init -- %d", iErrno);
    return false;
}
#endif

bool
CNetSwitch::Reset()
{
    uint8_t ucBit;

#ifdef __400GZR__

#define RESET_REG 0x48

    ucBit = BIT_1;

#else

#define RESET_REG 0x38

    ucBit = BIT_1;

#endif

    if (nullptr == m_stpLogicDrv) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpLogicDrv is nullptr.");
        return false;
    }

    m_stpLogicDrv->WriteBit(RESET_REG, ucBit, 0);

    /*CDelay::Delay(1, S); */
    CDelay::Delay(30, MS);  /* >10ms */

    m_stpLogicDrv->WriteBit(RESET_REG, ucBit, 1);

    return true;
}
#if 0
/* it for lband ,comment it temporary,reserver it for reference*/
bool
CNetSwitch::Init()
{
    uint16_t usDataBuf = 0;

    int iErrno = 0;
    volatile uint32_t vulTimeOut = 0; /* in 100MS units */

    LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::Init ++");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr");
        return false;
    }

    /* TBD: reset ? */
    if (!Reset()) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error.");
        return false;
    }

    /* config no cpu mode */
    CfgNoCpuMode();
    ReadReg(MV6390X_PORT7_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORT7_ADDR, 0, usDataBuf);

    WriteSMIC45PhyReg(0x04, 0x17, 0xa000, 0x2001);

    /*********************/

    /* set port9/10 C_MODE 1000base-x */
    ReadReg(MV6390X_PORT9_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORT9_ADDR, 0, usDataBuf);

    ReadReg(MV6390X_PORTA_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORTA_ADDR, 0, usDataBuf);

    /* Port10 lane1(5)/2(6) 1000base-x nosie */
    WriteSMIC45PhyReg(0x04, 0x15, 0xa000, 0x2001);
    WriteSMIC45PhyReg(0x04, 0x16, 0xa000, 0x2001);

    /* port1/4/8 led init for hardware issue */
    WriteReg(1, 0x16, 0x8058);
    WriteReg(4, 0x16, 0x8058);
    WriteReg(8, 0x16, 0x8058);

    /* Port10 lane1(5)/2(6) led Link */
    WriteReg(5, 0x16, 0x8058);
    WriteReg(6, 0x16, 0x8058);


    /*eth0 Port 0 <---> 5 10   : DCN */
    /*eth1 Port 4 <---> 3      : LLDP CV */
    /*eth2 Port 8 <---> 2  7   : OSC */
    /*eth3 Port 1 <---> 9      : peer SCC */
#if 1
    /* set port isolation, each front port can olny forward to CPU */
    WriteReg(5, 6, 1);        /* DCN P5 -> P0*/
    WriteReg(10, 6, 1);       /* DCN P10 ->P0*/

    WriteReg(3, 6, 0x10);     /* LLDP P9-2(3) -> P4*/
    WriteReg(4, 6, 0x8);      /* LLDP P4 ->P9-2(3)*/

    WriteReg(2, 6, 0x100);    /* OSC P9-1(2)  -> P8*/
    WriteReg(7, 6, 0x100);    /* OSC P10-3(7) -> P8*/

    WriteReg(1, 6, 0x200);     /* peerSCC P1 -> P9*/
    WriteReg(9, 6, 0x2);      /* peerSCC P9 -> P1*/

#if 0

    /* disable lldp for port 0/1/5/9/10 */  /* if  need here ??? */
    WriteReg(0, 0xf, 0x88cc);
    WriteReg(1, 0xf, 0x88cc);
    WriteReg(5, 0xf, 0x88cc);
    WriteReg(9, 0xf, 0x88cc);
    WriteReg(10, 0xf, 0x88cc);

    WriteReg(0, 0xe, 0x300);
    WriteReg(1, 0xe, 0x300);
    WriteReg(5, 0xe, 0x300);
    WriteReg(9, 0xe, 0x300);
    WriteReg(10, 0xe, 0x300);

    /* set port 4/8/ 2/3/6/7 to support lldp */   /* if  need here ??? */
    usDataBuf = 0;
    vulTimeOut = MSD_SMI_ACCESS_LOOP;             /* initialize the loop count */

    do {
        if (!ReadReg(0x1B, 0xB, &usDataBuf)) {
            iErrno = 1;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 2;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x1, 0);
    WriteReg(0x1B, 0xC, 0x1DC7);  /* 0817??????????????????????   0 001   1101   1100  0111 */
    WriteReg(0x1B, 0xD, 0x0180);
    WriteReg(0x1B, 0xE, 0xC200);
    WriteReg(0x1B, 0xF, 0x000E);
    WriteReg(0x1B, 0xB, 0xB000);

    usDataBuf = 0;
    vulTimeOut = MSD_SMI_ACCESS_LOOP;             /* initialize the loop count */

    do {
        if (!ReadReg(0x1B, 0xB, &usDataBuf)) {
            iErrno = 3;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 4;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x1, 0);
    WriteReg(0x1B, 0xC, 0x1DC7);   /*?????????????????????? */
    WriteReg(0x1B, 0xD, 0x0180);
    WriteReg(0x1B, 0xE, 0xC200);
    WriteReg(0x1B, 0xF, 0x0000);
    WriteReg(0x1B, 0xB, 0xB000);

    usDataBuf = 0;
    vulTimeOut = MSD_SMI_ACCESS_LOOP;             /* initialize the loop count */

    do {
        if (!ReadReg(0x1B, 0xB, &usDataBuf)) {
            iErrno = 5;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 6;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x1, 0);
    WriteReg(0x1B, 0xC, 0x1DC7);      /*?????????????????????? */
    WriteReg(0x1B, 0xD, 0x0180);
    WriteReg(0x1B, 0xE, 0xC200);
    WriteReg(0x1B, 0xF, 0x0003);
    WriteReg(0x1B, 0xB, 0xB000);
#endif


    /*  example:
       1.Set the default VLAN ID of P1 and P4.
       6390 write 0x1 0x7 0x10
       6390 write 0x4 0x7 0x20

       2.Set egress modes of P1 and P4 . Use normal network mode and remove the tag from any tagged frame.
       6390 write 0x1 0x4 0x107f
       6390 write 0x4 0x4 0x107f

       3.Set P9�s attributes. Provider mode & fallback mode & special Ether Type.
       6390 write 0x9 0x8 0x2480
       6390 write 0x9 0x4 0x27f
       6390 write 0x9 0xf 0x8100

       4. Add P1 and P9 to VLAN 0x10.
       6390 read 0x1b 0x5
       6390 write 0x1b 0x6 0x1010
       6390 write 0x1b 0x7 0xfff7
       6390 write 0x1b 0x8 0x3b
       6390 write 0x1b 0x5 0xb000

       5. Add P4 and P9 to VLAN 0x20.
       6390 read 0x1b 0x5
       6390 write 0x1b 0x6 0x1020
       6390 write 0x1b 0x7 0xfdff
       6390 write 0x1b 0x8 0x3b
       6390 write 0x1b 0x5 0xb000
     */
    /*  seem not necessary ?????*/
#if 1
    WriteReg(5, 0x7, 0x802);
    WriteReg(10, 0x7, 0x801);

    WriteReg(5, 0x4, 0x107f);
    WriteReg(10, 0x4, 0x107f);

    WriteReg(0, 0x8, 0x2480);
    WriteReg(0, 0x4, 0x27f);
    WriteReg(0, 0xf, 0x8100);

    /* add eth0.1 VLAN ID 0x801 for port  P0 an P5 */
    do {
        if (!ReadReg(0x1B, 0x5, &usDataBuf)) {
            iErrno = 7;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 8;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x2, 0);
    WriteReg(0x1B, 0x3, 0);
    WriteReg(0x1B, 0x6, 0x1801);
    WriteReg(0x1B, 0x7, 0xF3FC);  /*should 0xF7FD , P4 untag01 P0 tag10, 00 port is a member of this vlan& frame egress  undified  / 01 untage /02 tagged / 03 port is not a member of this vlan */
    WriteReg(0x1B, 0x8, 0x03FF);  /*bit8~15 need set ??? */
    WriteReg(0x1B, 0x5, 0xB000);

    WriteReg(0, 0x8, 0x2480);  /*bit 11:10  802.1Q mode  0 disabled 1 fallback   2 check 3 secure */
    WriteReg(5, 0x8, 0x2480);

    /* add eth0.2 VLAN ID 0x802 for port  P0 an P10 */
    do {
        if (!ReadReg(0x1B, 0x5, &usDataBuf)) {
            iErrno = 7;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 8;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x2, 0);
    WriteReg(0x1B, 0x3, 0);
    WriteReg(0x1B, 0x6, 0x1802);
    WriteReg(0x1B, 0x7, 0xFFFC);  /*should 0xFFFE P0 tag */
    WriteReg(0x1B, 0x8, 0x03CF);  /*should 0x03DF P10 untag ,bit8~15 need set ??? */
    WriteReg(0x1B, 0x5, 0xB000);

    WriteReg(0, 0x8, 0x2480);
    WriteReg(10, 0x8, 0x2480);
#endif

#if 1
    /*  seem not necessary ?????*/
    WriteReg(2, 0x7, 0x201);
    WriteReg(7, 0x7, 0x202);

    WriteReg(2, 0x4, 0x107f);
    WriteReg(7, 0x4, 0x107f);

    WriteReg(8, 0x8, 0x2480);
    WriteReg(8, 0x4, 0x27f);
    WriteReg(8, 0xf, 0x8100);
    /* add eth2.1 VLAN ID 0x201 for port  P8 an P2 */
    do {
        if (!ReadReg(0x1B, 0x5, &usDataBuf)) {
            iErrno = 7;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 8;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x2, 0);
    WriteReg(0x1B, 0x3, 0);
    WriteReg(0x1B, 0x6, 0x1201);
    WriteReg(0x1B, 0x7, 0xFFCF);  /*should 0xFFDF P2 untag 01 */
    WriteReg(0x1B, 0x8, 0x03FC);  /*should 0x03FE P8 tag 10 , bit8~15 need set ??? */
    WriteReg(0x1B, 0x5, 0xB000);

    WriteReg(8, 0x8, 0x2480);
    WriteReg(2, 0x8, 0x2480);

    /* add eth2.2 VLAN ID 0x202 for port  P8 an P7 */
    do {
        if (!ReadReg(0x1B, 0x5, &usDataBuf)) {
            iErrno = 7;
            goto ERR_LABEL;
        }

        if (vulTimeOut-- < 1 ) {
            iErrno = 8;
            goto ERR_LABEL;
        }
    } while (usDataBuf & 0x8000);

    WriteReg(0x1B, 0x2, 0);
    WriteReg(0x1B, 0x3, 0);
    WriteReg(0x1B, 0x6, 0x1202);
    WriteReg(0x1B, 0x7, 0x3FFF);  /*should 0x7FFF P7 untag 01 */
    WriteReg(0x1B, 0x8, 0x03FC);  /*should 0x03FE P8 tat 10, bit8~15 need set ??? */
    WriteReg(0x1B, 0x5, 0xB000);

    WriteReg(8, 0x8, 0x2480);
    WriteReg(7, 0x8, 0x2480);
#endif

    /* 6390 port10 lane3 SGMII */
    /*WriteSMIC45PhyReg(0x04, 0x17, 0xF07C, 0x1FAC);   // ?????????????????? */
#endif
    LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::Init --");

    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::Init -- %d", iErrno);
    return false;
}
#endif

bool
CNetSwitch::Init88e1512(uint32_t ulIndex)
{

    uint16_t usDataBuf = 0;

    LOG_WRITE(EN_LOG_NOTICE, "++Init88e1512.");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr.");
        return false;
    }

    /* page 0 ,register 02 and 03 */
    m_stpDrvPtr->Write(0, 22, 0);

    m_stpDrvPtr->Read(0, 0x01, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "usDataBuf:0x%x", usDataBuf);
    m_stpDrvPtr->Read(0, 0x02, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "usDataBuf:0x%x", usDataBuf);
    m_stpDrvPtr->Read(0, 0x03, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "usDataBuf:0x%x", usDataBuf);


    /* page 18 ,register 20 */
    m_stpDrvPtr->Write(0, 22, 18);
    m_stpDrvPtr->Read(0, 0x14, &usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "usDataBuf:0x%x", usDataBuf);

    usDataBuf &= 0xFFF8;
    m_stpDrvPtr->Write(0, 0x14, usDataBuf);
    LOG_WRITE(EN_LOG_NOTICE, "write register 0x14 usDataBuf:0x%x", usDataBuf);

    LOG_WRITE(EN_LOG_NOTICE, "--Init88e1512.");

    return true;
}

bool
CNetSwitch::CfgNoCpuMode()
{
    /* set port9 lan0/1/2(port9/2/3) sedres power normal */
    WriteSMIC45PhyReg(0x04, 0x12, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x13, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x09, 0x2000, 0x9140);

    /* set port10 lan1/2 (sfp port5/6) sedres power normal */
    WriteSMIC45PhyReg(0x04, 0x0a, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x15, 0x2000, 0x9140);
    WriteSMIC45PhyReg(0x04, 0x16, 0x2000, 0x9140);
    /* WriteSMIC45PhyReg(0x04, 0x17, 0x2000, 0x9140); */

    /* set port1/4/8 phy power normal */
    WriteSMIC22PhyPage(0x01, 0x00, 0x00, 0x9140);
    WriteSMIC22PhyPage(0x04, 0x00, 0x00, 0x9140);
    WriteSMIC22PhyPage(0x08, 0x00, 0x00, 0x9140);
    WriteSMIC22PhyPage(0x07, 0x00, 0x00, 0x9140);

    /* set port0-10 forwarding */
    WriteReg(0, 4, 0x007f);
    WriteReg(1, 4, 0x007f);
    WriteReg(2, 4, 0x007f);
    WriteReg(3, 4, 0x007f);
    WriteReg(4, 4, 0x007f);
    WriteReg(5, 4, 0x007f);
    WriteReg(6, 4, 0x007f);
    WriteReg(7, 4, 0x007f);
    WriteReg(8, 4, 0x007f);
    WriteReg(9, 4, 0x007f);
    WriteReg(10, 4, 0x007f);

    return true;
}

bool
CNetSwitch::Init()
{
    uint16_t usDataBuf = 0;

    int iErrno = 0;
    /* uint32_t vulTimeOut = 0; / * in 100MS units * / */

    LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::Init ++");

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr");
        goto ERR_LABEL;
    }

    /* TBD: reset ? */
    if (!Reset()) {
        LOG_WRITE(EN_LOG_ERROR, "Reset error.");
        goto ERR_LABEL;
    }

    /* config no cpu mode */
    CfgNoCpuMode();

#if 0
    ReadReg(MV6390X_PORT7_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORT7_ADDR, 0, usDataBuf);
    WriteSMIC45PhyReg(0x04, 0x17, 0xa000, 0x2001);
#endif

    /* set port9/10 C_MODE 1000base-x */
    ReadReg(MV6390X_PORT9_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x000a;
    WriteReg(MV6390X_PORT9_ADDR, 0, usDataBuf);

#if 0
    /* TBD,comment temporary. P10 except P10-3(7) not use in ols-p, and P10-3(7) use as a Phy with MDI interface */
    ReadReg(MV6390X_PORTA_ADDR, 0, &usDataBuf);
    usDataBuf &= 0xfff0;
    usDataBuf |= 0x0009;
    WriteReg(MV6390X_PORTA_ADDR, 0, usDataBuf);
#endif

    /* Port10 lane1(2)/2(3) 1000base-x nosie */
    WriteSMIC45PhyReg(0x04, 0x12, 0xa000, 0x2001);
    WriteSMIC45PhyReg(0x04, 0x13, 0xa000, 0x2001);

    /* port1/4/8/7 led init for hardware issue */
    WriteReg(1, 0x16, 0x8058);
    WriteReg(4, 0x16, 0x8058);
    WriteReg(8, 0x16, 0x8058);
    WriteReg(7, 0x16, 0x8058);

    /* Port9 lane1(2)/2(3) led Link */
    WriteReg(2, 0x16, 0x8058);
    WriteReg(3, 0x16, 0x8058);
    WriteReg(7, 0x16, 0x8058);
    /*eth0 <--->i350 P0  <---> 6390 P0  P7 <---> DCN */
    /*eth1 <--->i350 P1  <---> 6390 P4  P3 <---> SLC0 OSC*/
    /*eth2 <--->i350 P2  <---> 6390 P8  P2 <---> SLC1 OSC*/

    /* set port isolation,  front DCN port can olny forward to CPU */
    WriteReg(0, 6, 0x80);     /* DCN P0 -> P7 */
    WriteReg(7, 6, 0x01);     /* DCN P7 -> P0 */

    WriteReg(4, 6, 0x08);     /* OSC P4  -> P9-2(3) */
    WriteReg(3, 6, 0x10);     /* OSC P9-2(3) -> P4  */

    WriteReg(8, 6, 0x04);    /* OSC P8 -> P9-1(2) */
    WriteReg(2, 6, 0x100);   /* OSC P9-1(2) -> P8 */
    LOG_WRITE(EN_LOG_NOTICE, "CNetSwitch::Init --");

    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CNetSwitch::Init -- %d", iErrno);
    return false;
}




