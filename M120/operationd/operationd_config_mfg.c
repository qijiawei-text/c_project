/**********************************************************************************
   Copyright(C), 2022, OPLINK Tech.Co.,Ltd
   FileName    : operationd_config_mfg.c
   Description : slot config and mfg
   History     :
          [Author]          [Date]       [Version]    [Description]
   [1]    bengangs          2022/05/06   V1.0.0       Initial file.
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "DevSlotCApi.h"
#include "DevSlotData.h"
#include "DevBoardCApi.h"
#include "DevBoardData.h"
#include "DevOscData.h"
#include "DevEdfaCApi.h"
#include "DevEdfaData.h"
#include "DevOpsCApi.h"
#include "DevOcmCApi.h"
#include "DevTdcmCApi.h"
#include "DevOscCApi.h"
#include "DevOtdrCApi.h"
#include "DevOtdrData.h"
#include "DevExtendCApi.h"
#include "DevExtendData.h"
#include "DevWssCApi.h"
#include "DevMuxCApi.h"
#include "DevOcsCApi.h"

#include "sysrepo.h"
#include "util_inc.h"
#include "DSC_inc.h"

#include "oplkupgd.h"
#include "operationd_alm.h"
#include "operationd_upgd.h"
#include "operationd_config_mfg.h"
#include "operationd_queue.h"
#include "DevDbgCApi.h"



static int __ConfigEvoa(uint32_t ulDevType, char *pcComponentName);
static int __ConfigOsc(uint32_t ulDevType, char *pcComponentName);
static int __ConfigEdfa(uint32_t ulDevType, char *pcComponentName);

static int __ConfigOps(uint32_t ulDevType, char *pcComponentName);
static int __ConfigOcm(uint32_t ulDevType, char *pcComponentName);
/* static int __ConfigWss(uint32_t ulDevType, char *pcComponentName); */
static int __ConfigOtdr(uint32_t ulDevType, char *pcComponentName);
static int __ConfigOcs(uint32_t ulDevType, char *pcComponentName);

/* mfg struct */
typedef struct {
    uint32_t uiDevType;
    char acOrgComponentName[32];         /* in org-openroadm-device yang */
    char acOcComponentName[32];          /* in openconfig-platform yang */
    int (*pFuncComponentGetMfg)(uint32_t uiDevType, CMfg *pstMfg);
} ST_GET_COMPONENT_MFG;


#define DEF_OCP_COMPONENT_MFG_NUM_MAX                             (20)

static ST_GET_COMPONENT_MFG m_aastGetComponentMfg[EN_PRODUCT_TYPE_MAX][DEF_OCP_COMPONENT_MFG_NUM_MAX] = {
    {   /**********************  OLSP **********************/
        {DEV_TYPE_OPS, CP_COMPONENT_OPS, COMPONENT_KEY_OPS, OPS_GetMfg},

        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_EDFA, COMPONENT_KEY_AMP, EDFA_GetMfg},
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_PA, COMPONENT_KEY_PA, EDFA_GetMfg},          /* special handler for COMPONENT_KEY_PA */
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_BA, COMPONENT_KEY_BA, EDFA_GetMfg},          /* special handler for COMPONENT_KEY_BA */
        {DEV_TYPE_OCM, CP_COMPONENT_AMP_OCM, COMPONENT_KEY_OCM, OCM_GetMfg},
        {DEV_TYPE_OTDR, CP_COMPONENT_AMP_OTDR, COMPONENT_KEY_OTDR, OTDR_GetMfg},

        {DEV_TYPE_P_BA_PA, CP_COMPONENT_AMP_P_EDFA, COMPONENT_KEY_AMP_P, EDFA_GetMfg},
        {DEV_TYPE_P_BA_PA, CP_COMPONENT_AMP_P_PA, COMPONENT_KEY_PA_P, EDFA_GetMfg},      /* special handler for COMPONENT_KEY_PA_P */
        {DEV_TYPE_P_BA_PA, CP_COMPONENT_AMP_P_BA, COMPONENT_KEY_BA_P, EDFA_GetMfg},       /* special handler for COMPONENT_KEY_BA_P */
        {DEV_TYPE_P_OCM, CP_COMPONENT_AMP_P_OCM, COMPONENT_KEY_OCM_P, OCM_GetMfg},
        {DEV_TYPE_P_OTDR, CP_COMPONENT_AMP_P_OTDR, COMPONENT_KEY_OTDR_P, OTDR_GetMfg}
    }, {
        /**********************  NR1004 **********************/
        {DEV_TYPE_OPS, CP_COMPONENT_OPS, COMPONENT_KEY_OPS, OPS_GetMfg},
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_EDFA, COMPONENT_KEY_AMP, EDFA_GetMfg},
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_PA, COMPONENT_KEY_PA, EDFA_GetMfg},          /* special handler for COMPONENT_KEY_PA */
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_BA, COMPONENT_KEY_BA, EDFA_GetMfg},          /* special handler for COMPONENT_KEY_BA */
        {DEV_TYPE_OCM, CP_COMPONENT_AMP_OCM, COMPONENT_KEY_OCM, OCM_GetMfg},
        {DEV_TYPE_OTDR, CP_COMPONENT_AMP_OTDR, COMPONENT_KEY_OTDR, OTDR_GetMfg},

        {DEV_TYPE_P_BA_PA, CP_COMPONENT_AMP_P_EDFA, COMPONENT_KEY_AMP_P, EDFA_GetMfg},
        {DEV_TYPE_P_BA_PA, CP_COMPONENT_AMP_P_PA, COMPONENT_KEY_PA_P, EDFA_GetMfg},      /* special handler for COMPONENT_KEY_PA_P */
        {DEV_TYPE_P_BA_PA, CP_COMPONENT_AMP_P_BA, COMPONENT_KEY_BA_P, EDFA_GetMfg},      /* special handler for COMPONENT_KEY_BA_P */
        {DEV_TYPE_P_OCM, CP_COMPONENT_AMP_P_OCM, COMPONENT_KEY_OCM_P, OCM_GetMfg},
        {DEV_TYPE_P_OTDR, CP_COMPONENT_AMP_P_OTDR, COMPONENT_KEY_OTDR_P, OTDR_GetMfg}
    }, {
        /********************** NR1004-OPS-4 **********************/
        {DEV_TYPE_OPS, CP_COMPONENT_OPS, COMPONENT_KEY_OPS, OPS_GetMfg},

        {DEV_TYPE_OPS4A, "", "OPS-4-A", OPS_GetMfg},
        {DEV_TYPE_OPS4A, CP_COMPONENT_OPS "2", COMPONENT_KEY_OPS "2", OPS_GetMfg},
        {DEV_TYPE_OPS4A, CP_COMPONENT_OPS "3", COMPONENT_KEY_OPS "3", OPS_GetMfg},
        {DEV_TYPE_OPS4A, CP_COMPONENT_OPS "4", COMPONENT_KEY_OPS "4", OPS_GetMfg},
        {DEV_TYPE_OPS4A, CP_COMPONENT_OPS "5", COMPONENT_KEY_OPS "5", OPS_GetMfg},

        {DEV_TYPE_OPS4B, "", "OPS-4-B", OPS_GetMfg},
        {DEV_TYPE_OPS4B, CP_COMPONENT_OPS "6", COMPONENT_KEY_OPS "6", OPS_GetMfg},
        {DEV_TYPE_OPS4B, CP_COMPONENT_OPS "7", COMPONENT_KEY_OPS "7", OPS_GetMfg},
        {DEV_TYPE_OPS4B, CP_COMPONENT_OPS "8", COMPONENT_KEY_OPS "8", OPS_GetMfg},
        {DEV_TYPE_OPS4B, CP_COMPONENT_OPS "9", COMPONENT_KEY_OPS "9", OPS_GetMfg}
    }, {
        /**********************  HCF **********************/
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_EDFA, COMPONENT_KEY_AMP, EDFA_GetMfg},
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_PA, COMPONENT_KEY_PA, EDFA_GetMfg},          /* special handler for COMPONENT_KEY_PA */
        {DEV_TYPE_BA_PA, CP_COMPONENT_AMP_BA, COMPONENT_KEY_BA, EDFA_GetMfg},          /* special handler for COMPONENT_KEY_BA */
        {DEV_TYPE_OCM, CP_COMPONENT_AMP_OCM, COMPONENT_KEY_OCM, OCM_GetMfg},
        {DEV_TYPE_OTDR, CP_COMPONENT_AMP_OTDR, COMPONENT_KEY_OTDR, OTDR_GetMfg},
    }, {
        /*****************  Optical cross connect swicth *****************/
        {DEV_TYPE_OCS, CP_COMPONENT_OCS_OCS, DEF_OCP_COMPONENT_KEY_LC_OCS, OCS_GetMfg}
    }
};

/* conf struct */
typedef struct {
    uint32_t uiSlotIdx;           /* The value is equal to the slot ID of hal */
    uint32_t uiCardType;          /* refer to ECardType */
    uint32_t uiDevType;           /* refer to EDevType */
    char acComponentName[32];     /* in optical yang */
    int (*pFuncComponentSetConf)(uint32_t uiDevType, char *pcComponentName);
} ST_SET_COMPONENT_CONF;


#define DEF_COMPONENT_CFG_NUM_MAX                             (20)

static ST_SET_COMPONENT_CONF m_astSetComponentConfig[EN_PRODUCT_TYPE_MAX][DEF_COMPONENT_CFG_NUM_MAX] = {
    { /********************* OLSP *********************/
        {DSC_SLOT_IDX0, CARD_TYPE_OPS, DEV_TYPE_OPS, OPS_OLP_KEY, __ConfigOps},      /* OPS card */

        /* AMP card */
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_PA, AMP_PA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_BA, AMP_BA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OSC, AMP_OSC_KEY, __ConfigOsc},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_BA, AMP_BA_VOA_KEY, __ConfigEvoa},
        /* {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_PA, AMP_PA_VOA_KEY, __ConfigEvoa}, */
        /*{DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OSC, AMP_OSC_VOA_KEY, __ConfigEvoa},*/  /* OSC VOA is not configurable*/
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OCM, AMP_OCM_KEY, __ConfigOcm},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OTDR, AMP_OTDR_KEY, __ConfigOtdr},

        /* AMP-P card */
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_PA, AMP_P_PA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_BA, AMP_P_BA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OSC, AMP_P_OSC_KEY, __ConfigOsc},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_BA, AMP_P_BA_VOA_KEY, __ConfigEvoa},
        /* {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_PA, AMP_P_PA_VOA_KEY, __ConfigEvoa}, */
        /*{DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OSC, AMP_P_OSC_VOA_KEY, __ConfigEvoa},*/ /* OSC VOA is not configurable*/
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OCM, AMP_P_OCM_KEY, __ConfigOcm},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OTDR, AMP_P_OTDR_KEY, __ConfigOtdr}
    }, { /**********************  NR1004 **********************/
        {DSC_SLOT_IDX0, CARD_TYPE_OPS, DEV_TYPE_OPS, OPS_OLP_KEY, __ConfigOps},     /* OPS card */

        /* AMP card */
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_PA, AMP_PA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_BA, AMP_BA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OSC, AMP_OSC_KEY, __ConfigOsc},
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_BA, AMP_BA_VOA_KEY, __ConfigEvoa},
        /* {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_PA, AMP_PA_VOA_KEY, __ConfigEvoa}, */
        /*{DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OSC, AMP_OSC_VOA_KEY, __ConfigEvoa},*/  /* OSC VOA is not configurable*/
        {DSC_SLOT_IDX1, CARD_TYPE_OTA, DEV_TYPE_OCM, AMP_OCM_KEY, __ConfigOcm},

        /* AMP-P card */
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_PA, AMP_P_PA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_BA, AMP_P_BA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OSC, AMP_P_OSC_KEY, __ConfigOsc},
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_BA, AMP_P_BA_VOA_KEY, __ConfigEvoa},
        /* {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_PA, AMP_P_PA_VOA_KEY, __ConfigEvoa}, */
        /*{DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OSC, AMP_P_OSC_VOA_KEY, __ConfigEvoa},*/ /* OSC VOA is not configurable*/
        {DSC_SLOT_IDX2, CARD_TYPE_OTA, DEV_TYPE_P_OCM, AMP_P_OCM_KEY, __ConfigOcm},
    }, {
        /********************** NR1004-OPS-4 **********************/
        {DSC_SLOT_IDX0, CARD_TYPE_OPS, DEV_TYPE_OPS, OPS_OLP_KEY "1", __ConfigOps},      /* OPS card */

        {DSC_SLOT_IDX1, CARD_TYPE_OPS, DEV_TYPE_OPS4A, OPS_OLP_KEY "2", __ConfigOps},          /* OPS-4-A card */
        {DSC_SLOT_IDX1, CARD_TYPE_OPS, DEV_TYPE_OPS4A, OPS_OLP_KEY "3", __ConfigOps},
        {DSC_SLOT_IDX1, CARD_TYPE_OPS, DEV_TYPE_OPS4A, OPS_OLP_KEY "4", __ConfigOps},
        {DSC_SLOT_IDX1, CARD_TYPE_OPS, DEV_TYPE_OPS4A, OPS_OLP_KEY "5", __ConfigOps},

        {DSC_SLOT_IDX2, CARD_TYPE_OPS, DEV_TYPE_OPS4B, OPS_OLP_KEY "6", __ConfigOps},
        {DSC_SLOT_IDX2, CARD_TYPE_OPS, DEV_TYPE_OPS4B, OPS_OLP_KEY "7", __ConfigOps},
        {DSC_SLOT_IDX2, CARD_TYPE_OPS, DEV_TYPE_OPS4B, OPS_OLP_KEY "8", __ConfigOps},
        {DSC_SLOT_IDX2, CARD_TYPE_OPS, DEV_TYPE_OPS4B, OPS_OLP_KEY "9", __ConfigOps},
    }, {
        /************************** HCF ***************************/
        /* AMP card */
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_PA, AMP_PA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_BA, AMP_BA_KEY, __ConfigEdfa},
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_OSC, AMP_OSC_KEY, __ConfigOsc},
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_BA, AMP_BA_VOA_KEY, __ConfigEvoa},
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_PA, AMP_PA_VOA_KEY, __ConfigEvoa},
        /* {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_OSC, AMP_OSC_VOA_KEY, __ConfigEvoa}, */
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_OCM, AMP_OCM_KEY, __ConfigOcm},
        {DSC_SLOT_IDX0, CARD_TYPE_OTA, DEV_TYPE_OTDR, AMP_OTDR_KEY, __ConfigOtdr},
    }, {
        /*****************  OCS  ******************/
        {DSC_SLOT_IDX0, CARD_TYPE_OCS, DEV_TYPE_OCS, OCS_OCS_KEY, __ConfigOcs},
    }


};

static ST_SHELF m_stShelfInfo;
static ST_CIRCUIT_PACK m_astCpInfo[CIRCUIT_PACK_NUM_MAX] = {0};

static int
__GetComponentMfgList(ST_GET_COMPONENT_MFG **ppstGetCompMfgList, size_t *pnGetCompMfgNum)
{
    PRODUCT_TYPE_EN enProductType = EN_PRODUCT_TYPE_MAX;
    ST_GET_COMPONENT_MFG *pstElem = NULL;
    size_t i = 0;

    enProductType = BOARD_GetProductType();

    for (i = 0; i < DEF_COMPONENT_CFG_NUM_MAX; i++) {
        pstElem = m_aastGetComponentMfg[enProductType] + i;
        if (NULL == pstElem->pFuncComponentGetMfg) {
            break;
        }
    }

    *ppstGetCompMfgList = m_aastGetComponentMfg[enProductType];
    *pnGetCompMfgNum = i;

    return OPLK_OK;
}

static int
__GetComponentConfigList(ST_SET_COMPONENT_CONF **ppstSetCompConfigList, size_t *pnSetCompConfigNum)
{
    PRODUCT_TYPE_EN enProductType = EN_PRODUCT_TYPE_MAX;
    ST_SET_COMPONENT_CONF *pstElem = NULL;
    size_t i = 0;

    enProductType = BOARD_GetProductType();

    for (i = 0; i < DEF_COMPONENT_CFG_NUM_MAX; i++) {
        pstElem = m_astSetComponentConfig[enProductType] + i;
        if (NULL == pstElem->pFuncComponentSetConf) {
            break;
        }
    }

    *ppstSetCompConfigList = m_astSetComponentConfig[enProductType];
    *pnSetCompConfigNum = i;

    return OPLK_OK;
}


void
PrintShelfInfo(ST_SHELF *pstShelfInfo)
{
    int i = 0;
    int j = 0;

    LOG_WRITE(EN_LOG_NOTICE, "=====shelf-name======  %s", pstShelfInfo->acName);

    for (i = 0; i < SHELF_SLOT_NUM_MAX; i++) {
        if (strlen(pstShelfInfo->astSlot[i].acName) != 0) {
            LOG_WRITE(EN_LOG_NOTICE, "=====slot-name======  %s", pstShelfInfo->astSlot[i].acName);
            LOG_WRITE(EN_LOG_NOTICE, "=====slot-acLabel======  %s", pstShelfInfo->astSlot[i].acLabel);
            for (j = 0; j < PROV_CIRCUITPACK_NUM_MAX; j++) {
                if (0 != strlen(pstShelfInfo->astSlot[i].aacRefProvisionedCircuitPack[j])) {
                    LOG_WRITE(EN_LOG_NOTICE, "=====slot-aacRefProvisionedCircuitPack======  %s", pstShelfInfo->astSlot[i].aacRefProvisionedCircuitPack[j]);
                }
            }
            LOG_WRITE(EN_LOG_NOTICE, "=====slot-acSlotStatus======  %s", pstShelfInfo->astSlot[i].acSlotStatus);
        } else {
            break;
        }
    }

    return;
}

void
PrintCpInfo(ST_CIRCUIT_PACK *pstCpInfo)
{
    int i = 0;
    int j = 0;

    for (i = 0; i < CIRCUIT_PACK_NUM_MAX; i++) {
        if (strlen(pstCpInfo[i].acName) != 0) {
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-name======  %s", pstCpInfo[i].acName);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-vendor======  %s", pstCpInfo[i].stPhysical.stNode.acVendor);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-model======  %s", pstCpInfo[i].stPhysical.stNode.acModel);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-sn======  %s", pstCpInfo[i].stPhysical.stNode.acSN);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-pn======  %s", pstCpInfo[i].stPhysical.stCommon.acProductCode);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-MfgDate======  %s", pstCpInfo[i].stPhysical.stCommon.acMfgDate);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-Clei======  %s", pstCpInfo[i].stPhysical.stCommon.acClei);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-HwVer======  %s", pstCpInfo[i].stPhysical.stCommon.acHwVer);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-OperationalState======  %d", pstCpInfo[i].stPhysical.stCommon.enOperationalState);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-RefShelfName======  %s", pstCpInfo[i].acRefShelfName);
            LOG_WRITE(EN_LOG_NOTICE, "=====cp-RefSlotName======  %s", pstCpInfo[i].acRefSlotName);

            for (j = 0; j < CP_COMPONENT_NUM_MAX; j++) {
                if (strlen(pstCpInfo[i].astComponent[j].acName) != 0) {
                    LOG_WRITE(EN_LOG_NOTICE, "=====component-name======  %s", pstCpInfo[i].astComponent[j].acName);
                    LOG_WRITE(EN_LOG_NOTICE, "=====component-bBoot======  %d", pstCpInfo[i].astComponent[j].bBoot);
                    LOG_WRITE(EN_LOG_NOTICE, "=====component-acCurrentVer======  %s", pstCpInfo[i].astComponent[j].acCurrentVer);
                } else {
                    break;
                }
            }
        } else {
            break;
        }
    }

    return;
}



static int
__InitShelfCpInfo(void)
{
    int rc = SR_ERR_OK;
    ST_SHELF stShelf;
    ST_SLOT *pstSlot = NULL;
    size_t nSlotNum = 0;
    ST_CIRCUIT_PACK *pstCircuitPack = NULL;
    size_t nCircuitPackCount = 0;

    memset(&m_stShelfInfo, 0, sizeof(m_stShelfInfo));
    memset(m_astCpInfo, 0, sizeof(m_astCpInfo));

    memset(&stShelf, 0, sizeof(stShelf));
    rc = DSC_SHELF_GetInfo(&stShelf);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SHELF_GetInfo : %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_SHELF_GetSlots(&pstSlot, &nSlotNum);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SHELF_GetSlots : %s", sr_strerror(rc));
        goto ERR_LABEL;
    }
    memcpy(stShelf.astSlot, pstSlot, sizeof(ST_SLOT) * nSlotNum);

    rc = DSC_CP_GetAll(&pstCircuitPack, &nCircuitPackCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_GetAll : %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    memcpy(&m_stShelfInfo, &stShelf, sizeof(ST_SHELF));
    memcpy(m_astCpInfo, pstCircuitPack, sizeof(ST_CIRCUIT_PACK) * nCircuitPackCount);

    /* PrintShelfInfo(&m_stShelfInfo); */
    /* PrintCpInfo(m_astCpInfo); */

ERR_LABEL:
    DEF_SAFE_FREE(pstSlot);
    DEF_SAFE_FREE(pstCircuitPack);

    return rc;
}

static int
__InitApsd()
{
    int rc = OPLK_OK;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    rc = EDFA_SetApsdEnable(DEV_TYPE_PA, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error EDFA_SetApsdEnable(%d), rc = %d", DEV_TYPE_PA, rc);
    }

    rc = EDFA_SetApsdEnable(DEV_TYPE_P_PA, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error EDFA_SetApsdEnable(%d), rc = %d", DEV_TYPE_P_PA, rc);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);

    return OPLK_OK;
}

static int
__InitOsc()
{
    int rc = OPLK_OK;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    /* NOTE: Every time the system is started, the OSC must be forced to enable */

    rc = OSC_SetEnable(DEV_TYPE_OSC, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error OSC_SetEnable(%d), rc = %d", DEV_TYPE_OSC, rc);
    }

    rc = DSC_SetOscOutputEnable(OSC_CFG_KEY, true);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SetOscOutputEnable(%s), rc = %d", OSC_CFG_KEY, rc);
    }

    rc = OSC_SetEnable(DEV_TYPE_P_OSC, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error OSC_SetEnable(%d), rc = %d", DEV_TYPE_P_OSC, rc);
    }

    rc = DSC_SetOscOutputEnable(OSC_P_CFG_KEY, true);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SetOscOutputEnable(%s), rc = %d", OSC_P_CFG_KEY, rc);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);

    return OPLK_OK;
}

static int
__InitApr()
{
    int rc = OPLK_OK;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    /* NOTE: Every time the system is started, the APR must be forced to enable */

    rc = EDFA_SetAprEnable(DEV_TYPE_BA, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error EDFA_SetAprEnable(%d), rc = %d", DEV_TYPE_BA, rc);
    }

    rc = DSC_SetEdfaAprEnable(EDFA_BA_CFG_KEY, true);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SetEdfaAprEnable(%s), rc = %d", EDFA_BA_CFG_KEY, rc);
    }

    rc = EDFA_SetAprEnable(DEV_TYPE_PA, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error EDFA_SetAprEnable(%d), rc = %d", DEV_TYPE_PA, rc);
    }

    rc = DSC_SetEdfaAprEnable(EDFA_PA_CFG_KEY, true);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SetEdfaAprEnable(%s), rc = %d", EDFA_PA_CFG_KEY, rc);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s --", __func__);

    return OPLK_OK;
}

#if 0
static int
__GetSlotInfoByLabel(int iSlotId, ST_SLOT *pstSlot)
{
    char acSlotLabel[4] = {0};
    int i = 0;

    memset(acSlotLabel, 0, sizeof(acSlotLabel));
    sprintf(acSlotLabel, "%d", iSlotId);

    for (i = 0; i < SHELF_SLOT_NUM_MAX; i++) {
        if (0 == strcmp(acSlotLabel, m_stShelfInfo.astSlot[i].acLabel)) {
            memcpy(pstSlot, &m_stShelfInfo.astSlot[i], sizeof(ST_SLOT));
            return OPLK_OK;
        }
    }

    return OPLK_ERR;
}
#endif

static int
__GetSlotNameBySlotId(uint32_t lSlotId, char *pcSlotName)
{
    char acSlotLabel[8] = {0};
    int i = 0;

    memset(acSlotLabel, 0, sizeof(acSlotLabel));
    snprintf(acSlotLabel, sizeof(acSlotLabel), "%d", lSlotId);

    for (i = 0; i < SHELF_SLOT_NUM_MAX; i++) {
        if (0 == strcmp(acSlotLabel, m_stShelfInfo.astSlot[i].acLabel)) {
            LOG_WRITE(EN_LOG_NOTICE, "acLable: %s, acSlotLabel: %s", m_stShelfInfo.astSlot[i].acLabel, acSlotLabel);
            strcpy(pcSlotName, m_stShelfInfo.astSlot[i].acName);
            if (0 != strlen(pcSlotName)) {
                return OPLK_OK;
            } else {
                return OPLK_ERR;
            }
        }
    }

    LOG_WRITE(EN_LOG_ERROR, "error __GetSlotNameBySlotId can not find slot id =  %d", lSlotId);
    return OPLK_ERR;
}



static int
__SlotStateEnumToStr(int iSlotState, char *pcStateStr)
{
    int rc = OPLK_OK;

    if (NULL == pcStateStr) {
        LOG_WRITE(EN_LOG_ERROR, "input null!");
        return OPLK_ERR;
    }

    switch (iSlotState) {
    case SLOT_STATE_ONLINE:
        strcpy(pcStateStr, SLOT_STATUS_INSTALL_MATCH_STR);
        break;
    case SLOT_STATE_OFFLINE:
        strcpy(pcStateStr, SLOT_STATUS_EMPTY_STR);
        break;
    case SLOT_STATE_MISMATCH:
        strcpy(pcStateStr, SLOT_STATUS_INSTALL_MISMATCH_STR);
        break;
    default:
        rc = OPLK_ERR;
        break;
    }

    return rc;
}
static int
__PutCpMfgInfoToOrg(char *pcCircuitPack, CMfg *pstModMfg)
{
    int rc = SR_ERR_OK;
    ST_PHYSICAL_INFO stPhysical;

    if (NULL == pcCircuitPack || NULL == pstModMfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(&stPhysical, 0x00, sizeof(stPhysical));

    strncpy(stPhysical.stNode.acVendor, pstModMfg->acManufacturer, sizeof(stPhysical.stNode.acVendor) - 1);
    strncpy(stPhysical.stNode.acModel, pstModMfg->acProductName, sizeof(stPhysical.stNode.acModel) - 1);
    strncpy(stPhysical.stNode.acSN, pstModMfg->acSN, sizeof(stPhysical.stNode.acSN) - 1);
    strncpy(stPhysical.stCommon.acProductCode, pstModMfg->acPN, sizeof(stPhysical.stCommon.acProductCode) - 1);
    strncpy(stPhysical.stCommon.acHwVer, pstModMfg->acHwVer, sizeof(stPhysical.stCommon.acHwVer) - 1);
    strncpy(stPhysical.stCommon.acMfgDate, pstModMfg->acMfgDate, sizeof(stPhysical.stCommon.acMfgDate) - 1);
    strncpy(stPhysical.stFujitsu.acUnitName, pstModMfg->acUnitName, sizeof(stPhysical.stFujitsu.acUnitName) - 1);
    strncpy(stPhysical.stFujitsu.acFcNumber, pstModMfg->acFCPN, sizeof(stPhysical.stFujitsu.acFcNumber) - 1);
    strncpy(stPhysical.stFujitsu.acPiuIssueNumber, pstModMfg->acFCIss, sizeof(stPhysical.stFujitsu.acPiuIssueNumber) - 1);
    strncpy(stPhysical.stFujitsu.acFujitsuPartNumber, pstModMfg->acFujitsuPN, sizeof(stPhysical.stFujitsu.acFujitsuPartNumber) - 1);


    rc = DSC_CP_SetCircuitPackPhysical(pcCircuitPack, &stPhysical);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set %s physical : %s", pcCircuitPack, sr_strerror(rc));
        goto cleanup;
    }

cleanup:

    return rc;
}

/* 20201125,2020/11/25,2020.11.25 2020-11-25T00:00:00Z to 2020-11-25*/
static int
__FormartMfgDate(char *pSrc, char *pDst)
{
    int rc = SR_ERR_OK;
    char acBuf[64] = {0};
    int i = 0;
    int j = 0;

    if (NULL == pSrc || NULL == pDst) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    while (pSrc[i] != '\0') {
        if (isdigit(pSrc[i])) {
            acBuf[j] = pSrc[i];
            j++;
            if (j > sizeof(acBuf) - 1) {
                break;
            }
        }
        i++;
        if ( i > sizeof(acBuf) - 1) {
            break;
        }
    }

    if (j < 8 ) {
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memcpy(pDst, acBuf, 4);
    pDst[4] = '-';
    memcpy(pDst + 5, acBuf + 4, 2);
    pDst[7] = '-';
    memcpy(pDst + 8, acBuf + 6, 2);

cleanup:

    return rc;

}

static int
__PutComponentMfgInfoToOc(char *pcComponent, CMfg *pstModMfg)
{
    int rc = SR_ERR_OK;
    ST_OC_COMPONENT_STATE stComponentSate;
    char acMfgDateOpenConfig[64] = {0};

    if (NULL == pcComponent || NULL == pstModMfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }

    memset(&stComponentSate, 0x00, sizeof(stComponentSate));

    strncpy(stComponentSate.acMfgName, pstModMfg->acManufacturer, sizeof(stComponentSate.acMfgName) - 1);
    /* strncpy(stComponentSate.acDescription, pstModMfg->acProductName, sizeof(stComponentSate.acDescription) - 1); */
    strncpy(stComponentSate.acSerialNo, pstModMfg->acSN, sizeof(stComponentSate.acSerialNo) - 1);
    strncpy(stComponentSate.acPartNo, pstModMfg->acPN, sizeof(stComponentSate.acPartNo) - 1);
    strncpy(stComponentSate.acHardwareVersion, pstModMfg->acHwVer, sizeof(stComponentSate.acHardwareVersion) - 1);

    strncpy(stComponentSate.acUnitName, pstModMfg->acUnitName, sizeof(stComponentSate.acUnitName) - 1);
    strncpy(stComponentSate.acFcNumber, pstModMfg->acFCPN, sizeof(stComponentSate.acFcNumber) - 1);
    strncpy(stComponentSate.acPiuIssueNumber, pstModMfg->acFCIss, sizeof(stComponentSate.acPiuIssueNumber) - 1);
#ifdef _NR1004_
    strncpy(stComponentSate.acFujitsuPartNumber, pstModMfg->acFujitsuPN, sizeof(stComponentSate.acFujitsuPartNumber) - 1);
    strncpy(stComponentSate.acFjtVendorName, DEF_FJT_VENDOR_NAME, sizeof(stComponentSate.acFjtVendorName) - 1);
    strncpy(stComponentSate.acFjtSn, pstModMfg->acFjtSn, sizeof(stComponentSate.acFjtSn) - 1);
#endif
    strncpy(stComponentSate.acClei, pstModMfg->acCleiCode, sizeof(stComponentSate.acClei) - 1);
    strncpy(stComponentSate.acModel, pstModMfg->acProductName, sizeof(stComponentSate.acModel) - 1);
    strncpy(stComponentSate.acConfigType, pstModMfg->acConfigType, sizeof(stComponentSate.acConfigType) - 1);

    if (0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_SCC)) {
        strncpy(stComponentSate.acFpgaVer, pstModMfg->acFpga1Ver, sizeof(stComponentSate.acFpgaVer) - 1);
        strncpy(stComponentSate.acCpldVer, pstModMfg->acCpld1Ver, sizeof(stComponentSate.acCpldVer) - 1);
    } else if (0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_AMP) ||
               (0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_AMP_P))) {
        /*strncpy(stComponentSate.acCpldVer, pstModMfg->acFpgaRunVer, sizeof(stComponentSate.acFpgaVer) - 1); */
    } else if (0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_OPS)) {
        strncpy(stComponentSate.acCpldVer, pstModMfg->acFpgaRunVer, sizeof(stComponentSate.acFpgaVer) - 1);
    } else if ((0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_OPS_4_A)) ||
               (0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_OPS_4_B))) {
        strncpy(stComponentSate.acFpgaVer, pstModMfg->acFpgaRunVer, sizeof(stComponentSate.acFpgaVer) - 1);
    } else if (0 == strcmp(pcComponent, AMP_PA_KEY) ||
               0 == strcmp(pcComponent, AMP_BA_KEY) ||
               0 == strcmp(pcComponent, AMP_P_PA_KEY) ||
               0 == strcmp(pcComponent, AMP_P_BA_KEY)) {
        strncpy(stComponentSate.acFpgaVer, pstModMfg->acFpgaRunVer, sizeof(stComponentSate.acFpgaVer) - 1);
        strncpy(stComponentSate.acCpldVer, pstModMfg->acCpld1Ver, sizeof(stComponentSate.acFpgaVer) - 1);
    }

    /* mfg-date format: YYYY-MM-DD */
    if (strlen(pstModMfg->acMfgDate) > 0) {
        memset(&acMfgDateOpenConfig, 0, sizeof(acMfgDateOpenConfig));
        rc = __FormartMfgDate(pstModMfg->acMfgDate, acMfgDateOpenConfig);
        if (SR_ERR_OK == rc) {
            strncpy(stComponentSate.acMfgDate, acMfgDateOpenConfig, sizeof(stComponentSate.acMfgDate) - 1);
        }
        LOG_WRITE(EN_LOG_NOTICE, "component : %s, acMfgDate:%s, rc:%d", pcComponent, pstModMfg->acMfgDate, rc);
    }

    LOG_WRITE(EN_LOG_NOTICE, "acFwVer:%s, acSwVer:%s", pstModMfg->acFwVer, pstModMfg->acSwVer);

    /* AMP and AMP-P card do NOT have FW and SW version */
    if (0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_AMP) ||
        0 == strcmp(pcComponent, DEF_OCP_COMPONENT_KEY_AMP_P)) {
    } else {
        strncpy(stComponentSate.acFirmwareVersion, pstModMfg->acFwVer, sizeof(stComponentSate.acFirmwareVersion) - 1);
        strncpy(stComponentSate.acSoftwareVersion, pstModMfg->acSwVer, sizeof(stComponentSate.acSoftwareVersion) - 1);
    }

    rc = DSC_OCP_SetComponentMfgInfo(pcComponent, &stComponentSate);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set %s component : %s", pcComponent, sr_strerror(rc));
        goto cleanup;
    }

cleanup:

    return rc;
}

static int
__GetBackupBdlVer(char *pcBdlVer, size_t nSize)
{
#define  BUNDLE_FILE_BACKUP    "/home/oplink/bundlebackup/FirmwareBundle_pre"
    ST_UPGD_BDL_HEAD stPreBdlHead;
    char acFileName[DEF_BUF_SIZE_128] = {0};
    uint32_t ulFileLen = 0;
    FILE *pfFile = NULL;
    int rc = OPLK_OK;

    snprintf(acFileName, sizeof(acFileName), "%s", BUNDLE_FILE_BACKUP);

    ulFileLen = sizeof(stPreBdlHead);
    memset(&stPreBdlHead, 0, ulFileLen);

    if (access(acFileName, F_OK) == 0) {
        if (NULL != (pfFile = fopen(acFileName, "r"))) {
            if (ulFileLen != fread((uint8_t *)&stPreBdlHead, sizeof(uint8_t), ulFileLen, pfFile)) {
                LOG_WRITE(EN_LOG_ERROR, "fread is error");
                rc = OPLK_ERR;
                goto END_LABEL;
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "fopen is error, file name is %s", acFileName);
            rc = OPLK_ERR;
            goto END_LABEL;
        }
        DEF_SAFE_FCLOSE(pfFile);
    }

    if (0 != strlen(stPreBdlHead.acBdlVer)) {
        snprintf(pcBdlVer, nSize, "%s", stPreBdlHead.acBdlVer);
    } else {
        snprintf(pcBdlVer, nSize, "%s", "X.YY.ZZZZ");
    }

END_LABEL:
    return rc;
}

static int
__PutChassisMfgToOrgAndOc(void)
{
    int rc = SR_ERR_OK;
    CMfg stBoardModMfg;
    ST_UPGD_INFO stUpgdInfo;
    char acBackupBdlVer[DEF_BUF_SIZE_32] = {0};
    char acModel[DEF_BUF_SIZE_32] = {0};

    /* write device info : bundle version */
    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    UPGD_GetInfo(&stUpgdInfo);
    rc = DSC_SetNodeSwVersion(stUpgdInfo.acBdlVerRun, strlen(stUpgdInfo.acBdlVerRun) + 1);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set node sw version %s", stUpgdInfo.acBdlVerRun);
    }

    rc = __GetBackupBdlVer(acBackupBdlVer, sizeof(acBackupBdlVer));
    if (OPLK_OK == rc) {
        rc = DSC_SetBuildSwVersion(acBackupBdlVer, strlen(acBackupBdlVer) + 1);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error by set build sw version %s", acBackupBdlVer);
        }
        LOG_WRITE(EN_LOG_NOTICE, "build sw version: %s", acBackupBdlVer);
    }

    /* write cp's administrative-state */
    rc = DSC_CP_SetAdminState(CIRCUIT_PACK_CHASSIS, EQPT_ADMIN_STATES_INSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetAdminState acCpName = %s", CIRCUIT_PACK_CHASSIS);
    }

    /* write cp's operational-state */
    rc = DSC_CP_SetOpState(CIRCUIT_PACK_CHASSIS, STATE_INSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetOpState acCpName = %s", CIRCUIT_PACK_CHASSIS);
    }

    /* read cp(cp-chassis)'mfg from rpc eeprom*/
    memset(&stBoardModMfg, 0x00, sizeof(stBoardModMfg));
    rc = BOARD_GetChassisMfg(&stBoardModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get board mfg info: %d", rc);
        goto cleanup;
    }

    snprintf(stBoardModMfg.acFwVer, sizeof(stBoardModMfg.acFwVer), "%s", stUpgdInfo.acBdlVerRun);
    snprintf(stBoardModMfg.acSwVer, sizeof(stBoardModMfg.acSwVer), "%s", stUpgdInfo.acBdlVerRun);

    LOG_WRITE(EN_LOG_NOTICE, "[chassis]vender: %s, model: %s, sn: %s, pn: %s", stBoardModMfg.acManufacturer, stBoardModMfg.acProductName,
              stBoardModMfg.acSN, stBoardModMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "[chassis]hardware-ver: %s, clei: %s, mfg-date: %s", stBoardModMfg.acHwVer, stBoardModMfg.acCleiCode,
              stBoardModMfg.acMfgDate);
    /* push mfg info to org-openroadm-device operational datastore */
    rc = __PutCpMfgInfoToOrg(CIRCUIT_PACK_CHASSIS, &stBoardModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by put mfginfo: %s, %s", CIRCUIT_PACK_CHASSIS, sr_strerror(rc));
        /* goto cleanup; */
    }

    /*
       Note: Delete the hard code for production type.
            Read chassis model from yang startup file for all types of production.
            This is a platform general code.
       Author: Maolu, Date: 12/17/2023
     */
    if (get_string_item(DSC_GetStartupSession(), COD_XPATH_MODEL, acModel, sizeof(acModel)) != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to get product model. %s", COD_XPATH_MODEL);
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "Product model: %s", acModel);

    rc = __PutComponentMfgInfoToOc(acModel, &stBoardModMfg);

    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by put mfginfo: %s, %s", acModel, sr_strerror(rc));
    }

cleanup:
    return rc;
}

static int
__PutSccMfgToOrgAndOc(void)
{
    int rc = SR_ERR_OK;
    CMfg stModMfg;
    ST_CIRCUIT_PACK_COMPONENT *pstCpComponent = NULL;
    ST_UPGD_INFO stUpgdInfo;
    size_t nCompntCount = 0;

    /* write cp's administrative-state */
    rc = DSC_CP_SetAdminState(CIRCUIT_PACK_SCC, EQPT_ADMIN_STATES_INSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetAdminState acCpName = %s", CIRCUIT_PACK_SCC);
    }

    /* write cp's operational-state */
    rc = DSC_CP_SetOpState(CIRCUIT_PACK_SCC, STATE_INSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetOpState acCpName = %s", CIRCUIT_PACK_SCC);
    }

    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    UPGD_GetInfo(&stUpgdInfo);

    memset(&stModMfg, 0x00, sizeof(stModMfg));
    rc = BOARD_GetSccMfg(&stModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get scc mfginfo: %d", rc);
        goto cleanup;
    }

    /* cpld/fpga version are same between SCC mfg and chassis mfg */
#if 0
    CMfg stBoardModMfg;
    memset(&stBoardModMfg, 0x00, sizeof(stBoardModMfg));
    rc = BOARD_GetChassisMfg(&stBoardModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get board mfg info: %d", rc);
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "[chassis]acFpga1Ver: %s, acCpld1Ver: %s, acAppVerRun: %s", stBoardModMfg.acFpga1Ver, stBoardModMfg.acCpld1Ver,
              stUpgdInfo.acAppVerRun);
    snprintf(stModMfg.acFpga1Ver, sizeof(stModMfg.acFpga1Ver), "%s", stBoardModMfg.acFpga1Ver);
    snprintf(stModMfg.acCpld1Ver, sizeof(stModMfg.acCpld1Ver), "%s", stBoardModMfg.acCpld1Ver);
#endif

    snprintf(stModMfg.acFwVer, sizeof(stModMfg.acFwVer), "%s", stUpgdInfo.acBdlVerRun);
    snprintf(stModMfg.acSwVer, sizeof(stModMfg.acSwVer), "%s", stUpgdInfo.acBdlVerRun);

    LOG_WRITE(EN_LOG_NOTICE, "[scc]acFpga1Ver: %s, acCpld1Ver: %s, acAppVerRun: %s", stModMfg.acFpga1Ver, stModMfg.acCpld1Ver,
              stUpgdInfo.acAppVerRun);
    LOG_WRITE(EN_LOG_NOTICE, "[scc]acFwVer: %s, acSwVer: %s", stModMfg.acFwVer, stModMfg.acSwVer);

    LOG_WRITE(EN_LOG_NOTICE, "[scc]vender: %s, model: %s, sn: %s, pn: %s", stModMfg.acManufacturer, stModMfg.acProductName,
              stModMfg.acSN, stModMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "[scc]hardware-ver: %s, clei: %s, mfg-date: %s", stModMfg.acHwVer, stModMfg.acCleiCode,
              stModMfg.acMfgDate);

    /* push mfg info to org-openroadm-device operational datastore */
    rc = __PutCpMfgInfoToOrg(CIRCUIT_PACK_SCC, &stModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by put mfginfo: %s, %s", CIRCUIT_PACK_SCC, sr_strerror(rc));
    }

    /* push components mfg info to org-openroadm-device operational datastore */
    rc = DSC_CP_GetComponents(CIRCUIT_PACK_SCC, &pstCpComponent, &nCompntCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get %s component : %s", CIRCUIT_PACK_SCC, sr_strerror(rc));
        goto cleanup;
    } else {
        for (size_t i = 0; i < nCompntCount; i++) {
            if (0 == strcasecmp(CP_COMPONENT_SCC_FPGA, pstCpComponent[i].acName)) {
                rc = DSC_CP_SetComponentCurrentVer(CIRCUIT_PACK_SCC, pstCpComponent[i].acName, stModMfg.acFpga1Ver);
            }
            if (0 == strcasecmp(CP_COMPONENT_SCC_CPLD, pstCpComponent[i].acName)) {
                rc = DSC_CP_SetComponentCurrentVer(CIRCUIT_PACK_SCC, pstCpComponent[i].acName, stModMfg.acCpld1Ver);
            } else if (0 == strcasecmp(CP_COMPONENT_SCC_APP, pstCpComponent[i].acName)) {
                rc = DSC_CP_SetComponentCurrentVer(CIRCUIT_PACK_SCC, pstCpComponent[i].acName, stUpgdInfo.acAppVerRun);
            } else {
                LOG_WRITE(EN_LOG_DEBUG, "no current version info: %s", pstCpComponent[i].acName);
            }

            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "Set %s current version error: %s", pstCpComponent[i].acName, sr_strerror(rc));
                /* goto cleanup; */
            }
        }
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "%s acFwVer:%s, acSwVer:%s", __func__, stModMfg.acFwVer, stModMfg.acSwVer); */

    /* push mfg info to openconfig-platform operational datastore */
    rc = __PutComponentMfgInfoToOc(DEF_OCP_COMPONENT_KEY_SCC, &stModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by put mfginfo: %s, %s", DEF_OCP_COMPONENT_KEY_SCC, sr_strerror(rc));
    }

cleanup:
    DEF_SAFE_FREE(pstCpComponent);
    return rc;
}

static int
__PutRpcMfgToOrgAndOc(void)
{
    int rc = SR_ERR_OK;
    CMfg stModMfg;

    /* write cp's administrative-state */
    rc = DSC_CP_SetAdminState(CIRCUIT_PACK_RPC, EQPT_ADMIN_STATES_INSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetAdminState acCpName = %s", CIRCUIT_PACK_RPC);
    }

    /* write cp's operational-state */
    rc = DSC_CP_SetOpState(CIRCUIT_PACK_RPC, STATE_INSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetOpState acCpName = %s", CIRCUIT_PACK_RPC);
        /* goto cleanup; */
    }

    /* rpc mfg == chassis mfg */
    memset(&stModMfg, 0x00, sizeof(stModMfg));
    rc = BOARD_GetChassisMfg(&stModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get rpc mfg info: %d", rc);
        goto cleanup;
    }

    /* clear version info from eeprom */
    memset(stModMfg.acFwVer, 0, sizeof(stModMfg.acFwVer));
    memset(stModMfg.acSwVer, 0, sizeof(stModMfg.acSwVer));
    /* LOG_WRITE(EN_LOG_NOTICE, "[rpc]acFwVer: %s, acSwVer: %s", stModMfg.acFwVer, stModMfg.acSwVer); */

    /* push mfg info to org-openroadm-device operational datastore */
    rc = __PutCpMfgInfoToOrg(CIRCUIT_PACK_RPC, &stModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by put mfginfo: %s, %s", CIRCUIT_PACK_RPC, sr_strerror(rc));
        /* goto cleanup; */
    }

    /* LOG_WRITE(EN_LOG_NOTICE, "%s acFwVer:%s, acSwVer:%s", __func__, stModMfg.acFwVer, stModMfg.acSwVer); */

    /* push mfg info to openconfig-platform operational datastore */
    rc = __PutComponentMfgInfoToOc(DEF_OCP_COMPONENT_KEY_RPC, &stModMfg);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by put mfginfo: %s, %s", DEF_OCP_COMPONENT_KEY_RPC, sr_strerror(rc));
    }

cleanup:
    return rc;
}

static int
__PutMuxMfgToOrgAndOc(int iSlotId, CSlotInfo *pstSlotInfo)
{
    int rc = SR_ERR_OK;
    CMfg stMuxMfg;
    int iSlotStatus = SLOT_STATE_ONLINE;
    bool bSlotOnline = true;

    if (NULL == pstSlotInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        return SR_ERR_INVAL_ARG;
    }

    iSlotStatus = pstSlotInfo->uiSlotState;
    bSlotOnline = (SLOT_STATE_ONLINE == iSlotStatus) ? true : false;

    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d ++", __func__, iSlotId, iSlotStatus);

    memset(&stMuxMfg, 0, sizeof(stMuxMfg));
    if (bSlotOnline) {
        rc = MUX_GetMfg(0, &stMuxMfg);
        if (rc != OPLK_OK) {
            LOG_WRITE(EN_LOG_ERROR, "error MUX_GetMfg");
        }

        LOG_WRITE(EN_LOG_NOTICE, "[mux]vender: %s, model: %s, sn: %s, pn: %s", stMuxMfg.acManufacturer, stMuxMfg.acProductName,
                  stMuxMfg.acSN, stMuxMfg.acPN);
        LOG_WRITE(EN_LOG_NOTICE, "[mux]hardware-ver: %s, clei: %s, mfg-date: %s", stMuxMfg.acHwVer, stMuxMfg.acCleiCode,
                  stMuxMfg.acMfgDate);
    }

    /* write cp's administrative-state */
    rc = DSC_CP_SetAdminState(CIRCUIT_PACK_MUX, (SLOT_STATE_ONLINE == iSlotStatus) ? EQPT_ADMIN_STATES_INSERVICE : EQPT_ADMIN_STATES_OUTOFSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetAdminState acCpName = %s, iSlotStatus = %d", CIRCUIT_PACK_MUX, iSlotStatus);
    }

    /* write cp's operational-state */
    rc = DSC_CP_SetOpState(CIRCUIT_PACK_MUX, bSlotOnline ? STATE_INSERVICE : STATE_OUTOFSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetOpState acCpName = %s, iSlotStatus = %d", CIRCUIT_PACK_MUX, iSlotStatus);
        /* goto cleanup; */
    }

    /* write cp's slot , cp-mux is a subcircuit packet of cp-scc*/
    rc = DSC_CP_SetModuleSlot(CIRCUIT_PACK_MUX, bSlotOnline ? CIRCUIT_PACK_SCC : "");
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetModuleSlot acCpName = %s, iSlotStatus = %d", CIRCUIT_PACK_MUX, SLOT_STATE_ONLINE);
    }

    /* write cp mfg to xml */
    rc = __PutCpMfgInfoToOrg(CIRCUIT_PACK_MUX, &stMuxMfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __PutCpMfgInfoToOrg acCpName = %s, iSlotStatus = %d", CIRCUIT_PACK_MUX, iSlotStatus);
        /* goto cleanup; */
    }

    /* write cp's administrative-state */
    rc = DSC_CP_SetAdminState(CIRCUIT_PACK_MUX, (SLOT_STATE_ONLINE == iSlotStatus) ? EQPT_ADMIN_STATES_INSERVICE : EQPT_ADMIN_STATES_OUTOFSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetAdminState acCpName = %s, iSlotStatus = %d", CIRCUIT_PACK_MUX, iSlotStatus);
        /* goto cleanup; */
    }

    /* TODO, wirte mux mfg to openconfig-platform */
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d, rc:%d --", __func__, iSlotId, iSlotStatus, rc);
    return rc;
}


static int
__SetPortThreAndHysToHal()
{
    return 0;
}

static int
__ConfigEvoa(uint32_t ulDevType, char *pcComponentName)
{
    int rc = SR_ERR_OK;
    double dAttn = 0.0;
#if 0
    bool bEnableCvProtection = false;
    static bool m_bSystemReset = true;
    char acResetCause[32] = {0};
#endif

    LOG_WRITE(EN_LOG_NOTICE, "%s ++ ulDevType = %d", __func__, ulDevType);

    rc = DSC_GetEvoaAttn(pcComponentName, &dAttn);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetEvoaAttn: %s, %s", pcComponentName, sr_strerror(rc));
        goto cleanup;
    }

#if 0
    /* set PA VOA according to system reset type, excluding plug-in */
    if ((DEV_TYPE_PA == ulDevType) && (0 == strcmp(pcComponentName, AMP_PA_VOA_KEY))) {
        if (true == m_bSystemReset) {
            m_bSystemReset = false;

            rc = DSC_GetCvProtectionEnable(NULL, &bEnableCvProtection);
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetCvProtectionEnable");
                goto setattn;
            }

            rc = DSC_SHELF_GetResetCause(acResetCause, sizeof(acResetCause));
            if (SR_ERR_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by DSC_SHELF_GetResetCause_I");
                goto setattn;
            }

            LOG_WRITE(EN_LOG_NOTICE, "Cv Protection (%d,%s,%.2lf)", bEnableCvProtection, acResetCause, dAttn);

            if (true == bEnableCvProtection) {
                if (0 == strcmp(acResetCause, EN_COMMON_INFO_RESET_CAUSE_POWER_STR)) {
                    LOG_WRITE(EN_LOG_NOTICE, "power on, set PA-VOA to blocked");
                    dAttn = MAX_PA_VOA_VALUE;
                } else {
                    LOG_WRITE(EN_LOG_NOTICE, "%s reset, PA-VOA unchanged", acResetCause);
                }
            }
        }
    }

setattn:
#endif

    rc = EDFA_SetAttn(ulDevType, (int16_t)ROUND_MUTI_100(dAttn));
    if (OPLK_OK != rc) {
        rc = SR_ERR_OPERATION_FAILED;
        LOG_WRITE(EN_LOG_ERROR, "Error by set evoa: %d attn: %.2f", ulDevType, dAttn);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dAttn = %.2f", __func__, ulDevType, pcComponentName, dAttn);
cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s -- ulDevType = %d", __func__, ulDevType);
    return rc;
}

static int
__ConfigOsc(uint32_t ulDevType, char *pcComponentName)
{
    int rc = SR_ERR_OK;
    OSC_CONF_ST stOscCfg;
    PRODUCT_TYPE_EN enProductType = EN_PRODUCT_TYPE_NR1004;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++ ulDevType = %d", __func__, ulDevType);

    /* get product name */
    enProductType = BOARD_GetProductType();

    memset(&stOscCfg, 0x00, sizeof(stOscCfg));
    rc = DSC_GetOscConfig(pcComponentName, &stOscCfg);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOscConfig %s, %s", pcComponentName, sr_strerror(rc));
        goto cleanup;
    }

    rc = OSC_SetEnable(ulDevType, stOscCfg.bEnable);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set %s, enable: %d", pcComponentName, stOscCfg.bEnable);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bEnable = %d", __func__, ulDevType, pcComponentName, stOscCfg.bEnable);

    if (EN_PRODUCT_TYPE_HCF == enProductType) {
        TCA_INFO_ST *pstTcaInfo = NULL;
        size_t count = 0;
        int iTcaTxIdx = 0, iTcaRxIdx = 0, iTcaAddIdx = 0, iTcaDropIdx = 0;
        double dTxLosHighThr = 0.0, dRxLosHighThr = 0.0, dAddLosHighThr = 0.0, dDropLosHighThr = 0.0;
        double dTxLosLowThr = 0.0, dRxLosLowThr = 0.0, dAddLosLowThr = 0.0, dDropLosLowThr = 0.0;
        double dTargetTx = 0.0;
        bool bEnable = false;

        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "%s Get tca list error", __func__);
            goto cleanup;
        }

        if (0 == strcmp(pcComponentName, OSC_CFG_KEY)) {
            iTcaTxIdx = TCA_IDX_OSC1_TX;
            iTcaRxIdx = TCA_IDX_OSC1_RX;
            iTcaAddIdx = TCA_IDX_OSC1_ADD;
            iTcaDropIdx = TCA_IDX_OSC1_DROP;
        } else {
            iTcaTxIdx = TCA_IDX_OSC2_TX;
            iTcaRxIdx = TCA_IDX_OSC2_RX;
            iTcaAddIdx = TCA_IDX_OSC2_ADD;
            iTcaDropIdx = TCA_IDX_OSC2_DROP;
        }

        DSC_GetTcaThresh(iTcaTxIdx, pstTcaInfo, &dTxLosHighThr, &dTxLosLowThr);
        DSC_GetTcaThresh(iTcaRxIdx, pstTcaInfo, &dRxLosHighThr, &dRxLosLowThr);
        DSC_GetTcaThresh(iTcaAddIdx, pstTcaInfo, &dAddLosHighThr, &dAddLosLowThr);
        DSC_GetTcaThresh(iTcaDropIdx, pstTcaInfo, &dDropLosHighThr, &dDropLosLowThr);

        rc = EDFA_SetThr(ulDevType, OA_THR_PORT_OSC_TX, OA_THR_TYPE_LOS, (int16_t)ROUND_MUTI_100(dTxLosLowThr), (int16_t)(ROUND_MUTI_100(dTxLosHighThr - dTxLosLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set osc %d TxLosThreshold error, dHighThr=%.2lf, dLowThr=%.2lf", ulDevType, dTxLosHighThr, dTxLosLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dTxLosHighThr = %.2lf, dTxLosLowThr = %.2lf", __func__, ulDevType, pcComponentName, dTxLosHighThr, dTxLosLowThr);

        rc = EDFA_SetThr(ulDevType, OA_THR_PORT_OSC_RX, OA_THR_TYPE_LOS, (int16_t)ROUND_MUTI_100(dRxLosLowThr), (int16_t)(ROUND_MUTI_100(dRxLosHighThr - dRxLosLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set osc %d RxLosThreshold error, dHighThr=%.2lf, dLowThr=%.2lf", ulDevType, dRxLosHighThr, dRxLosLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dRxLosHighThr = %.2lf, dRxLosLowThr = %.2lf", __func__, ulDevType, pcComponentName, dRxLosHighThr, dRxLosLowThr);

        rc = EDFA_SetThr(ulDevType, OA_THR_PORT_OSC_ADD, OA_THR_TYPE_LOS, (int16_t)ROUND_MUTI_100(dAddLosLowThr), (int16_t)(ROUND_MUTI_100(dAddLosHighThr - dAddLosLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set osc %d AddLosThreshold error, dHighThr=%.2lf, dLowThr=%.2lf", ulDevType, dAddLosHighThr, dAddLosLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dAddLosHighThr = %.2lf, dAddLosLowThr = %.2lf", __func__, ulDevType, pcComponentName, dAddLosHighThr, dAddLosLowThr);

        rc = EDFA_SetThr(ulDevType, OA_THR_PORT_OSC_DROP, OA_THR_TYPE_LOS, (int16_t)ROUND_MUTI_100(dDropLosLowThr), (int16_t)(ROUND_MUTI_100(dDropLosHighThr - dDropLosLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set osc %d DropLosThreshold error, dHighThr=%.2lf, dLowThr=%.2lf", ulDevType, dDropLosHighThr, dDropLosLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dDropLosHighThr = %.2lf, dDropLosLowThr = %.2lf", __func__, ulDevType, pcComponentName, dDropLosHighThr, dDropLosLowThr);
#if 0
        /* 1.01.0001 */
        /* set OSC amp power */
        if (0 == strcmp(AMP_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEvoaTarget(AMP_OSC_TX_VOA_KEY, &dTargetTx);
        } else if (0 == strcmp(AMP_P_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEvoaTarget(AMP_P_OSC_TX_VOA_KEY, &dTargetTx);
        } else {
            goto cleanup;
        }

        if (SR_ERR_OK == rc) {
            rc = OSC_SetTarget(ulDevType, OSC_TX, (int16_t)ROUND_MUTI_100(dTargetTx));
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by set osc %d target tx power %.2lf", ulDevType, dTargetTx);
                goto cleanup;
            }

            LOG_WRITE(EN_LOG_NOTICE, "Set osc %d target tx power %.2lf ", ulDevType, dTargetTx);
        }

        /* set OSC amp enable */
        if (0 == strcmp(AMP_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEvoaEnable(AMP_OSC_TX_VOA_KEY, &bEnable);
        } else if (0 == strcmp(AMP_P_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEvoaEnable(AMP_P_OSC_TX_VOA_KEY, &bEnable);
        } else {
            goto cleanup;
        }
        rc = OSC_SetAmpEnable(ulDevType, bEnable);
        if (OPLK_OK != rc) {
            rc = SR_ERR_OPERATION_FAILED;
            LOG_WRITE(EN_LOG_ERROR, "Set evoa: %s enable: %d error", ulDevType, bEnable);
            goto cleanup;
        }
#else
        /* set OSC amp power */
        if (0 == strcmp(AMP_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEdfaOutputPowerByName(AMP_OSC_KEY, &dTargetTx);
        } else if (0 == strcmp(AMP_P_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEdfaOutputPowerByName(AMP_P_OSC_KEY, &dTargetTx);
        } else {
            goto cleanup;
        }

        if (SR_ERR_OK == rc) {
            LOG_WRITE(EN_LOG_NOTICE, "Set osc %d target tx power %.2lf ", ulDevType, dTargetTx);
            rc = EDFA_SetTargetPower(ulDevType, (int16_t)ROUND_MUTI_100(dTargetTx));
            if (OPLK_OK != rc) {
                rc = SR_ERR_OPERATION_FAILED;
                LOG_WRITE(EN_LOG_ERROR, "Error by set osc %d target tx power %.2lf", ulDevType, dTargetTx);
                goto cleanup;
            }
        }

        /* set OSC amp enable */
        if (0 == strcmp(AMP_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEdfaOutputEnableByName(AMP_OSC_KEY, &bEnable);
        } else if (0 == strcmp(AMP_P_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEdfaOutputEnableByName(AMP_P_OSC_KEY, &bEnable);
        } else {
            goto cleanup;
        }

        if (SR_ERR_OK == rc) {
            rc = EDFA_SetPumpEnable(ulDevType, bEnable);
            if (OPLK_OK != rc) {
                rc = SR_ERR_OPERATION_FAILED;
                LOG_WRITE(EN_LOG_ERROR, "Set evoa: %s enable: %d error", ulDevType, bEnable);
                goto cleanup;
            }
        }

#endif

    }

    if (EN_PRODUCT_TYPE_NR1004 == enProductType) {
        double dTargetTx = 0.0;

        if (0 == strcmp(AMP_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEvoaTarget(AMP_OSC_TX_VOA_KEY, &dTargetTx);
        } else if (0 == strcmp(AMP_P_OSC_KEY, pcComponentName)) {
            rc = DSC_GetEvoaTarget(AMP_P_OSC_TX_VOA_KEY, &dTargetTx);
        } else {
            goto cleanup;
        }

        if (SR_ERR_OK == rc) {
            rc = OSC_SetTarget(ulDevType, OSC_TX, (int16_t)ROUND_MUTI_100(dTargetTx));
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by set osc %d target tx power %.2lf", ulDevType, dTargetTx);
                goto cleanup;
            }

            LOG_WRITE(EN_LOG_NOTICE, "Set osc %d target tx power %.2lf ", ulDevType, dTargetTx);
        }
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "%s -- ulDevType = %d", __func__, ulDevType);
    return rc;
}

static int
__ConfigEdfa(uint32_t ulDevType, char *pcComponentName)
{
    int rc = OPLK_OK;
    EDFA_CFG_ST stEdfaCfg;
    PRODUCT_TYPE_EN enProductType = EN_PRODUCT_TYPE_NR1004;

    double dAttn = 0.0;
    DFB_CONF_ST stDfbCfg;
    LASERSAFETY_CFG_ST stLasersafetyCfg;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++ ulDevType = %d %s", __func__, ulDevType, pcComponentName);

    memset(&stEdfaCfg, 0, sizeof(stEdfaCfg));
    rc = DSC_GetEdfaConfig(pcComponentName, &stEdfaCfg, sizeof(stEdfaCfg));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetEdfaConfig edfa : %s, %s", pcComponentName, sr_strerror(rc));
        goto cleanup;
    }

    rc = EDFA_SetApsdEnable(ulDevType, true);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error EDFA_SetApsdEnable(%d), rc = %d", ulDevType, rc);
    }

    rc = EDFA_SetAprEnable(ulDevType, (stEdfaCfg.bAprEnable ? 1 : 0));
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set edfa %d apr enable error, AprEnable = %d", ulDevType, stEdfaCfg.bAprEnable);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bAprEnable = %d", __func__, ulDevType, pcComponentName, stEdfaCfg.bAprEnable);

    rc = EDFA_SetTilt(ulDevType, (int16_t)ROUND_MUTI_100(stEdfaCfg.dTiltSetPoint));
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set edfa %d tilt error, TiltSetPoint = %.2f", ulDevType, stEdfaCfg.dTiltSetPoint);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dTiltSetPoint = %.2f", __func__, ulDevType, pcComponentName, stEdfaCfg.dTiltSetPoint);

    /* get product name */
    enProductType = BOARD_GetProductType();

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) product name = %d", __func__, ulDevType, pcComponentName, enProductType);

    if (EN_PRODUCT_TYPE_OLSP == enProductType) {
        if (EDFA_MODE_APC == stEdfaCfg.enMode) {
            rc = EDFA_SetMode(ulDevType, EDFA_MODE_APC, 1500);
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error EDFA_SetMode apc");
                goto cleanup;
            }
        } else {
            rc = EDFA_SetGain(ulDevType, (int16_t)ROUND_MUTI_100(stEdfaCfg.dGainSetPoint));
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Set edfa gain error. ulDevType = %d, dGainSetPoint = %f, errNum = %d",
                          ulDevType, stEdfaCfg.dGainSetPoint, rc);
                goto cleanup;
            }
        }
    } else if (EN_PRODUCT_TYPE_NR1004 == enProductType || EN_PRODUCT_TYPE_HCF == enProductType) {
        if (EDFA_MODE_ACC == stEdfaCfg.enMode) {
            rc = EDFA_SetMode(ulDevType, OA_MODE_ACC, 0);
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Set edfa acc mode error. ulDevType = %d, ucMode = %hhu, errNum = %d",
                          ulDevType, OA_MODE_ACC, rc);
                goto cleanup;
            }

            LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) ACC mode", __func__, ulDevType, pcComponentName);

            for (int i = 0; i < stEdfaCfg.ucNumOfPumps; ++i) {
                rc = EDFA_SetPumpCurrent(ulDevType, i, (int16_t)ROUND_MUTI_10(stEdfaCfg.dLaserBiasCurrents[i]));
                if (OPLK_OK != rc) {
                    LOG_WRITE(EN_LOG_ERROR, "Set edfa pump%d current error. ulDevType = %d, dLaserBiasCurrent = %.2lf, errNum = %d",
                              i, ulDevType, stEdfaCfg.dLaserBiasCurrents[i], rc);
                    goto cleanup;
                }

                LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) ACC mode dLaserBiasCurrent%d = %.2lf", __func__, ulDevType, pcComponentName, i, stEdfaCfg.dLaserBiasCurrents[0]);
            }
        } else {
            rc = EDFA_SetMode(ulDevType, OA_MODE_AGC, (int16_t)ROUND_MUTI_100(stEdfaCfg.dGainSetPoint));
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Set edfa agc mode error. ulDevType = %d, dGainSetPoint = %.2lf, errNum = %d",
                          ulDevType, stEdfaCfg.dGainSetPoint, rc);
                goto cleanup;
            }

            LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) AGC mode dGainSetPoint = %.2lf", __func__, ulDevType, pcComponentName, stEdfaCfg.dGainSetPoint);
        }
    }

    rc = EDFA_SetAutolosEnable(ulDevType, stEdfaCfg.bAutoLosEnable);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set edfa %d auto los enable error, AutoLosEnable = %d", ulDevType, stEdfaCfg.bAutoLosEnable);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bAutoLosEnable = %d", __func__, ulDevType, pcComponentName, stEdfaCfg.bAutoLosEnable);

    if (EN_PRODUCT_TYPE_OLSP == enProductType) {
        rc = EDFA_SetRxLosThr(ulDevType, (int16_t)ROUND_MUTI_100(stEdfaCfg.dInLosLowThr), (int16_t)(ROUND_MUTI_100(stEdfaCfg.dInLosHighThr - stEdfaCfg.dInLosLowThr)));
    } else {
        rc = EDFA_SetThr(ulDevType, (DEV_TYPE_BA == ulDevType || DEV_TYPE_P_BA == ulDevType ? OA_THR_PORT_MUX_R : OA_THR_PORT_LINE_R),
                         OA_THR_TYPE_LOS, (int16_t)ROUND_MUTI_100(stEdfaCfg.dInLosLowThr), (int16_t)(ROUND_MUTI_100(stEdfaCfg.dInLosHighThr - stEdfaCfg.dInLosLowThr)));
    }
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set edfa %d InLosThreshold error, dHighThr=%.2lf, dLowThr=%.2lf",
                  ulDevType, stEdfaCfg.dInLosHighThr, stEdfaCfg.dInLosLowThr);
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dInLosHighThr = %.2lf, dInLosLowThr = %.2lf", __func__, ulDevType, pcComponentName,
              stEdfaCfg.dInLosHighThr, stEdfaCfg.dInLosLowThr);

    if (EN_PRODUCT_TYPE_HCF == enProductType) {
        rc = EDFA_SetThr(ulDevType, (DEV_TYPE_BA == ulDevType || DEV_TYPE_P_BA == ulDevType ? OA_THR_PORT_LINE_T : OA_THR_PORT_MUX_T),
                         OA_THR_TYPE_LOS, (int16_t)ROUND_MUTI_100(stEdfaCfg.dOutLosLowThr), (int16_t)(ROUND_MUTI_100(stEdfaCfg.dOutLosHighThr - stEdfaCfg.dOutLosLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d OutLosThreshold error, dHighThr=%.2lf, dLowThr=%.2lf",
                      ulDevType, stEdfaCfg.dOutLosHighThr, stEdfaCfg.dOutLosLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dOutLosHighThr = %.2lf, dOutLosLowThr = %.2lf", __func__, ulDevType, pcComponentName,
                  stEdfaCfg.dOutLosHighThr, stEdfaCfg.dOutLosLowThr);
    }

    if (EN_PRODUCT_TYPE_HCF == enProductType) {
        rc = EDFA_SetFsmMode(ulDevType, stEdfaCfg.enFsmMode);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d fsm mode %d error", ulDevType, stEdfaCfg.enFsmMode);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) fsm mode = %d", __func__, ulDevType, pcComponentName, stEdfaCfg.enFsmMode);
    }

    if (EN_PRODUCT_TYPE_HCF == enProductType) {
        rc = EDFA_SetReadyPower(ulDevType, (int16_t)ROUND_MUTI_100(stEdfaCfg.dReadyPower));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d ready power %d error", ulDevType, stEdfaCfg.dReadyPower);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) ready power = %f", __func__, ulDevType, pcComponentName, stEdfaCfg.dReadyPower);
    }

#if 0
    /* hcf beta module add safety mode and ready mode, so no need this process here any more */
    if (EN_PRODUCT_TYPE_HCF == enProductType && DEV_TYPE_BA == ulDevType) {
        LOG_WRITE(EN_LOG_NOTICE, "High Power BA, not set EDFA_SetPumpEnable(%d,stEdfaCfg.bOutputEnable)", ulDevType, stEdfaCfg.bOutputEnable);
    } else {
#endif
    rc = EDFA_SetPumpEnable(ulDevType, (stEdfaCfg.bOutputEnable ? 1 : 0));
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Set edfa %d pumpenable error, OutputEnable = %d", ulDevType, stEdfaCfg.bOutputEnable);
        goto cleanup;
    }
#if 0
}
#endif

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bOutputEnable = %d", __func__, ulDevType, pcComponentName, stEdfaCfg.bOutputEnable);

    if (EN_PRODUCT_TYPE_NR1004 == enProductType) {
        if ( 0 == strcmp(AMP_BA_KEY, pcComponentName)) {
            rc = DSC_GetEvoaAttn(AMP_OSC_TX_VOA_KEY, &dAttn);
        } else if (0 == strcmp(AMP_P_BA_KEY, pcComponentName)) {
            rc = DSC_GetEvoaAttn(AMP_P_OSC_TX_VOA_KEY, &dAttn);
        } else {
            goto cleanup;
        }

        if (SR_ERR_OK == rc) {
            rc = OSC_SetVoa(ulDevType, OSC_TX, (int16_t)ROUND_MUTI_100(dAttn));
            LOG_WRITE(EN_LOG_NOTICE, "Set osc tx evoa: %d attn: %.2f", ulDevType, dAttn);
            if (OPLK_OK != rc) {
                rc = SR_ERR_OPERATION_FAILED;
                LOG_WRITE(EN_LOG_ERROR, "Error by set evoa: %d attn: %.2f", ulDevType, dAttn);
            }
        }
    }

    if (EN_PRODUCT_TYPE_HCF == enProductType && (DEV_TYPE_BA == ulDevType || DEV_TYPE_P_BA == ulDevType)) {
        memset(&stDfbCfg, 0, sizeof(stDfbCfg));
        rc = DSC_GetDfbConfig("dfb-hp", &stDfbCfg);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetDfbConfig, %s", sr_strerror(rc));
            goto cleanup;
        }

        rc = EDFA_SetDfbEnable(DEV_TYPE_BA, true, stDfbCfg.dTargetOutputPower);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d dfb target output power error, power = %.2lf", ulDevType, stDfbCfg.dTargetOutputPower);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bDfbEnable = true power = %.2lf", __func__, ulDevType, pcComponentName, stDfbCfg.dTargetOutputPower);

        memset(&stLasersafetyCfg, 0, sizeof(stLasersafetyCfg));
        rc = DSC_GetLaserSafetyCfg(&stLasersafetyCfg, sizeof(stLasersafetyCfg));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "rc:%d DSC_GetLaserSafetyCfg error!", rc);
            goto cleanup;
        }

        rc = EDFA_SetRemoteInterlockEnable(DEV_TYPE_BA, stLasersafetyCfg.bRemoteInterlockEnable);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d remote inter lock %d error", ulDevType, stLasersafetyCfg.bRemoteInterlockEnable);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bRemoteInterlockEnable = %d", __func__, ulDevType, pcComponentName, stLasersafetyCfg.bRemoteInterlockEnable);

        rc = EDFA_SetDummyAdminKey(DEV_TYPE_BA, stLasersafetyCfg.bDummyAdminKeyEnable);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d dummy admin key %d error", ulDevType, stLasersafetyCfg.bDummyAdminKeyEnable);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bDummyAdminKeyEnable = %d", __func__, ulDevType, pcComponentName, stLasersafetyCfg.bDummyAdminKeyEnable);

#if 1 /* NOT support currently */
        rc = EDFA_SetPanelSafeCmpt(DEV_TYPE_BA, stLasersafetyCfg.bPanelSafeCmptEnable);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d panel safety components %d error", ulDevType, stLasersafetyCfg.bPanelSafeCmptEnable);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bPanelSafeCmptEnable = %d", __func__, ulDevType, pcComponentName, stLasersafetyCfg.bPanelSafeCmptEnable);
#endif

        rc = EDFA_SetThr(ulDevType, OA_THR_PORT_SAFTY_R, OA_THR_TYPE_LOS,
                         (int16_t)ROUND_MUTI_100(stDfbCfg.dLaserSafetyRLowThr), (int16_t)(ROUND_MUTI_100(stDfbCfg.dLaserSafetyRHighThr - stDfbCfg.dLaserSafetyRLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d LaserSafetyRxLosThreshold error, dHighThr=%.2f, dLowThr=%.2f",
                      ulDevType, stDfbCfg.dLaserSafetyRHighThr, stDfbCfg.dLaserSafetyRLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dLaserSafetyRxHighThr = %.2f, dLaserSafetyRxLowThr = %.2f", __func__, ulDevType, pcComponentName,
                  stDfbCfg.dLaserSafetyRHighThr, stDfbCfg.dLaserSafetyRLowThr);

        rc = EDFA_SetThr(ulDevType, OA_THR_PORT_SAFTY_T, OA_THR_TYPE_LOS,
                         (int16_t)ROUND_MUTI_100(stDfbCfg.dLaserSafetyTLowThr), (int16_t)(ROUND_MUTI_100(stDfbCfg.dLaserSafetyTHighThr - stDfbCfg.dLaserSafetyTLowThr)));
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Set edfa %d LaserSafetyTxLosThreshold error, dHighThr=%.2f, dLowThr=%.2f",
                      ulDevType, stDfbCfg.dLaserSafetyTHighThr, stDfbCfg.dLaserSafetyTLowThr);
            goto cleanup;
        }

        LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dLaserSafetyTxHighThr = %.2f, dLaserSafetyTxLowThr = %.2f", __func__, ulDevType, pcComponentName,
                  stDfbCfg.dLaserSafetyTHighThr, stDfbCfg.dLaserSafetyTLowThr);
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s -- ulDevType = %d", __func__, ulDevType);

    return rc;
}

#if 0
static int
__ConfigOtdrOnSlot(uint32_t ulSlotIdx, uint32_t ulDevIdx, uint32_t ulFuncType, uint32_t ulSubId)
{
    int rc = OPLK_OK;
    OTDR_CFG_ST stOtdrCfg;


    LOG_WRITE(EN_LOG_NOTICE, "%s ++", __func__);

    memset(&stOtdrCfg, 0, sizeof(stOtdrCfg));
    rc = DSC_OtdrGetCfg(NULL, &stOtdrCfg);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_OtdrGetCfg rc = %s", sr_strerror(rc));
        goto cleanup;
    }

    rc = OTDR_SetPara(0, stOtdrCfg.stPara.ulDistRange, stOtdrCfg.stPara.ulPulseWidth, stOtdrCfg.stPara.dSampleResolution);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error OTDR_SetPara, ulDistRange = %d, ulPulseWidth = %d, dSampleResolution = %.2f",
                  stOtdrCfg.stPara.ulDistRange, stOtdrCfg.stPara.ulPulseWidth, stOtdrCfg.stPara.dSampleResolution);
        goto cleanup;
    }

    rc = OTDR_SetMeasuringTime(0, stOtdrCfg.ulMeasuringTime);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error OTDR_SetMeasuringTime, ulMeasuringTime = %d", stOtdrCfg.ulMeasuringTime);
        goto cleanup;
    }

    rc = OTDR_SetUserIOR(0, stOtdrCfg.dUserIOR);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error OTDR_SetUserIOR, dUserIOR = %d", stOtdrCfg.dUserIOR);
        goto cleanup;
    }

    rc = OTDR_SetThr(0, stOtdrCfg.stThr.dRefLosThr, stOtdrCfg.stThr.dSpliceLosThr, stOtdrCfg.stThr.dFiberEndThr);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "error OTDR_SetThr, dRefLosThr = %.2f, dSpliceLosThr = %.2f, dFiberEndThr = %.2f",
                  stOtdrCfg.stThr.dRefLosThr, stOtdrCfg.stThr.dSpliceLosThr, stOtdrCfg.stThr.dFiberEndThr);
        goto cleanup;
    }

cleanup:

    LOG_WRITE(EN_LOG_NOTICE, "%s -- ulDevType = %d", __func__, ulDevType);

    return rc;
}
#endif

static int
__ConfigOps(uint32_t ulDevType, char *pcComponentName)
{
    int rc = SR_ERR_OK;
    /* SAVE_OPS_MODE_T stMode; */
    OPS_CONFIG_ST stOpsCfg;
    uint32_t uiMode = 0;
    uint32_t uiDevType = 0, uiPortId = OPS_SUB_INDEX_DEFAULT;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++ ulDevType = %d", __func__, ulDevType);

    memset(&stOpsCfg, 0, sizeof(stOpsCfg));
    rc = DSC_GetOpsConfig(pcComponentName, &stOpsCfg, sizeof(stOpsCfg));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetOpsConfig: %s. acCfgKey: %s", sr_strerror(rc), pcComponentName);
        goto cleanup;
    }

    rc = OPS_GetOpsDevIdByKey(pcComponentName, &uiDevType, &uiPortId);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by OPS_GetOpsDevIdByKey, invalid ops key: %s", pcComponentName);
        goto cleanup;
    }

    rc = OPS_SetSwitchingType(ulDevType, stOpsCfg.enSwitchingType, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetSwitchingType ulDevType : %d, enSwitchingType = %d", ulDevType, stOpsCfg.enSwitchingType);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) enSwitchingType = %d", __func__, ulDevType, pcComponentName, stOpsCfg.enSwitchingType);

    rc = OPS_SetWtr(ulDevType, stOpsCfg.uiWtr / 1000, uiPortId);                                                                     /* ms => s */
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetWtr ulDevType : %d, ulOpsWtr = %d", ulDevType, stOpsCfg.uiWtr);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) uiWtr = %d", __func__, ulDevType, pcComponentName, stOpsCfg.uiWtr);

    rc = OPS_SetHoldOffTime(ulDevType, stOpsCfg.uiHoldOffTime / 10, uiPortId);                                                                     /* Unit: 10ms for OPS module */
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetHoldOffTime ulDevType : %d, ulHoldOffTime = %d",
                  ulDevType, stOpsCfg.uiHoldOffTime);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) uiHoldOffTime = %d", __func__, ulDevType, pcComponentName, stOpsCfg.uiHoldOffTime);

    rc = OPS_SetPortPowerThr(ulDevType, 0, TYPE_LOS, ROUND_MUTI_100(stOpsCfg.dPrimarySwitchThr), uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetPortPowerThr primary ulDevType : %d, dThreshold : %.2f",
                  ulDevType, stOpsCfg.dPrimarySwitchThr);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dPrimarySwitchThr = %.2f", __func__, ulDevType, pcComponentName, stOpsCfg.dPrimarySwitchThr);

    rc = OPS_SetPortPowerHys(ulDevType, 0, TYPE_LOS, ROUND_MUTI_100(stOpsCfg.dPrimaryHys), uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetPortPowerHys primary ulDevType : %d, dHysteresis : %.2f",
                  ulDevType, stOpsCfg.dPrimaryHys);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dPrimaryHys = %.2f", __func__, ulDevType, pcComponentName, stOpsCfg.dPrimaryHys);

    rc = OPS_SetPortPowerThr(ulDevType, 1, TYPE_LOS, ROUND_MUTI_100(stOpsCfg.dSecondarySwitchThr), uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetPortPowerThr secondary ulDevType : %d, dThreshold : %.2f",
                  ulDevType, stOpsCfg.dSecondarySwitchThr);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dSecondarySwitchThr = %.2f", __func__, ulDevType, pcComponentName, stOpsCfg.dSecondarySwitchThr);

    rc = OPS_SetPortPowerHys(ulDevType, 1, TYPE_LOS, ROUND_MUTI_100(stOpsCfg.dSecondaryHys), uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetPortPowerHys secondary ulDevType : %u, dHysteresis : %.2f",
                  ulDevType, stOpsCfg.dSecondaryHys);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dPrimaryHys = %.2f", __func__, ulDevType, pcComponentName, stOpsCfg.dPrimaryHys);

    rc = OPS_SetRevertive(ulDevType, stOpsCfg.bRevertive, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetRevertive ulDevType : %d, bRevertive = %d",
                  ulDevType, stOpsCfg.bRevertive);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) bRevertive = %d", __func__, ulDevType, pcComponentName, stOpsCfg.bRevertive);

    rc = OPS_SetRelativeThr(ulDevType, ROUND_MUTI_100(stOpsCfg.dRelativeThr), uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetRelativeThr ulDevType : %d, dRelativeThr = %.2f",
                  ulDevType, stOpsCfg.dRelativeThr);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dRelativeThr = %.2f", __func__, ulDevType, pcComponentName, stOpsCfg.dRelativeThr);

    rc = OPS_SetRelativeOffset(ulDevType, ROUND_MUTI_100(stOpsCfg.dRelativeThrOffset), uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetRelativeOffset ulDevType : %d, dRelativeThrOffset = %.2f",
                  ulDevType, stOpsCfg.dRelativeThrOffset);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) dRelativeThrOffset = %.2f", __func__, ulDevType, pcComponentName, stOpsCfg.dRelativeThrOffset);

    switch (stOpsCfg.enForceToPort) {
    case SWITCH_TO_NONE:
        uiMode = OPS_CLEAR;
        break;
    case SWITCH_TO_PRIMARY:
        uiMode = OPS_FORCE_P;
        break;
    case SWITCH_TO_SECONDARY:
        uiMode = OPS_FORCE_S;
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "Invalid enForceToPort: %d", stOpsCfg.enForceToPort);
        goto cleanup;
    }
    rc = OPS_SetMode(ulDevType, uiMode, uiPortId);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error OPS_SetMode ulDevType : %d, uiMode = %d",
                  ulDevType, uiMode);
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) enForceToPort = %d, uiMode = %d", __func__, ulDevType, pcComponentName, stOpsCfg.enForceToPort, uiMode);

    rc = __SetPortThreAndHysToHal();
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error __SetPortThreAndHysToHal");
    }

    {
        double dblThr = 0.0, dblHys = 0.0;
        size_t nItemIndex = 0;
        switch (uiDevType) {
        case DEV_TYPE_OPS:
            nItemIndex = 0;
            break;
        case DEV_TYPE_OPS4A:
        case DEV_TYPE_OPS4B:
            /* OPS-4 start from 1~9, but the item index is 0~8 */
            nItemIndex = (size_t)atoi(pcComponentName + strlen(OPS_OLP_KEY));
            if (nItemIndex >= 1) {
                nItemIndex = nItemIndex - 1;
            }
            break;
        }

        rc = DSC_GetOpsTcaValue(pcComponentName, nItemIndex, TCA_IDX_OPS_COM_IN_LOS, &dblThr, &dblHys);
        if (SR_ERR_OK == rc) {
            rc = OPS_SetConnectionPdThr(uiDevType, OPS_R, OPS_THR_TYPE_LOS, ROUND_MUTI_100(dblThr), uiPortId);
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by set '%s' com-in thr: %.2f",
                          pcComponentName, dblThr);
            } else {
                LOG_WRITE(EN_LOG_NOTICE, "Set '%s' com-in thr: %.2f succ",
                          pcComponentName, dblThr);
            }
            rc = OPS_SetConnectionPdHys(uiDevType, OPS_R, OPS_THR_TYPE_LOS, ROUND_MUTI_100(dblHys), uiPortId);
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by set '%s' com-in hys: %.2f",
                          pcComponentName, dblHys);
            } else {
                LOG_WRITE(EN_LOG_NOTICE, "Set '%s' com-in hys: %.2f succ",
                          pcComponentName, dblHys);
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Error by Get  '%s' tca com-in, item: %lu", pcComponentName, nItemIndex);
        }

        rc = DSC_GetOpsTcaValue(pcComponentName, nItemIndex, TCA_IDX_OPS_POWER_DIFFER, &dblThr, &dblHys);
        if (SR_ERR_OK == rc) {
            rc = OPS_SetThrPwrDiff(uiDevType, ROUND_MUTI_100(dblThr), ROUND_MUTI_100(dblHys), uiPortId);
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Error by set '%s' power-differ thr: %.2f, hys: %.2f",
                          pcComponentName, dblThr, dblHys);
            } else {
                LOG_WRITE(EN_LOG_NOTICE, "Set '%s' power-differ thr: %.2f, hys: %.2f succ",
                          pcComponentName, dblThr, dblHys);
            }
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Error by Get '%s' tca power-diff item: %lu", pcComponentName, nItemIndex);
        }
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "%s -- ulDevType = %d", __func__, ulDevType);
    return rc;
}

static int
__ConfigOcm(uint32_t ulDevType, char *pcComponentName)
{
    int rc = SR_ERR_OK;
    int i = 0;
    size_t nCount = 0;
    WAVEPLAN_ST *pstWaveplan = NULL;
    STChannelGrid *pstOcmFlexgridChan = NULL;
    WAVEPLAN_TYPE_EN enWaveplanType = EN_WAVEPLAN_TYPE_MAX;
    int iOcmPortCnt = 0;                                                                     /* OCM port number in one card */

    LOG_WRITE(EN_LOG_NOTICE, "%s ++ ulDevType = %d", __func__, ulDevType);

    enWaveplanType = DSC_GetWaveplanType();
    if (EN_WAVEPLAN_TYPE_FLEX == enWaveplanType) {
        LOG_WRITE(EN_LOG_NOTICE, "%s --, ignore config ocm", __func__);
        return SR_ERR_OK;
    }

    rc = DSC_GetOcmWaveplanInfo(&pstWaveplan, &nCount);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "DSC_GetInnerDefaultWaveplansInfo: %s", sr_strerror(rc));
        goto cleanup;
    }

    if (nCount <= 0 || nCount > MAX_CHL) {
        LOG_WRITE(EN_LOG_ERROR, "Invalid waveplan count: %lu", nCount);
        rc = SR_ERR_NOT_FOUND;
        goto cleanup;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s(%d, %s) waveplan count: %lu", __func__, ulDevType, pcComponentName, nCount);

    /* for ocm module protocol changed */
    pstOcmFlexgridChan = (STChannelGrid *)malloc(sizeof(STChannelGrid) * DEF_OCM_CHANNEL_MAX);
    if (NULL == pstOcmFlexgridChan) {
        LOG_WRITE(EN_LOG_ERROR, "Not enough memory");
        goto cleanup;
    }

    memset(pstOcmFlexgridChan, 0x00, sizeof(STChannelGrid) * DEF_OCM_CHANNEL_MAX);
    for (i = 0; i < nCount; i++) {
        pstOcmFlexgridChan[i].uCentralFreq = (uint32_t)(pstWaveplan[i].dCenterFreq * 1000);                                                                     /* GHz to MHz */
        pstOcmFlexgridChan[i].uBandwidth = (uint32_t)(pstWaveplan[i].dBandwidth * 1000);                                                                     /* GHz to MHz */
    }

    iOcmPortCnt = DSC_GetSingleOcmPortCnt(pcComponentName);
    for (i = 0; i < iOcmPortCnt; i++) {
        rc = OCM_SetOcmWaveplan(ulDevType, i + 1, DEF_OCM_CHANNEL_MAX, pstOcmFlexgridChan);
        if (OPLK_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error OCM_SetOcmWaveplan, ulDevType: %u, port : %d, count : %lu", ulDevType, i + 1, nCount);
            goto cleanup;
        }
    }

cleanup:

    DEF_SAFE_FREE(pstWaveplan);
    DEF_SAFE_FREE(pstOcmFlexgridChan);

    LOG_WRITE(EN_LOG_NOTICE, "%s -- ulDevType = %d", __func__, ulDevType);
    return rc;
}

static int
__ConfigOtdr(uint32_t ulDevType, char *pcComponentName)
{
    /* OTDR mudule is configured for each scan, no need initial config*/
    return OPLK_OK;
}

static int
__ConfigOcs(uint32_t ulDevType, char *pcComponentName)
{
    int rc = OPLK_OK;
    OCS_CONNECTIONS_ST stOcsCfg;
    COcsConnections stConns;
    uint32_t i = 0;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++ ulDevType = %d", __func__, ulDevType);

    memset(&stOcsCfg, 0, sizeof(stOcsCfg));
    rc = DSC_OcsGetConnections(&stOcsCfg, true);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_OcsGetConnections rc = %s", sr_strerror(rc));
        goto cleanup;
    }

    memset(&stConns, 0, sizeof(stConns));
    stConns.uiCount = stOcsCfg.uiConnCnt;
    for (i = 0; i < stConns.uiCount; i++) {
        stConns.astConn[i].ucId = stConns.astConn[i].ucSource = DSC_OcsPortNumber(stOcsCfg.astConnection[i].acSPort, DEF_OCS_PORT_IN_PREFIX);
        stConns.astConn[i].ucDestination = DSC_OcsPortNumber(stOcsCfg.astConnection[i].acDPort, DEF_OCS_PORT_OUT_PREFIX);
    }

    rc = OCS_SetConnections(ulDevType, (const COcsConnections *)&stConns);

cleanup:
    return rc;
}

#if 0
static int
__ConfigWss(uint32_t ulDevType, char *pcComponentName)
{
    int rc = SR_ERR_OK;
    CWavePlan astFlexgridChannels[WR_WAVEPLAN_NUM_MAX];
    uint32_t i = 0;
    WR_WAVEPLAN_LIST_ST stWaveplanList;
    uint64_t ulBandWidth = 0;

    LOG_WRITE(EN_LOG_NOTICE, "%s ++, ulDevType: %d", __func__, ulDevType);

    /* get flex waveplan */
    memset(&stWaveplanList, 0, sizeof(stWaveplanList));
    rc = DSC_WrGetWaveplan(&stWaveplanList);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_WrGetWaveplan");
        goto cleanup;
    }

    if (0 == stWaveplanList.uiTotal) {
        LOG_WRITE(EN_LOG_NOTICE, "wss waveplan is empty. pcComponentName: %s, ulDevType: %d", pcComponentName, ulDevType);
        goto cleanup;
    }
    /* convert flex waveplan to hal format's waveplan */
    memset(&astFlexgridChannels, 0, sizeof(astFlexgridChannels));
    for (i = 0; i < stWaveplanList.uiTotal; i++) {
        ulBandWidth = stWaveplanList.astWaveplan[i].ulUpperFreq - stWaveplanList.astWaveplan[i].ulLowerFreq;
        astFlexgridChannels[i].uiChId = i + 1;
        astFlexgridChannels[i].uiCenterFreq = (uint32_t)((stWaveplanList.astWaveplan[i].ulLowerFreq + ulBandWidth / 2) / WR_WSS_BAND_WIDTH_PER_SLICE);
        astFlexgridChannels[i].usSliceCount = (uint16_t)(ulBandWidth / WR_WSS_BAND_WIDTH_PER_SLICE);

        LOG_WRITE(EN_LOG_NOTICE, "%d %d %d %6.2f %6.2f",
                  astFlexgridChannels[i].uiChId,
                  astFlexgridChannels[i].uiCenterFreq,
                  astFlexgridChannels[i].usSliceCount,
                  (double)stWaveplanList.astWaveplan[i].ulLowerFreq / 1000,
                  (double)stWaveplanList.astWaveplan[i].ulUpperFreq / 1000);
    }

    for (i = stWaveplanList.uiTotal; i < WR_WAVEPLAN_NUM_MAX; i++) {
        astFlexgridChannels[i].uiChId = i + 1;
        astFlexgridChannels[i].uiCenterFreq = 0;
        astFlexgridChannels[i].usSliceCount = 0;
    }

    /* For meiwu-pro, uiDevIdx : 0 ---wss1, 1 ---wss2 */
    /* For l-band,  uiDevIdx : 0, only one wss */
    rc = WSS_SetWaveplan(0, 0, WR_WAVEPLAN_NUM_MAX, astFlexgridChannels);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error by set %s waveplan.", pcComponentName);
        goto cleanup;
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "%s --, pcComponentName: %s, rc: %d", __func__, pcComponentName, rc);
    return rc;
}
#endif

static int
__RegisterCard(int iSlotId, bool bCfgEnable, CSlotInfo *pstSlotInfo)
{
    int rc = OPLK_OK;
    int iSlotStatus = pstSlotInfo->uiSlotState;
    char acSlotName[32] = {0};
    char acCpName[32] = {0};
    char acSlotState[64] = {0};

    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, bCfgEnable:%d, iSlotStatus:%d ++", __func__, iSlotId, bCfgEnable, iSlotStatus);

    memset(acSlotName, 0, sizeof(acSlotName));
    /* solt-id in hal must be transfered to slot-id in org-openroadm-device */
    rc = __GetSlotNameBySlotId(iSlotId + SLOT_ID_SLC_OFFSET, acSlotName);

    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __GetSlotNameBySlotId iSlotId = %d", iSlotId);
        goto cleanup;
    }

    /* 1.write shelf-slot's status */
    memset(acSlotState, 0, sizeof(acSlotState));
    rc = __SlotStateEnumToStr(iSlotStatus, acSlotState);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __SlotStateEnumToStr iSlotStatus = %d", iSlotStatus);
        goto cleanup;
    }

    rc = DSC_SHELF_SetSlotModuleStatus(acSlotName, acSlotState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __SetShelfStatusAndProvCp slot-id = %d", iSlotId);
        goto cleanup;
    }

    if (SLOT_STATE_OFFLINE == iSlotStatus) {
        OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_ABSENT, true);

        /* clear cp's mfg in org-openroadm-device and clear component state in openconfig-platform*/
        Slot_PutPlugableDeviceMfgToOrgAndOc(iSlotId, iSlotStatus, pstSlotInfo);

    } else if (SLOT_STATE_MISMATCH == iSlotStatus) {
        OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_SW_MISMATCH, true);

        /* clear cp's mfg in org-openroadm-device and clear component state in openconfig-platform*/
        Slot_PutPlugableDeviceMfgToOrgAndOc(iSlotId, iSlotStatus, pstSlotInfo);

    } else {
        /* 2.write cp's ref-slot */
        memset(acCpName, 0, sizeof(acCpName));
        rc = DSC_GetCpNameByCardType(iSlotId, pstSlotInfo->uiCardType, acCpName);
        if (OPLK_ERR == rc) {
            LOG_WRITE(EN_LOG_ERROR, "error DSC_GetCpNameByCardType iSlotId = %d, uiCardType = %d", iSlotId, pstSlotInfo->uiCardType);
            rc = SR_ERR_OPERATION_FAILED;
            goto cleanup;
        }

        rc = DSC_CP_SetModuleSlot(acCpName, acSlotName);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetModuleSlot acCpName = %s, acSlotName = %s", acCpName, acSlotName);
            goto cleanup;
        }

        /* 2.write cp's mfg in org-openroadm-device and write component state in openconfig-platform*/
        Slot_PutPlugableDeviceMfgToOrgAndOc(iSlotId, iSlotStatus, pstSlotInfo);

        /* 4.config module */
        if (true == bCfgEnable) {
            Slot_Config(iSlotId);
        }
    }

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, bCfgEnable:%d, iSlotStatus:%d, rc:%d --", __func__, iSlotId, bCfgEnable, iSlotStatus, rc);
    return rc;
}

static int
__UnRegisterCard(int iSlotId, int iSlotStatus)
{
    int rc = OPLK_OK;
    CSlotInfo stSlotInfo;
    char acSlotName[32] = {0};
    char acSlotState[64] = {0};

    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d ++", __func__, iSlotId, iSlotStatus);

    OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_ABSENT, true);

    memset(acSlotName, 0, sizeof(acSlotName));
    rc = __GetSlotNameBySlotId(iSlotId + SLOT_ID_SLC_OFFSET, acSlotName);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __GetSlotNameBySlotId iSlotId = %d", iSlotId);
        goto cleanup;
    }

    /* 1.clear cp's mfg and cp's ref-slot in org-openroadm-device  and clear component state in openconfig-platform */
    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    Slot_PutPlugableDeviceMfgToOrgAndOc(iSlotId, iSlotStatus, &stSlotInfo);

    /* 2.clear shelf-slot's status */
    memset(acSlotState, 0, sizeof(acSlotState));
    rc = __SlotStateEnumToStr(iSlotStatus, acSlotState);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error __SlotStateEnumToStr iSlotStatus = %d", iSlotStatus);
        goto cleanup;
    }
    rc = DSC_SHELF_SetSlotModuleStatus(acSlotName, acSlotState);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Error DSC_SHELF_SetSlotModuleStatus slot-name = %s", acSlotName);
        goto cleanup;
    }

    /* 3.clear current working cp */
#if 0
    rc = __SetCurCpInSlot(acSlotName, "");
    if (false == rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error __SetCurCpInSlot slot-name = %s", acSlotName);
        rc = SR_ERR_OPERATION_FAILED;
        goto cleanup;
    }
#endif

cleanup:
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d, rc:%d --", __func__, iSlotId, iSlotStatus, rc);
    return rc;
}

static void
__ProcessFixed(bool bNeedConfigShelfScc)
{
    /* write mfg of shelf and chassis */
    __PutChassisMfgToOrgAndOc();

    /* write mfg of SCC card, including submodules */
    __PutSccMfgToOrgAndOc();

    /* write mfg of RPC card, */
    __PutRpcMfgToOrgAndOc();

    /* conf shelf and scc */
    if (bNeedConfigShelfScc) {
        /* conf module on scc or chassic, exp:osc */
        /* TBD */
    }

    return;
}

static void
__ProcessFlexd(uint32_t ulSlotNum, CSlotInfo *pstSlotInfo, bool bNeedConfigSlot)
{
    int i = 0;
    bool bEnableAutoUpgd = UPGD_GetPlugUpgdCondition();
    ST_UPGD_INFO stUpgdInfo;
    AUTO_UPGD_SLOT_INFO_ST stAutoUpgdInfo;
    bool bSlotUpgded = false;

    memset(&stUpgdInfo, 0, sizeof(stUpgdInfo));
    UPGD_GetInfo(&stUpgdInfo);

    for (i = 0; i < ulSlotNum; i++) {
        if (CARD_TYPE_UNKOWN == (pstSlotInfo + i)->uiCardType) {
            if (i <= DSC_SLOT_2) {
                __RegisterCard(i, false, &pstSlotInfo[i]);
            }
            continue;
        }

        if (CARD_TYPE_MUX == (pstSlotInfo + i)->uiCardType) {
            __PutMuxMfgToOrgAndOc(i, &pstSlotInfo[i]);
            continue;
        }

        bSlotUpgded = false;

        /* running bank is A, or if B but no ver diff, it can do auto upgrading */
        if (bEnableAutoUpgd && SLOT_STATE_ONLINE == (pstSlotInfo + i)->uiSlotState) {
            if (true == stUpgdInfo.abSlotVerDif[i]) {
                bSlotUpgded = true;
                memset(&stAutoUpgdInfo, 0, sizeof(stAutoUpgdInfo));
                stAutoUpgdInfo.iSlotId = i;
                stAutoUpgdInfo.bNeedConfig = bNeedConfigSlot;

            }
        }

        /*LOG_WRITE(EN_LOG_NOTICE, "bSlotUpgded = %d", bSlotUpgded);*/

        __RegisterCard(i, bSlotUpgded ? false : bNeedConfigSlot, &pstSlotInfo[i]);

        if (bSlotUpgded) {
            CreateAutoUpgdThread(&stAutoUpgdInfo);
        }
        /* SLOT_ClearPlugFlag(i); */
    }

    return;
}

int
Slot_Config(uint32_t ulSlotIdx)
{
    int rc = OPLK_OK;
    CSlotInfo stSlotInfo;
    /*size_t i = 0, j = 0, k = 0*/;
    size_t i = 0;
    size_t nCpCnt = 0;
    bool bFindFlag = false;
    ST_SET_COMPONENT_CONF *pstSetComponentConfigList = NULL;
    /* char acCpName[32] = {0}; */

    LOG_WRITE(EN_LOG_NOTICE, "%s %d ++", __func__, ulSlotIdx);

    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    rc = SLOT_GetInfo(ulSlotIdx, &stSlotInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot info error. errNum = %d", rc);
        return OPLK_ERR;
    }

    __GetComponentConfigList(&pstSetComponentConfigList, &nCpCnt);
    /* nCpCnt = sizeof(m_astSetComponentConfig) / sizeof(m_astSetComponentConfig[0]); */

#if 1
    for (i = 0; i < nCpCnt; i++) {
        if ((ulSlotIdx == pstSetComponentConfigList[i].uiSlotIdx) &&
            (stSlotInfo.uiCardType == pstSetComponentConfigList[i].uiCardType)) {
            bFindFlag = true;
            pstSetComponentConfigList[i].pFuncComponentSetConf(pstSetComponentConfigList[i].uiDevType, \
                                                               pstSetComponentConfigList[i].acComponentName);
        }
    }
#else
    DSC_GetCpNameBySlotId(ulSlotIdx + SLOT_ID_SLC_OFFSET, acCpName, &m_stShelfInfo);

    LOG_WRITE(EN_LOG_NOTICE, "cp name = %s", acCpName);

    for (i = 0; i < CIRCUIT_PACK_NUM_MAX; i++) {
        if (0 == strcmp(acCpName, m_astCpInfo[i].acName)) {
            if (0 == strlen(m_astCpInfo[i].acName)) {
                break;
            }

            for (j = 0; j < CP_COMPONENT_NUM_MAX; j++) {
                if (0 == strlen(m_astCpInfo[i].astComponent[j].acName)) {
                    break;
                }

                for (k = 0; k < nCpCnt; k++) {
                    if (0 == strcasecmp(m_astCpInfo[i].astComponent[j].acName, m_astSetComponentConfig[k].acComponentName) || \
                        (0 == strcasecmp(m_astCpInfo[i].astComponent[j].acName, CP_COMPONENT_OPS) && \
                         0 == strcasecmp(m_astSetComponentConfig[k].acComponentName, OPS_OLP_KEY))) {
                        bFindFlag = true;
                        m_astSetComponentConfig[k].pFuncComponentSetConf(m_astSetComponentConfig[k].uiDevType, \
                                                                         m_astSetComponentConfig[k].acComponentName);
                        break;
                    }
                }

            }
            break;
        }
    }
#endif

    if (!bFindFlag) {
        LOG_WRITE(EN_LOG_ERROR, "NOT found slot(%d), uiCardType: %d.", ulSlotIdx, stSlotInfo.uiCardType);
        rc = OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s %d --", __func__, ulSlotIdx);

    return rc;
}

int
Slot_ConfigModule(uint32_t ulSlotIdx, uint32_t uiDevType)
{
    int rc = OPLK_OK;
    CSlotInfo stSlotInfo;
    size_t i = 0;
    size_t nCpCnt = 0;
    bool bFindFlag = false;
    ST_SET_COMPONENT_CONF *pstSetComponentConfigList = NULL;

    LOG_WRITE(EN_LOG_NOTICE, "%s %d %d ++", __func__, ulSlotIdx, uiDevType);

    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    rc = SLOT_GetInfo(ulSlotIdx, &stSlotInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot info error. errNum = %d", rc);
        return OPLK_ERR;
    }

    __GetComponentConfigList(&pstSetComponentConfigList, &nCpCnt);
    /* nCpCnt = sizeof(m_astSetComponentConfig) / sizeof(m_astSetComponentConfig[0]); */
    for (i = 0; i < nCpCnt; i++) {
        if ((ulSlotIdx == pstSetComponentConfigList[i].uiSlotIdx) &&
            (stSlotInfo.uiCardType == pstSetComponentConfigList[i].uiCardType) &&
            (uiDevType == pstSetComponentConfigList[i].uiDevType)) {
            bFindFlag = true;
            pstSetComponentConfigList[i].pFuncComponentSetConf(pstSetComponentConfigList[i].uiDevType, \
                                                               pstSetComponentConfigList[i].acComponentName);
        }
    }

    if (!bFindFlag) {
        LOG_WRITE(EN_LOG_ERROR, "NOT found devType(%d) on slot(%d)", uiDevType, ulSlotIdx);
        rc = OPLK_ERR;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s %d %d --", __func__, ulSlotIdx, uiDevType);

    return rc;
}

static int
__Slot_PutPlugableDeviceMfgToOrg(int iSlotId, int iSlotStatus, CSlotInfo *pstSlotInfo, CMfg *pstSlotMfg)
{
    int rc = SR_ERR_OK;
    CMfg stModMfg;
    size_t i = 0;
    size_t j = 0;
    size_t nGetComponentMfgSize = 0;
    char acSlotName[32] = {0};
    char acCpName[32] = {0};
    ST_CIRCUIT_PACK_COMPONENT *pstCpComponent = NULL;
    size_t nCompntCount = 0;
    ST_GET_COMPONENT_MFG *pstGetComponentMfgList = NULL;

    if (NULL == pstSlotInfo || NULL == pstSlotMfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d ++", __func__, iSlotId, iSlotStatus);
    /* get slot-name by slot-id */
    memset(acSlotName, 0, sizeof(acSlotName));
    /* solt-id in hal must be transfered to slot-id in org-openroadm-device */
    rc = __GetSlotNameBySlotId(iSlotId + SLOT_ID_SLC_OFFSET, acSlotName);
    if (rc != OPLK_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __GetSlotNameBySlotId iSlotId = %d", iSlotId);
        rc = SR_ERR_NOT_FOUND;
        goto cleanup;
    }

    /* get cp-name by slot-name */
    memset(acCpName, 0, sizeof(acCpName));
    rc = DSC_CP_GetCurCpInSlot(acSlotName, acCpName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetCurCpInSlot acSlotName = %s", acSlotName);
        goto cleanup;
    }

    /* write cp's administrative-state */
    rc = DSC_CP_SetAdminState(acCpName, (SLOT_STATE_ONLINE == iSlotStatus) ? EQPT_ADMIN_STATES_INSERVICE : EQPT_ADMIN_STATES_OUTOFSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetAdminState acCpName = %s, iSlotStatus = %d", acCpName, iSlotStatus);
        /* goto cleanup; */
    }

    /* write cp's operational-state */
    rc = DSC_CP_SetOpState(acCpName, (SLOT_STATE_ONLINE == iSlotStatus) ? STATE_INSERVICE : STATE_OUTOFSERVICE);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetOpState acCpName = %s, iSlotStatus = %d", acCpName, iSlotStatus);
        /* goto cleanup; */
    }


    /* write cp's slot */
    rc = DSC_CP_SetModuleSlot(acCpName, (SLOT_STATE_ONLINE == iSlotStatus) ? acSlotName : "");
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_SetModuleSlot acCpName = %s, acSlotName = %s", acCpName, acSlotName);
    }

    /* write cp mfg to xml */
    rc = __PutCpMfgInfoToOrg(acCpName, pstSlotMfg);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error __PutCpMfgInfo acCpName = %s, iSlotStatus = %d", acCpName, iSlotStatus);
        /* goto cleanup; */
    }

    /* get all components by cp-name*/
    rc = DSC_CP_GetComponents(acCpName, &pstCpComponent, &nCompntCount);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_CP_GetComponents CpName = %s", acCpName);
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "nCompntCount = %lu", nCompntCount);

    __GetComponentMfgList(&pstGetComponentMfgList, &nGetComponentMfgSize);

    /* write component version to xml */
    for (i = 0; i < nCompntCount; i++) {
        memset(&stModMfg, 0x00, sizeof(stModMfg));
        if (SLOT_STATE_ONLINE == iSlotStatus) {
            /* nGetComponentMfgSize = sizeof(m_astGetComponentMfg) / sizeof(m_astGetComponentMfg[0]); */
            for (j = 0; j < nGetComponentMfgSize; j++) {
                if (0 == strcmp(pstCpComponent[i].acName, pstGetComponentMfgList[j].acOrgComponentName)) {
                    rc = pstGetComponentMfgList[j].pFuncComponentGetMfg(pstGetComponentMfgList[j].uiDevType, &stModMfg);
                    if (rc != OPLK_OK) {
                        LOG_WRITE(EN_LOG_ERROR, "error pFuncComponentGetMfg. CpComponent : %s, CpName : %s, uiDevType : %d", pstCpComponent[i].acName, acCpName, pstGetComponentMfgList[j].uiDevType);
                        rc = SR_ERR_OPERATION_FAILED;
                    }

                    break;
                }
            }

            if (j >= nGetComponentMfgSize) {
                LOG_WRITE(EN_LOG_ERROR, "NOT find component in m_aastGetComponentMfg. CpComponent : %s, CpName : %s, iSlotId : %d", pstCpComponent[i].acName, acCpName, iSlotId);
            }
        } else {
            /* ignore, clear component version to xml */
        }

        LOG_WRITE(EN_LOG_NOTICE, "acCpName:%s, CpComponent:%s, acFwVer:%s", acCpName, pstCpComponent[i].acName, stModMfg.acFwVer);
        rc = DSC_CP_SetComponentCurrentVer(acCpName, pstCpComponent[i].acName, stModMfg.acFwVer);
        if (rc != SR_ERR_OK) {
            LOG_WRITE(EN_LOG_ERROR, "Error DSC_CP_SetComponentCurrentVer acCpName : %s, stCpInfo.astComponent[i].acName : %s",
                      acCpName, pstCpComponent[i].acName);
        }
    }

cleanup:
    DEF_SAFE_FREE(pstCpComponent);
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d, rc:%d --", __func__, iSlotId, iSlotStatus, rc);
    return rc;
}

static int
__Slot_PutPlugableDeviceMfgToOc(int iSlotId, int iSlotStatus, CSlotInfo *pstSlotInfo, CMfg *pstSlotMfg)
{
    int rc = SR_ERR_OK;
    char acComponent[32] = {0};
    CMfg stSlotMfg;
    CMfg stModMfg;
    bool bSlotOnline = false;
    ST_OC_COMPONENT_SUBCOMPONENT *pstSubcomponent = NULL;
    size_t nSubCount = 0;
    size_t i = 0, j = 0;
    size_t nGetComponentMfgSize = 0;
    ST_GET_COMPONENT_MFG *pstGetComponentMfgList = NULL;
    const ST_DSC_COMPONENT_INFO *pstComponentInfo = NULL;

    if (NULL == pstSlotInfo || NULL == pstSlotMfg) {
        LOG_WRITE(EN_LOG_ERROR, "Input null");
        rc = SR_ERR_INVAL_ARG;
        goto cleanup;
    }
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d ++", __func__, iSlotId, iSlotStatus);

    /*
       Note: Change the hard code to general platform code.
            DO NOT specify the slot number with hard-coded component name.
            we should query slot and components information from yang startup configuration.

       Author: MaoLu, date: 12/17/2023
     */
    pstComponentInfo = DSC_GetComponentInfoBySlotIndex(iSlotId);
    if (NULL == pstComponentInfo) {
        LOG_WRITE(EN_LOG_ERROR, "Unknown slotid. iSlotId: %d, iSlotStatus: %d", iSlotId, iSlotStatus);
        rc = SR_ERR_NOT_FOUND;
        goto cleanup;
    }
    memset(acComponent, 0, sizeof(acComponent));
    strncpy(acComponent, pstComponentInfo->pcComponentName, sizeof(acComponent) - 1);

    /*No online state for slot's component */

    memset(&stSlotMfg, 0, sizeof(stSlotMfg));

    if (SLOT_STATE_ONLINE == iSlotStatus) {
        memcpy(&stSlotMfg, pstSlotMfg, sizeof(stSlotMfg));
        bSlotOnline = true;
    } else {
        bSlotOnline = false;
    }
#if 1
    /* write slot status */
    rc = DSC_OCP_SetComponentSlotStatus(acComponent, bSlotOnline);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "error DSC_OCP_SetComponentSlotStatus. acComponent:%s, bSlotOnline:%d", acComponent, bSlotOnline);
    }
#endif

    /* LOG_WRITE(EN_LOG_NOTICE, "%s slot acFwVer:%s, acSwVer:%s, acComponent:%s", __func__, pstSlotMfg->acFwVer, pstSlotMfg->acSwVer, acComponent); */

    /* write slot mfg info */
    __PutComponentMfgInfoToOc(acComponent, pstSlotMfg);

    /* get all subcomponent by component name */
    rc = DSC_OCP_GetSubComponents(acComponent, &pstSubcomponent, &nSubCount);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* write the status and mfg info of subcomponet */
    __GetComponentMfgList(&pstGetComponentMfgList, &nGetComponentMfgSize);
    /* nGetComponentMfgSize = sizeof(m_astGetComponentMfg) / sizeof(m_astGetComponentMfg[0]); */
    for (i = 0; i < nSubCount; i++) {
        memset(&stModMfg, 0, sizeof(stModMfg));

        for (j = 0; j < nGetComponentMfgSize; j++) {
            if (0 == strcmp(pstSubcomponent[i].acName, pstGetComponentMfgList[j].acOcComponentName)) {
                if (SLOT_STATE_ONLINE == iSlotStatus) {
                    rc = pstGetComponentMfgList[j].pFuncComponentGetMfg(pstGetComponentMfgList[j].uiDevType, &stModMfg);
                    if (rc != OPLK_OK) {
                        LOG_WRITE(EN_LOG_ERROR, "error pFuncComponentGetMfg. acComponent : %s, Subcomponent : %s, uiDevType : %d", acComponent, pstSubcomponent[i].acName, pstGetComponentMfgList[j].uiDevType);
                        rc = SR_ERR_OPERATION_FAILED;
                    }
                } else {
                    /* clear component version to xml */
                    memset(&stModMfg, 0, sizeof(stModMfg));
                }

                break;
            }
        }

        /* Set Module operation status */
        if (0 == strcmp(COMPONENT_KEY_AMP, acComponent) ||
            0 == strcmp(COMPONENT_KEY_AMP_P, acComponent)) {
            rc = DSC_OCP_SetComponentSlotStatus(pstSubcomponent[i].acName, bSlotOnline);
            if (rc != SR_ERR_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error DSC_OCP_SetComponentSlotStatus. Subcomponent : %s, acComponent : %s, bSlotOnline : %d", pstSubcomponent[i].acName, acComponent, bSlotOnline);
            }
        }

        if (j >= nGetComponentMfgSize) {
            LOG_WRITE(EN_LOG_WARNING, "NOT found subcomponent(%s) to ignore __PutComponentMfgInfoToOc, acComponent : %s, iSlotId : %d", pstSubcomponent[i].acName, acComponent, iSlotId);
        } else {
            /* LOG_WRITE(EN_LOG_NOTICE, "%s mod acFwVer:%s, acSwVer:%s", __func__, stModMfg.acFwVer, stModMfg.acSwVer); */
            __PutComponentMfgInfoToOc(pstSubcomponent[i].acName, &stModMfg);
        }

    }

cleanup:
    DEF_SAFE_FREE(pstSubcomponent);
    LOG_WRITE(EN_LOG_NOTICE, "%s iSlotId:%d, iSlotStatus:%d, rc:%d --", __func__, iSlotId, iSlotStatus, rc);
    return rc;
}

int
Slot_PutPlugableDeviceMfgToOrgAndOc(int iSlotId, int iSlotStatus, CSlotInfo *pstSlotInfo)
{
    int rc = SR_ERR_OK;
    CMfg stSlotMfg;
    uint32_t ulDevType = 0;
    CMfg stMfgInfo;

    memset(&stSlotMfg, 0, sizeof(stSlotMfg));
    if (SLOT_STATE_ONLINE == iSlotStatus) {

        /* if (DSC_SLOT_IDX0 == iSlotId) { */
        if (CARD_TYPE_OPS == pstSlotInfo->uiCardType) {

            /** For OPS card, replace stSlotMfg with ops moduleMfgInfo.
             *  NOTE: Production information is written by the ops card itself
             */

            if (DSC_SLOT_IDX0 == iSlotId) {
                ulDevType = DEV_TYPE_OPS;
            } else if (DSC_SLOT_IDX1 == iSlotId) {
                ulDevType = DEV_TYPE_OPS4A;
            } else if (DSC_SLOT_IDX2 == iSlotId) {
                ulDevType = DEV_TYPE_OPS4B;
            } else {
                ulDevType = DEV_TYPE_OPS;
            }
            LOG_WRITE(EN_LOG_NOTICE, "Slot: %d, type: %d, ulDevType: %u", iSlotId, pstSlotInfo->uiCardType, ulDevType);

            rc = OPS_GetMfg(ulDevType, &stSlotMfg);
            if (rc != OPLK_OK) {
                LOG_WRITE(EN_LOG_ERROR, "error SLOT_GetMfg iSlotId = %d", iSlotId);
            } else {
                /* transfer date format from 2023-01-12 to 2023-01-12T00:00:00Z, TODO */
                if (10 == strlen(stSlotMfg.acMfgDate)) {
                    /* memcpy(stSlotMfg.acMfgDate + strlen(stSlotMfg.acMfgDate), "T00:00:00Z", strlen("T00:00:00Z")); */
                    strcat(stSlotMfg.acMfgDate, "T00:00:00Z");
                }
            }
        } else {
            memset(&stMfgInfo, 0, sizeof(stMfgInfo));
            rc = BOARD_GetChassisMfg(&stMfgInfo);
            if (OPLK_OK != rc) {
                LOG_WRITE(EN_LOG_ERROR, "Get chassis mfginfo error error");
            }
            if ((CARD_TYPE_OCS == pstSlotInfo->uiCardType) && (0 == strcasecmp(stMfgInfo.acProductName, "OCS"))) {
                ulDevType = (DSC_SLOT_IDX0 == iSlotId) ? DEV_TYPE_OCS : DEV_TYPE_NONE;
                rc = OCS_GetMfg(ulDevType, &stSlotMfg);
                if (rc != OPLK_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "error SLOT_GetMfg iSlotId = %d", iSlotId);
                } else {
                    /* transfer date format from 2023-01-12 to 2023-01-12T00:00:00Z, TODO */
                    if (10 == strlen(stSlotMfg.acMfgDate)) {
                        strcat(stSlotMfg.acMfgDate, "T00:00:00Z");
                    }
                }
            } else {
                rc = SLOT_GetMfg(iSlotId, &stSlotMfg);
                if (rc != OPLK_OK) {
                    LOG_WRITE(EN_LOG_ERROR, "error SLOT_GetMfg iSlotId = %d", iSlotId);
                }
            }
        }

    }

    __Slot_PutPlugableDeviceMfgToOrg(iSlotId, iSlotStatus, pstSlotInfo, &stSlotMfg);

    __Slot_PutPlugableDeviceMfgToOc(iSlotId, iSlotStatus, pstSlotInfo, &stSlotMfg);

    return rc;
}

void
Slot_Process(uint32_t ulSlotNum, CSlotInfo *pstSlotInfo, bool bNeedConfig)
{
    __InitShelfCpInfo();

    if (DSC_IsSchemaInstalled(DSC_GetRunningSession(), MODULE_NS_AMP)) {
        __InitApsd();
        __InitOsc();
        __InitApr();
    }

    /* Process SCC/chassis/RPC */
    __ProcessFixed(bNeedConfig);

    /* Process SLC card */
    __ProcessFlexd(ulSlotNum, pstSlotInfo, bNeedConfig);

    return;
}

int
Slot_HalEventCb(int iEventId, int iSubEventId)
{
#define   SLOT_OFFSET    DEF_EVENT_ID_SLOT0

    int iSlotId = iEventId - SLOT_OFFSET;
    int rc = OPLK_OK;
    CSlotInfo stSlotInfo;
    bool bIsSlotUpgded = false;
    AUTO_UPGD_SLOT_INFO_ST stAutoUpgdInfo;
    bool bEnableAutoUpgd = UPGD_GetPlugUpgdCondition();


    if (OPERATIOND_GetCardRestFlag(DSC_SLOT_NUM)) {
        LOG_WRITE(EN_LOG_WARNING, "logic scc-cpld/scc-fpga is being restarted. Ignore %s(%d, %d)", __func__, iEventId, iSubEventId);
        return 0;
    }

    if (OPERATIOND_GetCardRestFlag(iSlotId)) {
        LOG_WRITE(EN_LOG_WARNING, "Slot%d is being restarted. Ignore %s(%d, %d)", iSlotId, __func__, iEventId, iSubEventId);
        return 0;
    }

    memset(&stSlotInfo, 0, sizeof(stSlotInfo));
    rc = SLOT_GetInfo(iSlotId, &stSlotInfo);
    if (OPLK_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot[%d] info error. errNum = %d", iSlotId, rc);
        return 0;
    }

    LOG_WRITE(EN_LOG_NOTICE, "%s SlotId = %d, uiSlotState = %d, uiCardType = %d ++", __func__, iSlotId, stSlotInfo.uiSlotState, stSlotInfo.uiCardType);
    BOARD_SetLedState(LED0, DSC_LED_STATE_AMBER_BLINK);

    switch (iEventId) {
    case DEF_EVENT_ID_SLOT0:
#ifdef __HCF__
#else
    case DEF_EVENT_ID_SLOT1:
    case DEF_EVENT_ID_SLOT2:
#endif
        LOG_WRITE(EN_LOG_NOTICE, "Slot%d online event receive from DEF_OPERATION_EVENT_SOCKET_PATH, iEventId: %d", iSlotId, iEventId);
        if (SLOT_STATE_ONLINE == stSlotInfo.uiSlotState || SLOT_STATE_MISMATCH == stSlotInfo.uiSlotState) {
            memset(&stAutoUpgdInfo, 0, sizeof(stAutoUpgdInfo));
            if (true == bEnableAutoUpgd &&
                true == UPGD_IsSlotVerDif(iSlotId, false) &&
                SLOT_STATE_ONLINE == stSlotInfo.uiSlotState) {
                stAutoUpgdInfo.iSlotId = iSlotId;
                stAutoUpgdInfo.bNeedConfig = true;
                bIsSlotUpgded = true;
                LOG_WRITE(EN_LOG_NOTICE, "slot id = %d need to be upgraded!", iSlotId);
            }

            __RegisterCard(iSlotId, bIsSlotUpgded ? false : true, &stSlotInfo);
            /* tranfer slot-id from hal to app layer(defined in org-openroadm-device) */
            OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_ABSENT, false);

            if (bIsSlotUpgded) {
                /* tranfer slot-id from hal to app layer(defined in org-openroadm-device) */
                OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_SW_MISMATCH, true);
                CreateAutoUpgdThread(&stAutoUpgdInfo);
            }
        } else if (SLOT_STATE_OFFLINE == stSlotInfo.uiSlotState) {
            /* tranfer slot-id from hal to app layer(defined in org-openroadm-device) */

            /* OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_FAIL, false); */
            OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_SW_MISMATCH, false);
            OPERATIOND_SetSlotAlm(iSlotId + SLOT_ID_SLC_OFFSET, EN_CARD_ALM_ABSENT, true);

            StopAutoUpgdThread(iSlotId);
            __UnRegisterCard(iSlotId, stSlotInfo.uiSlotState);
        }

        break;
#ifdef __HCF__
    case DEF_EVENT_ID_SLOT1:
#else
    case DEF_EVENT_ID_SLOT3:
#endif
        __PutMuxMfgToOrgAndOc(iSlotId, &stSlotInfo);
        break;
    default:
        LOG_WRITE(EN_LOG_ERROR, "Unknown iEventId:%d.", iEventId);
        break;
    }

    BOARD_SetLedState(LED0, DSC_LED_STATE_GREEN);

    LOG_WRITE(EN_LOG_NOTICE, "%s SlotId = %d, uiSlotState = %d, uiCardType = %d --", __func__, iSlotId, stSlotInfo.uiSlotState, stSlotInfo.uiCardType);
    return 0;
}

