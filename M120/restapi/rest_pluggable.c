#include "rest_common.h"
#include "rest_msft.h"

static SHELF_SLOTS_ST m_stShellSlots;

int
REST_GetAllSlotsStatus(SHELF_SLOTS_ST *pstShellSlots)
{
    int rc = SR_ERR_OK;
    SHELF_SLOTS_ST stShellSlots;

    memset(&stShellSlots, 0, sizeof(stShellSlots));

    rc = DSC_GetAllSlotsState(&stShellSlots);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "Get slot state failed. rc = %d.", rc);
        goto ERR_LABEL;
    }

    memcpy(&m_stShellSlots, &stShellSlots, sizeof(stShellSlots));

    if (NULL != pstShellSlots) {
        memcpy(pstShellSlots, &stShellSlots, sizeof(stShellSlots));
    }

ERR_LABEL:

    return rc;
}

bool
REST_IsSlotOnline(unsigned char ucSlot)
{
    int i = 0;

    for (i = 0; i < m_stShellSlots.uiSlotCnt; i++) {
        if (ucSlot == m_stShellSlots.astSlotInfo[i].uiSlotId) {

            if (DSC_SLOT_STATUS_INSTALLED_PROV_MATCH == m_stShellSlots.astSlotInfo[i].enStatus) {
                return true;
            }
        }
    }

    return false;
}

bool
REST_IsOpticalCardOnline()
{

    char acSlotName[32] = {0};
    int rc = SR_ERR_OK;

    rc = DSC_CP_GetModuleSlot(CIRCUIT_PACK_OPTICAL, acSlotName);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get slot state failed, err: %s", sr_strerror(rc));
        return false;
    }

    if (0 == strlen(acSlotName)) {

        return false;
    }

    return true;
}
