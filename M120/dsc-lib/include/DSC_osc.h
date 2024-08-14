#ifndef _DSC_OSC_H
#define _DSC_OSC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

/**************************************************************************************************/
/* definition OSC number                                                                          */
/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_OSC_IDX_1 = 0,
    EN_OSC_IDX_2 = 1,
    EN_OSC_IDX_W = 0,
    EN_OSC_IDX_P = 1,
    EN_OSC_IDX_MAX = 2
} EN_OSC_IDX;
/* } @                                                                                            */


#define OSC1_CFG_KEY           "OSC1"
#define OSC2_CFG_KEY           "OSC2"

#define OSC_CFG_KEY           "OSC"
#define OSC_P_CFG_KEY         "OSC-P"
#define OSC_WSS_CFG_KEY       "OSC-WSS"

#define Osc_KEY_MAX           (4)

#define OSC_FORMAT_12_5M_STR   "osc_12_5"
#define OSC_FORMAT_150M_STR    "osc_150"
#define OSC_FORMAT_155M_STR    "osc_155"
#define OSC_WAVELENGTH (1510)

#define DEF_SPANLOSS_CFG_KEY        "SPANLOSS"
#define DEF_SPANLOSS_P_CFG_KEY      "SPANLOSS-P"

typedef enum en_OSC_FORMAT
{
    OSC_FORMAT_12_5M = 0,
    OSC_FORMAT_150M,
    OSC_FORMAT_155M
} OSC_FORMAT_EN;

typedef struct st_OSC_CONF
{
    bool bEnable;
    double dPowerSetPoint;
    double dHighThr;
    double dLowThr;
    char acJ0[16];
} OSC_CONF_ST;

typedef struct st_OSC_MON
{
    double dPower;
} OSC_MON_ST;

typedef struct st_OSC_INFO
{
    OSC_FORMAT_EN enOscFormat;
    OSC_CONF_ST stOscRxConf;
    OSC_CONF_ST stOscTxConf;
    OSC_CONF_ST stOscDropConf;
    OSC_CONF_ST stOscAddConf;
    OSC_MON_ST stOscRxMon;
    OSC_MON_ST stOscTxMon;
    OSC_MON_ST stOscDropMon;
    OSC_MON_ST stOscAddMon;
} OSC_INFO_ST;

typedef struct  st_osc_key {
    int32_t iCnt;
    char aacOscKeys[Osc_KEY_MAX][8];
} OSC_KEY_ST;

#define MODULE_NS_OSC                      "openconfig-optical-amplifier"
#define DEF_OSC_MAX_MON_ATTR               4
#define OSC_XPATH_HEADER                   "/openconfig-optical-amplifier:optical-amplifier/supervisory-channels"

#define OSC_XPATH_ALL                      OSC_XPATH_HEADER "//*"
#define OSC_XPATH_NAME                     OSC_XPATH_HEADER "/supervisory-channel/interface"
#define OSC_RX_NODE                        "/osc-rx"
#define OSC_TX_NODE                        "/osc-tx"
#define OSC_XPATH_TX_CONF                  OSC_XPATH_HEADER "/supervisory-channel[interface='%s']/config"
#define OSC_XPATH_TX_ALL_CONF              OSC_XPATH_HEADER "/supervisory-channel/config//*"
#define OSC_XPATH_MON                      OSC_XPATH_HEADER "/supervisory-channel[interface='%s']/state"
#define OC3_ENABLE_XPATH                   "/com-oplink-amplifier-ext:osc-sfp-enable"
#define OSC_XPATH_TX_OC3_ENABLE            OSC_XPATH_TX_CONF OC3_ENABLE_XPATH
#define OSC_XPATH_RX_PWR                   OSC_XPATH_MON "/input-power/instant"
#define OSC_XPATH_TX_PWR                   OSC_XPATH_MON "/output-power/instant"
#define OSC_XPATH_DROP_PWR                 OSC_XPATH_MON "/com-oplink-amplifier-ext:drop-power"
#define OSC_XPATH_ADD_PWR                  OSC_XPATH_MON "/com-oplink-amplifier-ext:add-power"
#define OSC_XPATH_STATE_ALL                OSC_XPATH_MON "//*"

extern int DSC_GetOscCfgKeyFromDb(OSC_KEY_ST *pstOscKey);
extern int DSC_GetOscCfgKey(char *pcXpath, char *pcKeyValue);
extern int DSC_GetOscDevIdByKey(char *pcCfgKey, int *piDevId);
extern int DSC_GetOscAllName(char aacName[][32], size_t * pnCount);
extern int DSC_GetOscInfo(char *pcKey, OSC_INFO_ST *pstOscInfo, size_t size);
extern int DSC_GetOscConfig(char *pcKey, OSC_CONF_ST *pstOscCfg);
extern int DSC_SetOscOutputEnable(char *pcKey, bool state);
extern int DSC_SetOscTxLosThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscRxLosThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscAddLosThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscDropLosThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscRxLowThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscRxHighThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscTxLowThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscTxHighThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscDropLowThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscDropHighThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscAddLowThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetOscAddHighThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetSpanlossVariationLowThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetSpanlossVariationHighThreshold(char *pcKey, double dHighThr, double dLowThr);

#ifdef __cplusplus
}
#endif

#endif
