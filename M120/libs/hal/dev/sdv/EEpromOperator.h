#pragma once

#include "EEprom.h"

#define EEPROM_MFG_BUF_LEN               (4096)


/*  EEPROM Header Structure */
typedef struct EEPROM_CFG_HEAD_st
{
    uint32_t uiHeadCrc32;
    uint32_t uiDataLen;
    uint32_t uiDataCrc32;
    uint32_t uiCfgVersion;          /* cfg version */
    uint32_t uiResv1;
    uint32_t uiResv2;
    uint32_t uiResv3;
    uint32_t uiResv4;
} EEPROM_CFG_HEAD_ST;

#define EEPROM_CFG_HEAD_SIZE            (sizeof(EEPROM_CFG_HEAD_ST))

class CEEpromOperator
{
public:
    static bool ReadMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, CMfg &rstMfg);
    static bool ReadMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, CMfg &rstMfg, uint32_t uiIdx);
    static bool WriteMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcFilePath);
    static bool WriteMfg(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcFilePath, uint32_t uiIdx);
    static bool WriteMfgConfigType(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcConfigType);

    static int ReadCfg(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddrSlice, const char *pcFilePath);
    static int  WriteCfg(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddrSlice, uint32_t uiSize, const char *pcFilePath);

    static bool ReadComCali(const std::shared_ptr<CEEprom> c_rstpEEprom, CCali &rstCali);
    static bool WriteComCali(const std::shared_ptr<CEEprom> c_rstpEEprom, const char *c_pcFilePath);

    static bool ReadUpgInfo(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen);
    static bool WriteUpgInfo(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen);

    static bool ReadBdlHead(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen);
    static bool WriteBdlHead(const std::shared_ptr<CEEprom> c_rstpEEprom, uint8_t *pucBuf, uint32_t uiLen);

    static bool GetAddrOffset(uint32_t uiAddrSlice, uint32_t &uiAddrOffset);
    static bool CheckCfgTar(char *pcTarFileName);

/* private: */
    static bool Read(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen);
    static bool Write(const std::shared_ptr<CEEprom> c_rstpEEprom, uint32_t uiAddr, uint8_t *pucBuf, uint32_t uiLen);
};



