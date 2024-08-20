#ifndef _DSC_MODULE_H
#define _DSC_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"
#include "DSC_shelf.h"


typedef struct
{
    char acCfgKey[32]; /* key */
    char acCpName[32];
    char acComponentName[32];
}ST_MODULE_CFG_CP_MAPPING;

/*
   The data value must be consistent with ECardType in devslotdata.h
 */
typedef enum
{
    CARD_OCM = 0,
    CARD_PA = 1,
    CARD_BA,
    CARD_OPS,
    CARD_WSS,
    CARD_AMP,
    CARD_OCS = CARD_AMP,
    CARD_400GZR_BETA = 7,

    CARD_FAN,
    CARD_POWER,

    CARD_SCC,
    CARD_CLASSIS,
    CARD_FIX,
    CARD_UNKOWN = 0xFF
} EN_CARD_TYPE;

typedef enum
{
    MODULE_EDFA = 1,
    MODULE_OPS,
    MODULE_WSS,
    MODULE_TDCM,
    MODULE_OCM,
    MODULE_OTDR,
    MODULE_MAX
} EN_MODULE_TYPE;

typedef struct
{
    int iSlotIdx;       /* The value is equal to the slot ID of hal */
    int iCardType;
    char acCpName[32];
}ST_CARD_CP_MAPPING;

typedef struct
{
    int iModuleType;
    char acCfgKey[32];
}ST_MODULE_TYPE_KEY_MAPPING;


extern int DSC_GetALLModuleCfgMapping(ST_MODULE_CFG_CP_MAPPING *pstMappingInfo);
extern int DSC_GetOpticalCfgInfo(char *pcXpath, ST_OPTICAL_CONFIG *pstOpticalInfo, size_t *pnCount);
extern int DSC_GetModuleCfgKeyByCpName(char *pcCpName, char *pcComponentName,
                                       ST_MODULE_CFG_CP_MAPPING *pstAllInfo, char *pcKey);
extern int DSC_GetSlotIdByModuleCfgKey(char *pcKey, ST_MODULE_CFG_CP_MAPPING *pstAllInfo, int *piSlotId);
extern int DSC_GetCpNameByCardType(int iSlotIdx, int iCardType, char *pcCpName);
extern int DSC_GetCpNameBySlotId(int iSlotIdx, char *pcCpName, ST_SHELF *pstShelf);
extern int DSC_GetCardTypeByCpName(char *pcCpName, int *piCardType);
extern int DSC_GetSlotIndexByCpKey(const char *pcCpKey, int *piSlotIndex);

#ifdef __cplusplus
}
#endif

#endif

