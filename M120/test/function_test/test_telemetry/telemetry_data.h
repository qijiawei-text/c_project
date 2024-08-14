

#ifndef __TELEMETRY_DATA_H__
#define __TELEMETRY_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PMD_VOA_NUM_MAX                  (2)
#define PMD_EDFA_NUM_MAX                 (2)
#define PMD_PORT_OCH_IN_NUM_MAX          (0)
#define PMD_PORT_OCH_OUT_NUM_MAX         (0)
#define PMD_PORT_OSC_NUM_MAX             (1)
#define PMD_UNMANAGED_PORT_NUM_MAX       (6)
#define PMD_MANAGED_PORT_NUM_MAX         (0)

#define SYSTEM_ID_MAX_LEN                (40)



extern void TELEMETRY_UnpackOpticalStream(const uint8_t *pucMsgBuf, size_t nMsgLen);

#ifdef __cplusplus
}
#endif
#endif

