#ifndef _ORG_OPENROADM_PORT_TYPES_
#define _ORG_OPENROADM_PORT_TYPES_

#include <stdio.h>

/* org-openroadm-port-types:port-wavelength-types enumeration */
typedef enum en_PORT_WAVELENGTH_TYPES
{
    PORT_WAVELENGTH_TYPES_NONE = 0,
    PORT_WAVELENGTH_TYPES_WAVELENGTH,
    PORT_WAVELENGTH_TYPES_MULTI_WAVELENGTH,
    PORT_WAVELENGTH_TYPES_MAX
} EN_PORT_WAVELENGTH_TYPES;

/* org-openroadm-port-types:port-module-types enumeration */
typedef enum en_PORT_MODULE_TYPES
{
    PORT_MODULE_TYPES_NONE = 0,
    PORT_MODULE_TYPES_100GBASE_LR4,
    PORT_MODULE_TYPES_CWDM_4,
    PORT_MODULE_TYPES_MAX
} EN_PORT_MODULE_TYPES;

#endif
