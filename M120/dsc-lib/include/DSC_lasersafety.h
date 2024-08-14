#ifndef _DSC_LASERSAFETY_H
#define _DSC_LASERSAFETY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

#define MODULE_LASER_SAFTY               "com-oplink-laser-safty"

#define LASERSAFETY_CFG_PARENT_XPATH_HEADER  "/" MODULE_LASER_SAFTY ":laser-safty"
#define LASERSAFETY_CFG_CONFIG                  LASERSAFETY_CFG_PARENT_XPATH_HEADER "/config"
#define LASERSAFETY_XPATH_CFG_ALL               LASERSAFETY_CFG_CONFIG "//*"

#define LASERSAFETY_CFG_SHUTTER_ON              LASERSAFETY_CFG_CONFIG SHUTTER_ON
#define LASERSAFETY_CFG_REMOTE_INTERLOCK_ENABLE LASERSAFETY_CFG_CONFIG REMOTE_INTERLOCK_ENABLE
#define LASERSAFETY_CFG_DUMMY_ADMIN_KEY_ENABLE  LASERSAFETY_CFG_CONFIG DUMMY_ADMIN_KEY_ENABLE
#define LASERSAFETY_CFG_PANEL_SAFE_CMPT_ENABLE  LASERSAFETY_CFG_CONFIG PANEL_SAFE_CMPT_ENABLE

#define LASERSAFETY_STATE                       LASERSAFETY_CFG_PARENT_XPATH_HEADER "/state"
#define LASERSAFETY_XPATH_STATE_ALL             LASERSAFETY_STATE "//*"
#define LASERSAFETY_STATE_INTER_LOCK_ON         LASERSAFETY_STATE INTER_LOCK_ON
#define LASERSAFETY_STATE_ADMIN_KEY_ON          LASERSAFETY_STATE ADMIN_KEY_ON
#define LASERSAFETY_STATE_LC_PRESENT            LASERSAFETY_STATE LC_PRESENT
#define LASERSAFETY_STATE_LC_DOOR_LOCK          LASERSAFETY_STATE LC_DOOR_LOCK
#define LASERSAFETY_STATE_SHUTTER_ON            LASERSAFETY_STATE SHUTTER_ON

typedef struct st_LASERSAFETY_STATE
{
    bool bInterLock;
    int iAdminKey;
    bool bHcfLcPresent;
    bool bHcfLcDoorLock;
    bool bShutterSta;
} LASERSAFETY_STATE_ST;

typedef struct st_LASERSAFETY_CFG
{
    bool bShutterCfg;
    bool bRemoteInterlockEnable;
    bool bDummyAdminKeyEnable;
    bool bPanelSafeCmptEnable;
} LASERSAFETY_CFG_ST;

extern int DSC_GetLaserSafetyCfg(LASERSAFETY_CFG_ST *pstLaserSafetyCfg, size_t size);
extern int DSC_GetLaserSafetyState(LASERSAFETY_STATE_ST *pstLaserSafetyState, size_t size);
extern int DSC_SetShutter(bool state);
extern int DSC_SetRemoteInterlockEnable(bool bEnable);
extern int DSC_SetDummyAdminKeyEnable(bool bEnable);
extern int DSC_SetPanelSafeCmptEnable(bool bEnable);

#ifdef __cplusplus
}
#endif

#endif

