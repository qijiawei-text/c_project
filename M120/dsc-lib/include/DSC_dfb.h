#ifndef _DSC_DFB_H
#define _DSC_DFB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define DFB_CFG_KEY                             "dfb-hp"
#define DFB_P_CFG_KEY                           "dfb-p-hp"

#define MODULE_DFB                              "com-oplink-dfb"
#define DFB_XPATH_HEADER                        "/com-oplink-dfb:optical-dfb/dfbs"
#define DFB_XPATH_ALL                           DFB_XPATH_HEADER "//*"

#define DFB_XPATH_DFBS                          DFB_XPATH_HEADER
#define DFB_XPATH_DFB_LIST                      DFB_XPATH_DFBS "/dfb"
#define DFB_XPATH_DFB_LIST_NAME                 DFB_XPATH_DFBS DFB_XPATH_DFBS "/dfb/name"
#define DFB_XPATH_DFB                           DFB_XPATH_DFBS "/dfb[name='%s']"
#define DFB_XPATH_ALL_CONFIG                    DFB_XPATH_DFBS "/dfb/config//*"

#define DFB_XPATH_CONFIG                        DFB_XPATH_DFB "/config"
#define DFB_XPATH_CONFIG_ALL                    DFB_XPATH_CONFIG "//*"
#define DFB_XPATH_CONFIG_NAME                   DFB_XPATH_CONFIG "/name"
#define DFB_XPATH_CONFIG_TARGET_OUTPUT_POWER    DFB_XPATH_CONFIG "/target-output-power"
#define DFB_XPATH_CONFIG_ENABLED                DFB_XPATH_CONFIG "/enabled"
#define XPATH_CONFIG_ENABLED                    "/config/enabled"
#define XPATH_CONFIG_TARGET_OUTPUT_POWER        "/config/target-output-power"

#define DFB_XPATH_STATE                         DFB_XPATH_DFB "/state"
#define DFB_XPATH_STATE_DFB_ALL                 DFB_XPATH_STATE "//*"
#define DFB_XPATH_STATE_DFBS_ALL                DFB_XPATH_DFB_LIST "/state//*"
#define DFB_XPATH_STATE_NAME                    DFB_XPATH_STATE "/name "
#define DFB_XPATH_STATE_TARGET_OUTPUT_POWER     DFB_XPATH_STATE "/target-output-power"
#define DFB_XPATH_STATE_ENABLED                 DFB_XPATH_STATE "/enabled"

#define DFB_XPATH_NAME_ALL                      DFB_XPATH_DFB_LIST "/name"

#define DFB_XPATH_STATE_DFB_PORTS               DFB_XPATH_STATE "/dfb-port"
#define DFB_XPATHSTATE_DFB_PORTS_ALL            DFB_XPATH_STATE_DFB_PORTS "//*"

#define DFB_XPATH_STATE_DFB_PORT                DFB_XPATH_STATE "/dfb-port[port-name='%s']"
#define DFB_XPATH_STATE_DFB_PORT_INPUT_POWER    DFB_XPATH_STATE_DFB_PORT "/input-power/instant"
#define DFB_XPATH_STATE_DFB_PORT_OUTPUT_POWER   DFB_XPATH_STATE_DFB_PORT "/output-power/instant"

#define DFB_NODE_DFB                            "dfb"
#define DFB_NODE_NAME                           "name"

#define COO_I_XPATH_DFB_MODULE_ALL               COO_I_XPATH_PREFIX "/optical-modules/optical-dfb/dfb-modules/dfb-module[name='%s']//*"
#define COO_I_XPATH_DFB_NAME_COMPONENT_ALL       COO_I_XPATH_PREFIX "/optical-modules/optical-dfb/dfb-modules/dfb-module[component-name='%s']/name"

/**************************************************************************************************/
/* @ {                                                                                            */
typedef enum {
    EN_DFB_IDX_W_R = 0,
    EN_DFB_IDX_W_T,
    EN_DFB_IDX_1 = 0,
    EN_DFB_IDX_2 = 1,
    EN_DFB_IDX_MAX
} EN_DFB_IDX;
/* } @                                                                                            */

typedef struct {
    int32_t iCnt;
    char aacDfbKeys[EN_DFB_IDX_MAX][24];
} DFB_KEY_ST;

typedef enum {
    EN_DFB_PORT_ID_LINE_IN = 0,
    EN_DFB_PORT_ID_LINE_OUT,
    EN_DFB_PORT_ID_MUX_IN,
    EN_DFB_PORT_ID_MUX_OUT,
    EN_DFB_PORT_ID_MAX
} EN_DFB_PORT_ID;

typedef struct DFB_st {
    char acName[32];
    unsigned char ucId;
} DFB_ST;

typedef struct st_DFB_CONF
{
    bool bEnable;
    double dTargetOutputPower;
    double dLaserSafetyRHighThr;
    double dLaserSafetyRLowThr;
    double dLaserSafetyTHighThr;
    double dLaserSafetyTLowThr;
} DFB_CONF_ST;

typedef struct st_DFB_MON
{
    double dInputPower;
    double dOutputPower;
} DFB_MON_ST;

typedef struct st_DFB_INFO
{
    DFB_CONF_ST stDfbConf;
    double dTargetOutputPower;
    DFB_MON_ST stDfbMuxInMon;
    DFB_MON_ST stDfbMuxOutMon;
    DFB_MON_ST stDfbLineInMon;
    DFB_MON_ST stDfbLineOutMon;
} DFB_INFO_ST;

extern int DSC_GetDfbArrayLength();
extern int DSC_GetDfbCnt();
extern int DSC_GetSingleDfbCnt(const char *pcComponentKey);
extern int DSC_GetDfbCfgKeyFromDb(DFB_KEY_ST *pstDfbKey);
extern int DSC_GetDfbCfgKey(char *pcXpath, char *pcKeyValue);
extern int DSC_GetDfbDevIdByKey(char *pcCfgKey, int *piDevId);
extern int DSC_GetDfbAllName(char aacName[][32], size_t * pnCount);
extern int DSC_GetDfbInfo(char *pcKey, DFB_INFO_ST *pstDfbInfo, size_t size);
extern int DSC_GetDfbConfig(char *pcKey, DFB_CONF_ST *pstDfbCfg);
extern int DSC_SetDfbOutputEnable(char *pcKey, bool state, double dTargetOutputPower);
extern int DSC_SetLaserSafetyRxLosThreshold(char *pcKey, double dHighThr, double dLowThr);
extern int DSC_SetLaserSafetyTxLosThreshold(char *pcKey, double dHighThr, double dLowThr);

extern char aacDfbName[EN_DFB_IDX_MAX][16];

#ifdef __cplusplus
}
#endif

#endif
