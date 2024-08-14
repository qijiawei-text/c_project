#ifndef _DSC_CIRCUIT_PACK_H_
#define _DSC_CIRCUIT_PACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_ood.h"
#include "org_openroadm_common_equipment_types.h"
#include "org_openroadm_common_link_types.h"
#include "org_openroadm_common_alarm_pm_types.h"
#include "org_openroadm_port_types.h"
#include "org_openroadm_device_types.h"

#define CIRCUIT_PACK_BOARD       "cp-main-board"
#define CIRCUIT_PACK_CHASSIS     "cp-chassis"
#define CIRCUIT_PACK_KEY_FAN     "cp-fan-"
#define CIRCUIT_PACK_KEY_POWER   "cp-power-"
#define CIRCUIT_PACK_KEY_OPS     "cp-ops-"
#define CIRCUIT_PACK_OPS         "cp-ops"
#define CIRCUIT_PACK_EDFA_BA     "cp-edfa-ba"
#define CIRCUIT_PACK_EDFA_PA     "cp-edfa-pa"
#define CIRCUIT_PACK_AMP         "cp-amp"
#define CIRCUIT_PACK_AMP_P       "cp-amp-p"
#define CIRCUIT_PACK_OCM         "cp-ocm"
#define CIRCUIT_PACK_SCC         "cp-scc"
#define CIRCUIT_PACK_OPTICAL     "cp-optical"
#define CIRCUIT_PACK_MUX         "cp-mux"
#define CIRCUIT_PACK_RPC         "cp-rpc"
#define CIRCUIT_PACK_OPS4_A        "cp-ops4-a"
#define CIRCUIT_PACK_OPS4_B        "cp-ops4-b"
#define CIRCUIT_PACK_OCS         "cp-ocs"
/*
 * different product has it's own component
 */
/* scc card's component */
#define CP_COMPONENT_SCC_APP      "scc-app"
#define CP_COMPONENT_SCC_FPGA     "scc-fpga"
#define CP_COMPONENT_SCC_CPLD     "scc-cpld"

/* amp card's component */
#define CP_COMPONENT_AMP_EDFA     "amp-edfa"
#define CP_COMPONENT_AMP_BA       "ba"
#define CP_COMPONENT_AMP_PA       "pa"
#define CP_COMPONENT_AMP_OCM      "ocm"
#define CP_COMPONENT_AMP_OTDR     "otdr"

/* amp-p card's component */
#define CP_COMPONENT_AMP_P_EDFA   "amp-p-edfa"
#define CP_COMPONENT_AMP_P_BA     "ba-p"
#define CP_COMPONENT_AMP_P_PA     "pa-p"
#define CP_COMPONENT_AMP_P_OCM    "ocm-p"
#define CP_COMPONENT_AMP_P_OTDR   "otdr-p"

/* ops card's component */
#define CP_COMPONENT_OPS_CPLD     "ops-cpld"
#define CP_COMPONENT_OPS          "ops"

/* ocs card's component */
#define CP_COMPONENT_OCS_OCS      "ocs"

/* optical module key on OCS card */
#define OCS_OCS_KEY               "OCS"

/* optical module key on AMP card */
#define AMP_PA_KEY                "PA"
#define AMP_BA_KEY                "BA"
#define AMP_OSC_KEY               "OSC"
#define AMP_OCM_KEY               "OCM"
#define AMP_BA_VOA_KEY            "PADDING-VOA"
#define AMP_PA_VOA_KEY            "PA-VOA"
#define AMP_OSC_VOA_KEY           "OSC-VOA"
#define AMP_OSC_TX_VOA_KEY        "OSC-TX-VOA"
#define AMP_OCM_VOA_KEY           "OCM-VOA"
#define AMP_OTDR_KEY              "OTDR"

/* optical module key on AMP-P card */
#define AMP_P_PA_KEY              "PA-P"
#define AMP_P_BA_KEY              "BA-P"
#define AMP_P_OSC_KEY             "OSC-P"
#define AMP_P_OCM_KEY             "OCM-P"
#define AMP_P_BA_VOA_KEY          "PADDING-VOA-P"
#define AMP_P_PA_VOA_KEY          "PA-VOA-P"
#define AMP_P_OSC_VOA_KEY         "OSC-VOA-P"
#define AMP_P_OSC_TX_VOA_KEY      "OSC-TX-VOA-P"
#define AMP_P_OCM_VOA_KEY         "OCM-VOA-P"
#define AMP_P_OTDR_KEY            "OTDR-P"

/* optical module key on OPS card */
#define OPS_OLP_KEY               "OLP"

#define DEF_OOD_MANUFACTURE_DATE     "2000-01-01T00:00:00Z"
#define DEF_OOD_MODEL                "DEF_model"
#define DEF_OOD_VENDOR               "DEF_vendor"
#define DEF_OOD_SERIAL_ID            "DEF_serial_id"

typedef struct st_CIRCUIT_PACK_CATEGORY
{
    EN_EQUIPMENT_TYPE_ENUM enType;
    char acExtension[32];
}ST_CIRCUIT_PACK_CATEGORY;

#define CP_FEATURE_NUM_MAX       (8)
typedef struct st_CIRCUIT_PACK_FEATURE
{
    char acDescription[32];
    bool bBoot;
    bool bActivate;
}ST_CIRCUIT_PACK_FEATURE;

#define CP_COMPONENT_NUM_MAX     (16)
typedef struct st_CIRCUIT_PACK_COMPONENT
{
    char acName[32]; /* key */
    bool bBoot;
    char acCurrentVer[32];
    char acVerToApply[32];
}ST_CIRCUIT_PACK_COMPONENT;

typedef struct st_CIRCUIT_PACK_PORT_PROPERTY
{
    power_dBm dCapMinRxPower;
    power_dBm dCapMinTxPower;
    power_dBm dCapMaxRxPower;
    power_dBm dCapMaxTxPower;
} ST_CIRCUIT_PACK_PORT_PROPERTY;

#define CP_PORT_NUM_MAX     (32)
typedef struct st_CIRCUIT_PACK_PORT
{
    uint32_t u32PortId;
    char acName[32];
    EN_CMN_ALM_PM_TYPE_DIRECTION enDirection;
    char acType[32];
    EN_PORT_WAVELENGTH_TYPES enWavelengthType;
    EN_PORT_QUAL enQualType;
    power_dBm dTargetPower;
    power_dBm dCurrentPower;
    ST_CIRCUIT_PACK_PORT_PROPERTY stProperty;
    char acFacePlateLabel[32];
    EN_LIFECYCLE_STATE enLifeCycleState;
    EN_EQPT_ADMIN_STATES enEqptAdminState;
    EN_STATE enOperationalState;
}ST_CIRCUIT_PACK_PORT;


#define CIRCUIT_PACK_NUM_MAX       (32)
typedef struct st_CIRCUIT_PACK
{
    char acName[32]; /* key */
    char acType[32];
    char acProductCode[32];
    EN_LIFECYCLE_STATE enLifeCycleState;
    EN_EQPT_ADMIN_STATES enEqptAdminState;
    ST_PHYSICAL_INFO stPhysical;
    ST_CIRCUIT_PACK_CATEGORY stCategory;
    EN_EQPT_STATES enEqptState;
    char acMode[32];
    char acRefShelfName[32];
    char acRefSlotName[32];
    char acSwLoadVer[32];
    ST_CIRCUIT_PACK_FEATURE astFeature[CP_FEATURE_NUM_MAX];
    ST_CIRCUIT_PACK_COMPONENT astComponent[CP_COMPONENT_NUM_MAX];
    ST_CIRCUIT_PACK_PORT astPort[CP_PORT_NUM_MAX];
}ST_CIRCUIT_PACK;


typedef struct
{
    char acKey[32]; /* key */
    char acCpName[32];
    char acComponentName[32];
}ST_OPTICAL_CONFIG;

/* Enum for slot index. */
typedef enum en_DSC_SLOT_IDX
{
    DSC_SLOT_NONE = -1,
    DSC_SLOT_IDX0 = 0,
    DSC_SLOT_IDX1,
    DSC_SLOT_IDX2,
    DSC_SLOT_IDX3,
    DSC_SLOT_NUM
} EN_DSC_SLOT_IDX;

extern int DSC_CP_GetInfoByName(char *pcCircuitPackName, ST_CIRCUIT_PACK *pstCircuitPackInfo);
extern int DSC_CP_GetAll(ST_CIRCUIT_PACK **ppCircuitPack, size_t *ptCount);
extern int DSC_CP_SetCpldVer(uint32_t u32CpldIndex, char *pcVersion);
extern int DSC_CP_GetComponents(char *pcCircuitPackName, ST_CIRCUIT_PACK_COMPONENT **ppstCpComponent, size_t *ptCompntCount);
extern int DSC_CP_SetComponentCurrentVer(char *pcCircuitPackName, char *pcComponentName, char *pcCurrentVer);
extern int DSC_CP_GetInfos(ST_CIRCUIT_PACK **ppstCircuitPackInfo, size_t *ptCircuitPackCount);

extern int DSC_CP_SetFanPhysical(int iIndex, ST_PHYSICAL_INFO *pstPhysical);
extern int DSC_CP_SetPowerPhysical(int iIndex, ST_PHYSICAL_INFO *pstPhysical);
extern int DSC_CP_SetFanSlot(int iIndex, char *pcSlotName);
extern int DSC_CP_SetPowerSlot(int iIndex, char *pcSlotName);
extern int DSC_CP_SetOpsPhysical(int iIndex, ST_PHYSICAL_INFO *pstPhysical);
extern int DSC_CP_SetBaPhysical(ST_PHYSICAL_INFO *pstPhysical);
extern int DSC_CP_SetCircuitPackPhysical(char *pcCircuitPack, ST_PHYSICAL_INFO *pstPhysical);

extern int DSC_CP_GetMainboardPorts(ST_CIRCUIT_PACK_PORT **ppstPorts, size_t *ptPortsCount);
extern int DSC_CP_SetModuleSlot(char *pcCpName, char *pcSlotName);
extern int DSC_CP_GetModuleSlot(char *pcCpName, char *pcSlotName);
/* extern int DSC_CP_GetInfoByName(char *pcName, ST_CIRCUIT_PACK *pstOutCpInfo); */
extern int DSC_CP_GetCurCpInSlot(char *pcSlotName, char *pcCpName);
#ifdef __cplusplus
}
#endif

#endif
