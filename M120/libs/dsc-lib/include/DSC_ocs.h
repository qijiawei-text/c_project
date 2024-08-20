#ifndef _DSC_OCS_H
#define _DSC_OCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

typedef enum {
    OCS_MODE_SWAP = 0,
    OCS_MODE_BLOCK,
    OCS_MODE_NATIVE,
    OCS_MODE_CNT
} OCS_MODE_EN;

#define OCS_STR_MODE_SWAP        "OCS_SWAP"
#define OCS_STR_MODE_BLOCK       "OCS_BLOCK"
#define OCS_STR_MODE_NATIVE      "OCS_NATIVE"

#define OCS_CONNECTIONS_MAX    (256)
#define OCS_PORT_DESC_LEN    (32)
typedef struct {
    uint32_t uiIndex;
    char acSPort[OPLK_NAME_MAX_LEN];
    char acSDesc[OCS_PORT_DESC_LEN];
    char acDPort[OPLK_NAME_MAX_LEN];
    char acDDesc[OCS_PORT_DESC_LEN];
} OCS_CONNECTION_ST;

typedef struct {
    uint32_t uiConnCnt;
    OCS_CONNECTION_ST astConnection[OCS_CONNECTIONS_MAX];
} OCS_CONNECTIONS_ST;

#define MODULE_NS_OCS                "openconfig-transport-line-connectivity"
#define OCS_XPATH_ROOT               "/openconfig-transport-line-connectivity:connections"

/*connections */
#define OCS_MODE_NODE                "com-oplink-line-connectivity-ext:ocs-mode"
#define OCS_XPATH_MODE               OCS_XPATH_ROOT "/" OCS_MODE_NODE
#define OCS_XPATH_CONN_ALL           OCS_XPATH_ROOT "//*"
#define OCS_XPATH_CONN               OCS_XPATH_ROOT "/connection[index='%d']"
#define OCS_XPATH_CONN_NODES         OCS_XPATH_CONN "//*"

/* connection */
#define OCS_CONNECTION_NODE           "connection"
#define OCS_XPATH_CONNECTION          OCS_XPATH_ROOT "/" OCS_CONNECTION_NODE
#define OCS_XPATH_CONNECTION_ALL      OCS_XPATH_CONNECTION "//*"

#define OCS_INDEX_NODE                "index"
#define OCS_INDEX_ALL                 OCS_XPATH_CONNECTION "/" OCS_INDEX_NODE

/* config */
#define OCS_CONFIG_XPATH              OCS_XPATH_CONN "/config"
#define OCS_CONFIG_INDEX_XPATH        OCS_CONFIG_XPATH "/" OCS_INDEX_NODE
#define OCS_SOURCE_NODE               "source"
#define OCS_CONFIG_SOURCE_XPATH       OCS_CONFIG_XPATH "/" OCS_SOURCE_NODE
#define OCS_SOURCE_DESC_NODE          "com-oplink-line-connectivity-ext:source-desc"
#define OCS_CONFIG_SOURCE_DESC_XPATH  OCS_CONFIG_XPATH "/" OCS_SOURCE_DESC_NODE
#define OCS_DEST_NODE                 "dest"
#define OCS_CONFIG_DEST_XPATH         OCS_CONFIG_XPATH "/" OCS_DEST_NODE
#define OCS_DEST_DESC_NODE            "com-oplink-line-connectivity-ext:dest-desc"
#define OCS_CONFIG_DEST_DESC_XPATH    OCS_CONFIG_XPATH "/" OCS_DEST_DESC_NODE


/*state */
#define OCS_STATE_ATTR_CNT 10
#define OCS_STATE_XPATH              OCS_XPATH_CONN "/state"
#define OCS_STATE_INDEX_XPATH        OCS_STATE_XPATH "/" OCS_INDEX_NODE
#define OCS_STATE_SOURCE_XPATH       OCS_STATE_XPATH "/" OCS_SOURCE_NODE
#define OCS_STATE_SOURCE_DESC_XPATH  OCS_STATE_XPATH "/" OCS_SOURCE_DESC_NODE
#define OCS_STATE_DEST_XPATH         OCS_STATE_XPATH "/" OCS_DEST_NODE
#define OCS_STATE_DEST_DESC_XPATH    OCS_STATE_XPATH "/" OCS_DEST_DESC_NODE

#define DEF_OCS_PORT_IN_PREFIX       "port-in"
#define DEF_OCS_PORT_OUT_PREFIX      "port-out"

/* OCS_MODE_EN DSC_GetOcsModeEn(char *pcMode); */
/* char *DSC_GetOcsModeStr(char *pcStr, OCS_MODE_EN enVal); */
/* int DSC_OcsSetSwitchMode(OCS_MODE_EN modeEn); */
/* OCS_MODE_EN DSC_OcsGetSwitchMode(); */

int DSC_OcsGetConnections(OCS_CONNECTIONS_ST *pstConn, bool isCfg);

int DSC_OcsSetConnections(const OCS_CONNECTIONS_ST *pstConns);

int DSC_OcsAddConnections(const OCS_CONNECTIONS_ST *pstConns);
int DSC_OcsUpdateConnections(const OCS_CONNECTIONS_ST *pstConns);
int DSC_OcsReplaceConnections(const OCS_CONNECTIONS_ST *pstConns);
int DSC_OcsDeleteConnections(const OCS_CONNECTIONS_ST *pstConns);
int DSC_OcsDeleteAllConnections();
int DSC_OcsDeleteConnectionByIndex(uint32_t uiIndex);

/*utils by csm and operationd*/
int DSC_OcsGetSourcePort(char *pcXpath, char *pcPort);
int DSC_OcsPortNumber(char *pcPort, char *prefix);


#ifdef __cplusplus
}
#endif

#endif

