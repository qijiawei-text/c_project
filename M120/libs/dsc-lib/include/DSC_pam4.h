#ifndef _DSC_PAM4_H
#define _DSC_PAM4_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define PAM4_BER_MAX_LEN                   (64)
#define PAM4_LOCK_STATE_MAX_LEN            (64)
typedef struct st_PAM4_INFO
{
    bool bTxenable;         /* WR    */
    char acBer[PAM4_BER_MAX_LEN]; /* RONLY */
    char acLockState[PAM4_LOCK_STATE_MAX_LEN];
    /* char acBerHighThr;      / * WR    * / */
    /* char acBerLowThr;       / * WR    * / */
    double dTxPower;        /* RONLY */
    double dTxHighThr;      /* WR    */
    double dTxLowThr;       /* WR    */
    double dRxPower;        /* RONLY */
    double dRxHighThr;      /* WR    */
    double dRxLowThr;       /* WR    */
    double dLaserTemp;      /* RONLY */
    double dCaseTemp;       /* RONLY */
}ST_PAM4_INFO;

typedef enum en_PAM4_LOCK_STATE
{
    PAM4_LOCK_UNKNOWN = 0,
    PAM4_LOCK_RUNNING,
    PAM4_LOCK_SUCC,
    PAM4_LOCK_FAILED,
    PAM4_LOCK_MAX
}EN_PAM4_LOCK_STATE;

#define MODULE_NS_PAM4                      "com-oplink-pam4"
#define PAM4_MAX_MON_ATTR                   6
#define PAM4_XPATH_HEADER                   COO_XPATH_PREFIX "/com-oplink-pam4:pam4"
#define PAM4_XPATH_ALL                      PAM4_XPATH_HEADER "//*"
#define PAM4_XPATH_CONFIG                   PAM4_XPATH_HEADER "/config"
#define PAM4_XPATH_STATE                    PAM4_XPATH_HEADER "/pam4-state"

#define PAM4_LEAF_CD_SETTING                "/cd-setting"
#define PAM4_XPATH_CD_SETTING                PAM4_XPATH_CONFIG PAM4_LEAF_CD_SETTING

#define PAM4_LEAF_TX_ENABLE                 "/tx-enable"
#define PAM4_XPATH_TX_ENABLE                PAM4_XPATH_CONFIG PAM4_LEAF_TX_ENABLE

#define PAM4_LEAF_WAVELEN_NUM               "/wavelength-number"
#define PAM4_XPATH_WAVELEN_NUM               PAM4_XPATH_CONFIG PAM4_LEAF_WAVELEN_NUM

#define PAM4_LEAF_BER                       "/bit-error-rate"
#define PAM4_XPATH_BER                      PAM4_XPATH_STATE PAM4_LEAF_BER

#define PAM4_LEAF_ADD_POWER                 "/add-power"
#define PAM4_XPATH_ADD_POWER                PAM4_XPATH_STATE PAM4_LEAF_ADD_POWER

#define PAM4_LEAF_DROP_POWER                "/drop-power"
#define PAM4_XPATH_DROP_POWER               PAM4_XPATH_STATE PAM4_LEAF_DROP_POWER

#define PAM4_LEAF_LASER_TEMP                "/laser-temperature"
#define PAM4_XPATH_LASER_TEMP               PAM4_XPATH_STATE PAM4_LEAF_LASER_TEMP

#define PAM4_LEAF_CASE_TEMP                 "/case-temperature"
#define PAM4_XPATH_CASE_TEMP                PAM4_XPATH_STATE PAM4_LEAF_CASE_TEMP

#define PAM4_LEAF_LOCK_STATE                "/lock-state"
#define PAM4_XPATH_LOCK_STATE               PAM4_XPATH_STATE PAM4_LEAF_LOCK_STATE

#define RPC_XPATH_PAM4_REACQUIRE            "/com-oplink-pam4:reacquire"
#define RPC_XPATH_PAM4_REACQUIRE_OPTION     RPC_XPATH_PAM4_REACQUIRE "/option"

#define RPC_XPATH_PAM4_CLEAR_STATICS        "/com-oplink-pam4:clear-statistics"
#define RPC_XPATH_PAM4_CLEAR_STA_OPTION     RPC_XPATH_PAM4_CLEAR_STATICS "/option"

#define RPC_XPATH_PAM4_FEC_ENABLE           "/com-oplink-pam4:set-fec"
#define RPC_XPATH_PAM4_FEC_ENABLE_OPTION    RPC_XPATH_PAM4_FEC_ENABLE "/option"

#define RPC_XPATH_PAM4_RESET_MUTIFY         "/com-oplink-pam4:reset-mutify"
#define RPC_XPATH_PAM4_RESET_MUTIFY_OPTION  RPC_XPATH_PAM4_RESET_MUTIFY "/option"

extern int DSC_SetPam4RxThreshold(double dHighThr, double dLowThr);
extern int DSC_SetPam4TxThreshold(double dHighThr, double dLowThr);
extern int DSC_SetPam4BerThreshold(double dHighThr, double dLowThr);
extern int DSC_SetPam4TxEnable(bool state);
extern int DSC_SetPam4WavelenNum(uint32_t u32WavelenNum);
extern int DSC_Pam4Reacquire(bool bBoolean);
extern int DSC_Pam4ClearStatistics(bool bBoolean);
extern int DSC_Pam4SetFecStatus(unsigned char ucStatus);
extern int DSC_GetPam4Info(ST_PAM4_INFO *psPam4Info, size_t size);
extern int DSC_Pam4ResetMutify(bool bBoolean);

#ifdef __cplusplus
}
#endif

#endif
