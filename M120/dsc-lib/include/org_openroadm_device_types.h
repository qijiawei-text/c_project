#ifndef _ORG_OPENROADM_DEVICE_TYPES_
#define _ORG_OPENROADM_DEVICE_TYPES_

#include <stdio.h>

/* org-openroadm-device-types:node-types enumeration */
typedef enum en_NODE_TYPES
{
    NODE_TYPES_NONE = 0,
    NODE_TYPES_RDM,
    NODE_TYPES_XPDR,
    NODE_TYPES_ILA,
    NODE_TYPES_EXTPLUG,
    NODE_TYPES_TOA,
    NODE_TYPES_MAX
} EN_NODE_TYPES;

/* org-openroadm-device-types:xpdr-node-types enumeration */
typedef enum en_XPDR_NODE_TYPES
{
    XPDR_NODE_TYPES_NONE = 0,
    XPDR_NODE_TYPES_TPDR,
    XPDR_NODE_TYPES_MPDR,
    XPDR_NODE_TYPES_SWITCH,
    XPDR_NODE_TYPES_REGEN,
    XPDR_NODE_TYPES_REGEN_UNI,
    XPDR_NODE_TYPES_MAX
} EN_XPDR_NODE_TYPES;

/* org-openroadm-device-types:port-qual enumeration */
typedef enum en_PORT_QUAL
{
    PORT_QUAL_NONE = 0,
    PORT_QUAL_ROADM_INTERNAL,
    PORT_QUAL_ROADM_EXTERNAL,
    PORT_QUAL_XPDR_NETWORK,
    PORT_QUAL_XPDR_CLIENT,
    PORT_QUAL_OTDR,
    PORT_QUAL_SWITCH_NETWORK,
    PORT_QUAL_SWITCH_CLIENT,
    PORT_QUAL_ILA_EXTERNAL,
    PORT_QUAL_ILA_INTERNAL,
    PORT_QUAL_MAX
} EN_PORT_QUAL;

#endif
