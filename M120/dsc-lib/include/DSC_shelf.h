#ifndef _DSC_SHELF_H_
#define _DSC_SHELF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_ood.h"
#include "org_openroadm_common_equipment_types.h"
#include "org_openroadm_common_link_types.h"
#include "org_openroadm_common_alarm_pm_types.h"
#include "org_openroadm_port_types.h"
#include "org_openroadm_device_types.h"

typedef enum en_SHELF
{
    SHELF_DEFAULT = 0,
    SHELF_MAX
} EN_SHELF;


#define SLOT_STATUS_EMPTY_STR             "empty-not-prov"
#define SLOT_STATUS_INSTALL_MATCH_STR     "installed-prov-match"
#define SLOT_STATUS_INSTALL_MISMATCH_STR  "installed-prov-mismatch"


#define SLOT_KEY_FAN   "slot-fan-"
#define SLOT_KEY_POWER "slot-pm-"
#define SLOT_KEY_EDFA  "slot-oa-"
#define SLOT_KEY_OPS   "slot-ops-"
#define SLOT_KEY_OCM   "slot-ocm-"


#define PROV_CIRCUITPACK_NUM_MAX         (4)
typedef struct st_SLOT
{
    char acName[32];  /* key */
    char acLabel[32];
    char acCurProvisionedCircuitPack[64];
    char aacRefProvisionedCircuitPack[PROV_CIRCUITPACK_NUM_MAX][64];
    char acSlotStatus[32];
}ST_SLOT;


#define SHELF_SLOT_NUM_MAX         (32)
typedef struct st_SHELF
{
    char acName[32];  /* key */
    char acType[32];
    char acRack[32];
    char acPosition[32];
    EN_LIFECYCLE_STATE enLifeCycleState;
    EN_EQPT_ADMIN_STATES enEqptAdminState;
    ST_PHYSICAL_INFO stPhysical;
    EN_EQPT_STATES enEqptState;
    bool bIsPhysical;
    bool bIsPassive;
    char acFacePlateLabel[32];
    char acDueDate[64];
    ST_SLOT astSlot[SHELF_SLOT_NUM_MAX];
}ST_SHELF;

typedef struct st_shelf_TEMP_INFO
{
    double dBoardTemp;
    double dHighThr;
    double dLowThr;
} SHELF_TEMP_INFO_ST;


extern int DSC_SHELF_GetName(char *pcName, size_t tNameMaxLen);
extern int DSC_SHELF_GetNameFromStartup(char *pcName, size_t nLen);

extern int DSC_SHELF_GetInfo(ST_SHELF *pstShelf);
extern int DSC_SHELF_GetSlots(ST_SLOT **ppstSlot, size_t *ptCount);

extern int DSC_SHELF_GetSlotsNameById(int iSlotId, char *pcSlotName);
extern int DSC_SHELF_GetSlotsIdByName(char *pcSlotName, int *piSlotId);

extern int DSC_SHELF_SetSlotFanStatus(int iIndex, char *pcSlotStatusStr);
extern int DSC_SHELF_SetSlotPowerStatus(int iIndex, char *pcSlotStatusStr);
extern int DSC_SHELF_SetSlotModuleStatus(char *pcSlotName, char *pcSlotStatus);
extern int DSC_SHELF_SetResetCause(uint8_t ucRestCause);
extern int DSC_SHELF_GetResetCause(char *pcBuf, size_t nLen);
extern int DSC_SHELF_ResetCauseEnumToString(EN_COMMON_INFO_RESET_CAUSE enResetCause, char *pcString, size_t nLen);
extern int DSC_SHELF_GetTemperature(SHELF_TEMP_INFO_ST *pstTempInfo);
extern int DSC_SHELF_SetTemperatureThr(double dHighThr, double dLowThr);
#ifdef __cplusplus
}
#endif

#endif
