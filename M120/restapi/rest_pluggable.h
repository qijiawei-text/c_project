#ifndef __REST_PLUGGABLE_H__
#define __REST_PLUGGABLE_H__
#ifdef __cplusplus
extern "C" {
#endif


int REST_GetAllSlotsStatus(SHELF_SLOTS_ST *pstShellSlots);

int REST_IsSlotOnline(unsigned char ucSlot);

bool REST_IsOpticalCardOnline();

#ifdef __cplusplus
}
#endif

#endif