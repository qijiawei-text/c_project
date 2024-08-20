#include "Flash.h"


#define CHIP_PAGE_BYTES                 (0x100)
#define CHIP_PAGE_NUMS                  (0x10000)
#define CHIP_TOTAL_BYTES                (CHIP_PAGE_BYTES * CHIP_PAGE_NUMS)

#define CMD_WRITE_ENABLE                (0x06)
#define CMD_WRITE_DISABLE               (0x04)

#define CMD_RDID                        (0x9F)

#define CMD_WRITE                       (0x02)
#define CMD_READ                        (0x03)

#define CMD_STATUS_READ                 (0x05)
#define CMD_STATUS_WRITE                (0x01)

#define CMD_ERASE_4K                    (0x20)
#define CMD_ERASE_64K                   (0xD8)

#define CHIP_BLOCK4K_LEN                (4 * 1024)
#define CHIP_BLOCK64K_LEN               (64 * 1024)

#define PROTECT_TRY_TIMES               (1000)
#define PAGE_WRITE_TRY_TIMES            (10000)
#define ERASE4K_TRY_TIMES               (5000)
#define ERASE64K_TRY_TIMES              (10000)

enum FPGA_SPI_FLASH_ID
{
    FPGA_SPI_FLASH_ID_N25Q128 = 0,                                          /* N25Q128                                                 */
    FPGA_SPI_FLASH_ID_S25FL129P,                                            /* S25FL129P                                               */

    FPGA_SPI_FLASH_ID_MAX

};
enum FPGA_SPI_LOAD_BIT
{
    FPGA_SPI_LOAD_BIT_1 = 1,
    FPGA_SPI_LOAD_BIT_2,
    FPGA_SPI_LOAD_BIT_4 = 4,

    FPGA_SPI_LOAD_BIT_MAX

};

#define FPGA1_ADDR                      (17)
#define FPGA2_ADDR                      (17)

#define FPGA1_ADDR_IN_FLASH             (0x010000)                          /* 64KB                                                    */
#define FPGA2_ADDR_IN_FLASH             (0x800000)                          /* 8MB                                                     */

#define FPGA_IMG_HEAD_LEN               (0x002C)

static const uint8_t m_aucFpgaHead[] =
{0xFF, 0xFF, 0xFF, 0xFF,        /*DUMMYWORD */
 0xAA, 0x99, 0x55, 0x66,        /*SYNCWORD */
 0x20, 0x00, 0x00, 0x00,        /*Type 1 NO OP */
 0x30, 0x02, 0x00, 0x01,        /*Write WBSTAR cmd */
 0x00, 0x01, 0x00, 0x00,        /* Addr in SPI Flash of MultiBoot image address */
 0x30, 0x00, 0x80, 0x01,        /*Write CMD */
 0x00, 0x00, 0x00, 0x0F,        /*Write IPROG */
 0x20, 0x00, 0x00, 0x00,        /*Type 1 NO OP */
 0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF};

bool
CFlash::WriteEnable()
{
    uint8_t aucBufTx[32] = {0};
    uint8_t aucBufRx[32] = {0};
    uint8_t ucCmd = CMD_WRITE_ENABLE;
    int32_t i = 0;


    for (i = 0; i < 3; i++) {

        if (!m_stpDrvPtr->ReadWrite(m_ulChip, &ucCmd, 1, aucBufRx, std::make_shared<CRspHandler>(1))) {
            goto ERR_LABEL;
        }

        CDelay::Delay(1, MS);

        aucBufTx[0] = CMD_STATUS_READ;
        if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 1, aucBufRx, std::make_shared<CRspHandler>(2))) {
            goto ERR_LABEL;
        }

        if (0x02 == (aucBufRx[1] & 0x03)) {
            break;
        }
    }

    if (3 == i) {
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;

}
bool
CFlash::CheckStatus()
{
    uint8_t aucBufTx[32] = {0};
    uint8_t aucBufRx[32] = {0};


    aucBufTx[0] = CMD_STATUS_READ;

    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 1, aucBufRx, std::make_shared<CRspHandler>(2))) {
        goto ERR_LABEL;
    }

    if (aucBufRx[1] & 0x01) {
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::CheckTime(uint32_t ulTimes)
{
    uint32_t ulTryTimes = 0;

    while (ulTryTimes < ulTimes) {
        if (CheckStatus()) {
            return true;
        }

        CDelay::Delay(1, MS);
        ulTryTimes++;
    }

    return false;
}

int32_t
CFlash::GetChipId()
{
    uint8_t aucBufTx[CHIP_PAGE_BYTES] = {0};
    uint8_t aucBufRx[CHIP_PAGE_BYTES] = {0};
    int32_t lChipId = FPGA_SPI_FLASH_ID_N25Q128;

    if (nullptr == m_stpDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "m_stpDrvPtr is nullptr");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "m_ulChip = %d", m_ulChip);

    /* Read the spi flash chip id                                               */
    aucBufTx[0] = CMD_RDID;
    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 1, aucBufRx, std::make_shared<CRspHandler>(4))) {
        LOG_WRITE(EN_LOG_ERROR, "spi read chip%d id error", m_ulChip);
        goto ERR_LABEL;
    }


    LOG_WRITE(EN_LOG_NOTICE, "m_ulChip = %d, aucBufRx: 0x%x 0x%x 0x%x", m_ulChip, aucBufRx[1], aucBufRx[2], aucBufRx[3]);

    if ((0x01 == aucBufRx[1]) &&
        (0x20 == aucBufRx[2]) &&
        (0x18 == aucBufRx[3])) {
        lChipId = FPGA_SPI_FLASH_ID_S25FL129P;
    } else if ((0x20 == aucBufRx[1]) &&
               (0xBA == aucBufRx[2]) &&
               (0x18 == aucBufRx[3])) {
        lChipId = FPGA_SPI_FLASH_ID_N25Q128;
    } else {
        LOG_WRITE(EN_LOG_ERROR, "spi chip%d id error, no recognised flash:%02x%02x%02x", m_ulChip, aucBufRx[1], aucBufRx[2], aucBufRx[3]);
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "spi read chip%d id %d", m_ulChip, lChipId);

    return lChipId;

ERR_LABEL:
    return OPLK_ERR;
}
int32_t
CFlash::GetLoadBit(uint8_t *pucBuf)
{
    int32_t lLoadBit = FPGA_SPI_LOAD_BIT_1;
    uint32_t ulTagCnt = 0;
    int32_t i = 0;

    if (NULL == pucBuf) {
        goto ERR_LABEL;
    }

    for (i = 0; i < 1024; i++) {
        if (0xAA == pucBuf[i]) {
            if ((0x99 == pucBuf[i + 1]) &&
                (0x55 == pucBuf[i + 2]) &&
                (0x66 == pucBuf[i + 3])) {
                i += 4;
                ulTagCnt++;
            }
        }

        if (ulTagCnt > 1) {
            break;
        }
    }

    if (0 == ulTagCnt) {
        goto ERR_LABEL;
    } else if (1 == ulTagCnt) {
        lLoadBit = FPGA_SPI_LOAD_BIT_1;
    } else {
        lLoadBit = FPGA_SPI_LOAD_BIT_4;
    }

    return lLoadBit;

ERR_LABEL:
    return OPLK_ERR;
}
bool
CFlash::Protect(uint32_t ulBit)
{
    uint8_t aucBufTx[32] = {0};
    uint8_t aucBufRx[32] = {0};
    uint8_t ucLen = 0;

    if (!WriteEnable()) {
        LOG_WRITE(EN_LOG_ERROR, "write enable fail.");
        goto ERR_LABEL;
    }

    switch (m_ulId) {
    case FPGA_SPI_FLASH_ID_N25Q128:
        aucBufTx[0] = CMD_STATUS_WRITE;
        aucBufTx[1] = 0x00;
        aucBufTx[2] = 0x00;

        ucLen = 2;
        break;

    case FPGA_SPI_FLASH_ID_S25FL129P:
        aucBufTx[0] = CMD_STATUS_WRITE;
        aucBufTx[1] = 0x00;
        if (FPGA_SPI_LOAD_BIT_1 == ulBit) {
            aucBufTx[2] = 0x00;
        } else if (FPGA_SPI_LOAD_BIT_4 == ulBit) {
            aucBufTx[2] = 0x02;
        } else {
            goto ERR_LABEL;
        }

        ucLen = 3;
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "chip id error");
        goto ERR_LABEL;
    }

    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 3, aucBufRx, std::make_shared<CRspHandler>(ucLen))) {
        LOG_WRITE(EN_LOG_ERROR, "spi write and read interface error");
        goto ERR_LABEL;
    }

    if (!CheckTime(PROTECT_TRY_TIMES)) {
        LOG_WRITE(EN_LOG_ERROR, "time out");
        goto ERR_LABEL;
    }
    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::UnProtect()
{
    uint8_t aucBufTx[32] = {0};
    uint8_t aucBufRx[32] = {0};
    uint8_t ucLen = 0;


    if (!WriteEnable()) {
        LOG_WRITE(EN_LOG_ERROR, "write enable fail.");
        goto ERR_LABEL;
    }

    aucBufTx[0] = CMD_STATUS_WRITE;
    aucBufTx[1] = 0x00;
    aucBufTx[2] = 0x00;

    switch (m_ulId) {
    case FPGA_SPI_FLASH_ID_N25Q128:
        ucLen = 2;
        break;

    case FPGA_SPI_FLASH_ID_S25FL129P:
        ucLen = 3;
        break;

    default:
        LOG_WRITE(EN_LOG_ERROR, "chip id error");
        goto ERR_LABEL;
    }

    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 3, aucBufRx, std::make_shared<CRspHandler>(ucLen))) {
        LOG_WRITE(EN_LOG_ERROR, "SPI Write error.");
        goto ERR_LABEL;
    }

    if (!CheckTime(PROTECT_TRY_TIMES)) {
        LOG_WRITE(EN_LOG_ERROR, "time out.");
        goto ERR_LABEL;
    }
    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::EraseBy64K(uint32_t ulSectorAddr)
{
    uint8_t aucBufTx[32] = {0};
    uint8_t aucBufRx[32] = {0};


    if (!WriteEnable()) {
        LOG_WRITE(EN_LOG_ERROR, "write enable fail.");
        goto ERR_LABEL;
    }

    aucBufTx[0] = CMD_ERASE_64K;
    aucBufTx[1] = ulSectorAddr >> 16;
    aucBufTx[2] = ulSectorAddr >> 8;
    aucBufTx[3] = ulSectorAddr;

    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 4, aucBufRx, std::make_shared<CRspHandler>(4))) {
        LOG_WRITE(EN_LOG_ERROR, "spi write and read interface error");
        goto ERR_LABEL;
    }

    if (!CheckTime(PROTECT_TRY_TIMES)) {
        LOG_WRITE(EN_LOG_ERROR, "time out.");
        goto ERR_LABEL;
    }

    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::WriteOnePage(uint32_t ulPageAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    uint8_t aucBufTx[512] = {0};
    uint8_t aucBufRx[512] = {0};


    if ((ulPageAddr > CHIP_TOTAL_BYTES) ||
        (ulLen > CHIP_PAGE_BYTES) ||
        (NULL == pucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "input parameter wrong!");
        goto ERR_LABEL;
    }

    if (!WriteEnable()) {
        LOG_WRITE(EN_LOG_ERROR, "write enable fail.");
        goto ERR_LABEL;
    }

    aucBufTx[0] = CMD_WRITE;
    aucBufTx[1] = ulPageAddr >> 16;
    aucBufTx[2] = ulPageAddr >> 8;
    aucBufTx[3] = ulPageAddr;
    memcpy(aucBufTx + 4, pucBuf, ulLen);

    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 4 + ulLen, aucBufRx, std::make_shared<CRspHandler>(4 + ulLen))) {
        LOG_WRITE(EN_LOG_ERROR, "spi write and read interface error");
        goto ERR_LABEL;
    }

    if (!CheckTime(PROTECT_TRY_TIMES)) {
        LOG_WRITE(EN_LOG_ERROR, "time out.");
        goto ERR_LABEL;
    }
    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::ReadOnePage(uint32_t ulPageAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    uint8_t aucBufTx[512] = {0};
    uint8_t aucBufRx[512] = {0};


    if ((ulPageAddr > CHIP_TOTAL_BYTES) ||
        (ulLen > CHIP_PAGE_BYTES) ||
        (NULL == pucBuf)) {
        LOG_WRITE(EN_LOG_ERROR, "input parameter wrong!");
        goto ERR_LABEL;
    }

    aucBufTx[0] = CMD_READ;
    aucBufTx[1] = ulPageAddr >> 16;
    aucBufTx[2] = ulPageAddr >> 8;
    aucBufTx[3] = ulPageAddr;

    if (!m_stpDrvPtr->ReadWrite(m_ulChip, aucBufTx, 4, aucBufRx, std::make_shared<CRspHandler>(4 + ulLen))) {
        LOG_WRITE(EN_LOG_ERROR, "spi write and read interface error");
        goto ERR_LABEL;
    }

    if (!CheckTime(PROTECT_TRY_TIMES)) {
        LOG_WRITE(EN_LOG_ERROR, "time out.");
        goto ERR_LABEL;
    }

    memcpy(pucBuf, aucBufRx + 4, ulLen);

    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::WriteData(uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    uint8_t aucBufRx[CHIP_PAGE_BYTES] = {0};
    uint32_t nDataPageLen = 0;
    uint32_t nDataIndex = 0;
    uint32_t nBlockEraseAddr = 0;
    uint32_t nBlockEraseNum = 0;

    int32_t lLoadBit = FPGA_SPI_LOAD_BIT_1;

    LOG_WRITE(EN_LOG_NOTICE, "CFlash::WriteData ++ chip %d id %d len %d", m_ulChip, m_ulId, ulLen);

    if ((NULL == pucBuf) ||
        (ulAddr > CHIP_TOTAL_BYTES) ||
        ((ulAddr + ulLen) > CHIP_TOTAL_BYTES)) {
        LOG_WRITE(EN_LOG_ERROR, "input parameter wrong!");
        goto ERR_LABEL;
    }


    /* Read spi bus load mode                                                   */
    lLoadBit = GetLoadBit(pucBuf);
    if (OPLK_ERR == lLoadBit) {
        /* OPLK_LOG_PRINT_OPT("spi read load mode error, use 1 bit load mode"); */
    }

    if (!UnProtect()) {
        LOG_WRITE(EN_LOG_ERROR, "spi flash unprotect error!");
        goto ERR_LABEL;
    }

    nBlockEraseAddr = ulAddr;
    nBlockEraseNum = (ulLen + CHIP_BLOCK64K_LEN - 1) / CHIP_BLOCK64K_LEN;

    do {
        if (!EraseBy64K(nBlockEraseAddr)) {
            LOG_WRITE(EN_LOG_ERROR, "spi device erase by 64KB error.");
            goto ERR_LABEL;
        }

        nBlockEraseAddr += CHIP_BLOCK64K_LEN;
    }
    while (--nBlockEraseNum);

    LOG_WRITE(EN_LOG_NOTICE, "Erase OK");

    while (0 != ulLen) {
        if (ulLen < CHIP_PAGE_BYTES) {
            nDataPageLen = ulLen;
        } else {
            nDataPageLen = CHIP_PAGE_BYTES;
        }

        if (!WriteOnePage(ulAddr + nDataIndex, pucBuf + nDataIndex, nDataPageLen)) {
            LOG_WRITE(EN_LOG_ERROR, "spi write one page error.");
            goto ERR_LABEL;
        }

        memset(aucBufRx, 0, sizeof(aucBufRx));
        if (!ReadOnePage(ulAddr + nDataIndex, aucBufRx, nDataPageLen)) {
            LOG_WRITE(EN_LOG_ERROR, "spi read one page error.");
            goto ERR_LABEL;
        }

        /* Check                                                                */
        if (0 != memcmp(aucBufRx, pucBuf + nDataIndex, nDataPageLen)) {
            LOG_WRITE(EN_LOG_ERROR, "spi read one page cmp error, index = %d.", nDataIndex);
            goto ERR_LABEL;
        }

        nDataIndex += nDataPageLen;
        ulLen -= nDataPageLen;
    }

    if (!Protect(lLoadBit)) {
        LOG_WRITE(EN_LOG_ERROR, "spi flash protect error!");
        goto ERR_LABEL;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CFlash::WriteData --");

    return true;

ERR_LABEL:
    return false;
}
bool
CFlash::ReadData(uint32_t ulAddr, uint8_t *pucBuf, uint32_t ulLen)
{
    uint32_t nDataPageLen = 0;
    uint32_t nDataIndex = 0;

    LOG_WRITE(EN_LOG_NOTICE, "CFlash::ReadData ++ chip %d id %d len %d", m_ulChip, m_ulId, ulLen);

    if ((NULL == pucBuf) ||
        (ulAddr > CHIP_TOTAL_BYTES) ||
        ((ulAddr + ulLen) > CHIP_TOTAL_BYTES)) {
        LOG_WRITE(EN_LOG_ERROR, "input parameter wrong!");
        goto ERR_LABEL;
    }

    while (0 != ulLen) {
        if (ulLen < CHIP_PAGE_BYTES) {
            nDataPageLen = ulLen;
        } else {
            nDataPageLen = CHIP_PAGE_BYTES;
        }

        if (!ReadOnePage(ulAddr + nDataIndex, pucBuf + nDataIndex, nDataPageLen)) {
            LOG_WRITE(EN_LOG_ERROR, "read one page error.");
            goto ERR_LABEL;
        }

        nDataIndex += nDataPageLen;
        ulLen -= nDataPageLen;
    }

    LOG_WRITE(EN_LOG_NOTICE, "CFlash::ReadData -- 0x%x", pucBuf[0]);
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CFlash::ReadData **");
    return false;
}

bool
CFlash::IsPrimary()
{
    uint8_t aucImgHead[FPGA_IMG_HEAD_LEN];
    uint8_t aucImgHeadRd[FPGA_IMG_HEAD_LEN] = {0};

    /* Read the fpga image header from spi flash                                */
    if (!ReadData(0, aucImgHeadRd, sizeof(aucImgHeadRd))) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to read fpga image header from spi flash");
        return true;
    }

    memcpy(aucImgHead, m_aucFpgaHead, FPGA_IMG_HEAD_LEN);

    aucImgHead[FPGA1_ADDR] = aucImgHeadRd[FPGA1_ADDR];
    aucImgHead[FPGA2_ADDR] = aucImgHeadRd[FPGA2_ADDR];

    if ((aucImgHeadRd[FPGA1_ADDR] != m_aucFpgaHead[FPGA1_ADDR])) {
        LOG_WRITE(EN_LOG_NOTICE, "IsPrimary false,0x%0x,0x%x,0x%x,0x%x\n", aucImgHeadRd[FPGA1_ADDR], m_aucFpgaHead[FPGA2_ADDR], aucImgHeadRd[FPGA2_ADDR], m_aucFpgaHead[FPGA1_ADDR]);
        return false;
    }
    LOG_WRITE(EN_LOG_NOTICE, "IsPrimary true,0x%0x,0x%x,0x%x,0x%x\n", aucImgHeadRd[FPGA1_ADDR], m_aucFpgaHead[FPGA2_ADDR], aucImgHeadRd[FPGA2_ADDR], m_aucFpgaHead[FPGA1_ADDR]);
    return true;
}
bool
CFlash::WriteHead(uint32_t ulAddr1, uint32_t ulAddr2)
{
    uint8_t aucImgHead[FPGA_IMG_HEAD_LEN];
    uint8_t aucImgHeadRd[FPGA_IMG_HEAD_LEN] = {0};


    /* Read the fpga image header from spi flash                                */
    if (!ReadData(0, aucImgHeadRd, sizeof(aucImgHeadRd))) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to read fpga image header from spi flash");
        goto ERR_LABEL;
    }

    memcpy(aucImgHead, m_aucFpgaHead, FPGA_IMG_HEAD_LEN);

    aucImgHead[FPGA1_ADDR] = ulAddr1 >> 16;
    /*aucImgHead[FPGA2_ADDR] = ulAddr2 >> 16; */

    /* Writes the image header if no header                                     */
    if (0 != memcmp(aucImgHead, aucImgHeadRd, FPGA_IMG_HEAD_LEN)) {
        if (!WriteData(0, aucImgHead, sizeof(aucImgHead))) {
            goto ERR_LABEL;
        }
    }

    return true;

ERR_LABEL:
    return false;
}

#if 1
bool
CFlash::WriteImage(uint8_t *pucBuf, uint32_t ulLen)
{
    uint32_t ulAddr = 0;
    uint32_t ulHeadToAddr = 0;


    LOG_WRITE(EN_LOG_NOTICE, "CFlash::WriteImage ++ chip %d id %d len %d", m_ulChip, m_ulId, ulLen);

    printf("CFlash::WriteImage ++ chip %d id %d len %d\n", m_ulChip, m_ulId, ulLen);

    if (!IsPrimary()) {
        ulAddr = FPGA1_ADDR_IN_FLASH;
        ulHeadToAddr = FPGA2_ADDR_IN_FLASH;
    } else {
        ulAddr = FPGA2_ADDR_IN_FLASH;
        ulHeadToAddr = FPGA1_ADDR_IN_FLASH;
    }

    printf("CFlash::WriteImage ++ 0\n");
    LOG_WRITE(EN_LOG_NOTICE, "WriteImage, ulAddr:0x%x,ulHeadToAddr:0x%x", ulAddr, ulHeadToAddr);

    if (!WriteData(ulAddr, pucBuf, ulLen)) {

        printf("CFlash::WriteImage error0\n");
        goto ERR_LABEL;
    }


    printf("CFlash::WriteImage ++ 1\n");

    if (!WriteHead(ulAddr, ulHeadToAddr)) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to write fpga head to spi flash");

        printf("CFlash::WriteImage error1\n");
        goto ERR_LABEL;
    }


    printf("CFlash::WriteImage ++ 2\n");

    LOG_WRITE(EN_LOG_NOTICE, "CFlash::WriteImage --");
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CFlash::WriteImage **");

    printf("CFlash::WriteImage error\n");
    return false;
}
#else
bool
CFlash::WriteImage(uint8_t *pucBuf, uint32_t ulLen)
{
    uint32_t ulAddr = 0;
    uint32_t ulHeadToAddr = 0;


    LOG_WRITE(EN_LOG_NOTICE, "CFlash::WriteImage ++ chip %d id %d len %d", m_ulChip, m_ulId, ulLen);



    printf("CFlash::WriteImage ++ 1\n");

    if (!WriteHead(0x800000, ulHeadToAddr)) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to write fpga head to spi flash");

        printf("CFlash::WriteImage error1\n");
        goto ERR_LABEL;
    }


    printf("CFlash::WriteImage ++ 2\n");

    LOG_WRITE(EN_LOG_NOTICE, "CFlash::WriteImage --");
    return true;

ERR_LABEL:
    LOG_WRITE(EN_LOG_ERROR, "CFlash::WriteImage **");

    printf("CFlash::WriteImage error\n");
    return false;
}

#endif

bool
CFlash::Erase()
{
    int32_t i = 0;

    if (!UnProtect()) {
        LOG_WRITE(EN_LOG_ERROR, "UnProtect error!");
        goto ERR_LABEL;
    }

    for (i = 0; i < CHIP_TOTAL_BYTES / CHIP_BLOCK64K_LEN; i++) {
        if (!EraseBy64K(CHIP_BLOCK64K_LEN * i)) {
            LOG_WRITE(EN_LOG_ERROR, "erase by 64k error.");
            goto ERR_LABEL;
        }
    }
    return true;

ERR_LABEL:
    return false;
}



