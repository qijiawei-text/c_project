#ifndef _ORG_OPENROADM_COMMON_LINK_TYPES_
#define _ORG_OPENROADM_COMMON_LINK_TYPES_

#include <stdio.h>

/* org-openroadm-common-link-types:optical-control-mode enumeration */
typedef enum en_OPTICAL_CONTROL_MODE
{
    OPTICAL_CONTROL_MODE_NONE = 0,
    OPTICAL_CONTROL_MODE_POWER,
    OPTICAL_CONTROL_MODE_GAINLOSS,
    OPTICAL_CONTROL_MODE_OFF,
    OPTICAL_CONTROL_MODE_WAITING,
    OPTICAL_CONTROL_MODE_FAILED,
    OPTICAL_CONTROL_MODE_MAX
} EN_OPTICAL_CONTROL_MODE;

/* typedef org-openroadm-common-link-types:power-dBm decimal64 */
typedef double power_dBm;

/* typedef org-openroadm-common-link-types:ratio-dB decimal64 */
typedef double ratio_dB;

/* typedef org-openroadm-common-link-types:fiber-pmd decimal64 */
typedef double fiber_pmd;

#endif
