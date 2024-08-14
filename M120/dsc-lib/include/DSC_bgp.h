/***MODU+******************************************************************************************
* Copyright(C), 2021, Molex Tech.Co.,Ltd
* FileName    : dsc_bgp.h
* Description : Define the bgp control functions.
* History     :
*     [Author]      [Date]          [Version]        [Description]
* [1] Chun Ye       2021/06/05      Ver 1.0.0        Initial file.
***MODU-******************************************************************************************/


#ifndef __DSC_BGP_H_
#define __DSC_BGP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "util_cmn.h"

/**************************************************************************************************/
/* define the structure of bgp configuration                                                      */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_BGP_NEIGHBOR_MAX_CNT      (32)

typedef struct {
    char acIpAddr[DEF_BUF_SIZE_32];
    char acPeerGroup[DEF_BUF_SIZE_32];
    uint32_t ulRemoteAs;
} ST_BGP_NEIGHBOR;

typedef struct {
    uint32_t ulAs;
    char acRouterId[DEF_BUF_SIZE_32];
    uint32_t ulNeighborNum;
    ST_BGP_NEIGHBOR astNeighbor[DEF_BGP_NEIGHBOR_MAX_CNT];
} ST_BGP_CONFIG;
/* } @                                                                                            */

/**************************************************************************************************/
/* define the xpath of bgp                                                                        */
/**************************************************************************************************/
/* @ {                                                                                            */
#define DEF_MODULE_BGP    "openconfig-bgp"
#define DEF_BGP_XPATH_PREFIX  "/" DEF_MODULE_BGP ":bgp"

#define DEF_BGP_XPATH_GLOBAL            DEF_BGP_XPATH_PREFIX "/global"
#define DEF_BGP_LEAF_AS                 "/config/as"
#define DEF_BGP_XPATH_CONFIG_AS         DEF_BGP_XPATH_GLOBAL DEF_BGP_LEAF_AS
#define DEF_BGP_LEAF_ROUTER_ID          "/config/router-id"
#define DEF_BGP_XPATH_CONFIG_ROUTER_ID  DEF_BGP_XPATH_GLOBAL DEF_BGP_LEAF_ROUTER_ID

#define DEF_BGP_XPATH_NEIGHBORS            DEF_BGP_XPATH_PREFIX "/neighbors/neighbor"
#define DEF_BGP_LEAF_CONFIG_NEIGHBOR_ADDR  "/config/neighbor-address"
#define DEF_BGP_LEAF_CONFIG_PEER_GROUP     "/config/peer-group"
#define DEF_BGP_XPATH_CONFIG_NEIGHBOR_ADDR DEF_BGP_XPATH_NEIGHBORS "[neighbor-address='%s']" DEF_BGP_LEAF_CONFIG_NEIGHBOR_ADDR
#define DEF_BGP_XPATH_CONFIG_PEER_GROUP    DEF_BGP_XPATH_NEIGHBORS "[neighbor-address='%s']" DEF_BGP_LEAF_CONFIG_PEER_GROUP


#define DEF_BGP_XPATH_PEER_GROUPS       DEF_BGP_XPATH_PREFIX "/peer-groups/peer-group"
#define DEF_BGP_LEAF_CONFIG_GROUP_NAME  "/config/peer-group-name"
#define DEF_BGP_LEAF_CONFIG_PEER_AS     "/config/peer-as"
#define DEF_BGP_XPATH_CONFIG_GROUP_NAME DEF_BGP_XPATH_PEER_GROUPS "[peer-group-name='%s']" DEF_BGP_LEAF_CONFIG_GROUP_NAME
#define DEF_BGP_XPATH_CONFIG_PEER_AS    DEF_BGP_XPATH_PEER_GROUPS "[peer-group-name='%s']" DEF_BGP_LEAF_CONFIG_PEER_AS


/* } @                                                                                            */

int DSC_GetBgpNeighborKey(char *pcXpath, char *pcKeyValue);
int DSC_GetBgpPeerGroupKey(char *pcXpath, char *pcKeyValue);
int DSC_GetBgpConfig(ST_BGP_CONFIG *stBgpConfig);

int DSC_SetBgpAs(uint32_t ulAsNumber);
int DSC_SetBgpRouterId(char *pcAddr);
int DSC_SetBgpNeighbor(char *pcAddr, uint32_t ulRemoteAs);

int DSC_DelBgpAs(uint32_t ulAsNumber);
int DSC_DelBgpRouterId(char *pcAddr);
int DSC_DelBgpNeighbor(char *pcAddr);



#ifdef __cplusplus
}
#endif

#endif

